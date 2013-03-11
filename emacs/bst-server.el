;;; -*- mode: emacs-lisp; coding: utf-8 -*-

;;; a control connection from emacs to a running lua vm to remotely initiate the evaluation of lua code in the VM.
;;; uses json to send/receive messages.

;;; probably writing a Slime backend would be a better idea
;;; https://github.com/csrhodes/swankr
;;; https://github.com/matsu911/swank-lua

(require 'json)

(defvar bst/port 9999
  "port of the server")

(defvar bst/host nil
  "host of the server")

(defvar bst/serial-device-file "/dev/ttyUSB0"
  "tty file of the device when using the serial backend")

(defvar bst/eom-marker "\nQoOqmr5+ThM=\n"
  "End Of Message marker. A random string that needs to match the lua side. This is pretty much a KLUDGE, but how else?")

(defvar bst/process-name "*bst listener*")

(defvar bst/connections '())

(defvar bst/server-process nil)

(defvar bst/server-message-buffer-name "*bst-server-log*"
  "The buffer where log messages are inserted.")

;;;;;;
;;; utils

(defun bst/log (msg &rest args)
  (with-current-buffer (get-buffer-create bst/server-message-buffer-name)
    (goto-char (point-max))
    (insert (format "%s: %s\n"
		    (format-time-string "%Y-%m-%d %H:%M:%S")
		    (if (car-safe args)
			(apply 'format `(,msg ,@args))
                        msg)))))

(defun bst/default-connection ()
  (or (bst/find-connection/by-process (get-buffer-process (current-buffer)))
      (car bst/connections)
      (error "No default BST connection")))

(defun bst/find-connection/by-process (process)
  (let ((connection (assoc process bst/connections)))
    (if connection
        (if (member (process-status process) '(run stop open))
            connection
            (progn
              (bst/delete-connection/by-process process)
              nil))
        nil)))

(defun bst/delete-connection/by-process (process)
  (setf bst/connections (remove* process bst/connections :key 'car))
  (when (process-buffer process)
    (kill-buffer (process-buffer process))))

(defun bst/flash-region (start end &optional timeout)
  "Temporarily highlight region from START to END."
  (let ((overlay (make-overlay start end)))
    (overlay-put overlay 'face 'secondary-selection)
    (run-with-timer (or timeout 0.2) nil 'delete-overlay overlay)))

(defun bst/region-for-toplevel-form-at-point ()
  (save-excursion
    (save-match-data
      (beginning-of-line)
      (let* ((start (point))
             (end start))
        (while (and (looking-at "[ \n]")
                    (not (bobp)))
          (beginning-of-line 0)
          (setq start (point)))
        (goto-char end)
        (beginning-of-line 2)
        (while (and (looking-at "[ \n]")
                    (not (eobp)))
          (beginning-of-line 2)
          (setq end (point)))
        (unless (eobp)
          (beginning-of-line 2)
          (setq end (point)))
        (list start end)))))

;;;;;;
;;; start/stop

;; FIXME: the ip and serial backends can't really work simpultaneously without stepping on each other's toes
;; e.g. clearing bst/connections

(defun bst/start-ip-server ()
  (interactive)
  (unless (process-status bst/process-name)
    (setq bst/server-process
          (make-network-process :name bst/process-name
                                ;; :buffer "*listen*"
                                :family 'ipv4
                                :server t
                                :nowait t
                                :host bst/host
                                :service bst/port
                                :coding 'utf-8-unix
                                :sentinel 'bst/listen-sentinel
                                :filter 'bst/listen-filter))
    (setq bst/connections '())
    (with-current-buffer (get-buffer-create bst/server-message-buffer-name)
      (delete-region (point-min) (point-max)))))

(defun bst/stop-server ()
  (interactive)
  (when bst/server-process
    (while bst/connections
      (bst/delete-connection/by-process (first (pop bst/connections))))
    (delete-process bst/server-process)
    (setq bst/server-process nil)))

(defun bst/start-serial-server ()
  (interactive)
  (unless (process-status bst/process-name)
    (setq bst/server-process
          (make-serial-process :name bst/process-name
                               :port bst/serial-device-file
                               :speed 115200
                               :coding 'utf-8-unix
                               :filter 'bst/listen-filter
                               :sentinel 'bst/listen-sentinel
                               :bytesize 8
                               :parity nil
                               :stopbits 1
                               :flowcontrol 'hw))
    (setq bst/connections '())
    (with-current-buffer (get-buffer-create bst/server-message-buffer-name)
      (delete-region (point-min) (point-max)))))

(defun bst/listen-filter (process data)
  ;;(message (format "filter: '%s' %s" data proc))
  (let ((buffer (or (process-buffer process)
                    ;; Set the process buffer (because the server doesn't automatically allocate them)
                    ;; the name of the buffer has the client port in it
                    ;; the space in the name ensures that emacs does not list it
                    (let* ((port (cadr (process-contact process)))
                           (buffer (get-buffer-create (format "*bst-connection-%s*" port))))
                      (set-process-buffer process buffer)
                      (with-current-buffer buffer
                        (setq buffer-read-only t))
                      (process-buffer process))))
        (connection (assoc process bst/connections)))
    (unless connection
      (setq bst/connections (cons (list process :last-message-end 1) bst/connections))
      (setq connection (assoc process bst/connections)))
   (flet ((set-last-message-end-point (point)
            (setf (getf (cdr connection) :last-message-end) point))
          (get-last-message-end-point ()
            (getf (cdr connection) :last-message-end)))
     (with-current-buffer buffer
       (save-excursion
         (let ((inhibit-read-only t))
           (goto-char (point-max))
           (insert data)
           (let ((message-start-point (let ((start (get-last-message-end-point)))
                                        (bst/log "expecting eom marker at %s" start)
                                        (if (and (> (buffer-size) (+ start (length bst/eom-marker)))
                                                 (equal bst/eom-marker (buffer-substring start (+ start (length bst/eom-marker)))))
                                            (+ start (length bst/eom-marker))
                                            (progn
                                              (bst/log "resyncing to eom marker at %s" start)
                                              (goto-char start)
                                              (when (search-forward bst/eom-marker nil t)
                                                (set-last-message-end-point (- (point) (length bst/eom-marker)))))))))
             (when message-start-point
               (goto-char message-start-point)
               ;; try to search for the next EOM marker
               (bst/log "looking for the next marker starting at %s" message-start-point)
               (when (search-forward bst/eom-marker nil t)
                 (let* ((end-point (- (point) (length bst/eom-marker)))
                        (message-string (buffer-substring message-start-point end-point))
                        (json-object-type 'plist)
                        (message (json-read-from-string message-string)))
                   (bst/log "got message: '%s'" message)
                   (set-last-message-end-point end-point)
                   (bst/process-message process message)
                   (bst/log "finished processing message")))))))))))

(defun bst/listen-sentinel (process status)
  (cond
    ;; Server status
    ((and (eq bst/server-process process)
          (equal status "deleted\n"))
     ;;(let ((server-bugger (get-buffer bst/server-message-buffer-name)))
     ;;  (when server-bugger
     ;;    (kill-buffer server-bugger)))
     (bst/log "server socket stopped"))

    ;; Client socket status
    ((equal status "connection broken by remote peer\n")
     (bst/delete-connection/by-process process)
     (bst/log "connection broken by remote peer"))

    ((equal status "open\n") ;; this says "open from ..."
     (bst/log "opened new connection"))

    ;; Default
    (t
     (bst/log "sentinel status: %s" status))))

(defun bst/send-message (object)
  (interactive)
  (let ((message (json-encode object))
        (process (car (bst/default-connection))))
    (bst/log "sending message: '%s'" message)
    (process-send-string process (concatenate 'string message bst/eom-marker))))

;;;;;;
;;; message handlers

(defvar bst/message-handlers (make-hash-table :test 'equal))

(defmacro define-message-handler (name &rest body)
  `(setf (gethash ,name bst/message-handlers)
         (lambda (-message-)
           ,@body)))

(define-message-handler "bst connect request"
  (bst/log "got connect request, sending response.")
  (bst/send-message '(:type "bst connect response")))

(define-message-handler "eval response"
  (let ((error-message (prin1-to-string (getf -message- :error) nil))
        (return-value (prin1-to-string (getf -message- :return-value) nil)))
    (bst/log "got eval response. Error message: %s, return-value: %s." error-message return-value)
    (if (eq (getf -message- :result) t)
        (message (concatenate 'string "LUA result: " return-value))
        (message (concatenate 'string "LUA side error: " error-message)))))

(define-message-handler "pong"
  (message "Got pong from LUA."))

(defun bst/process-message (process message)
  (let* ((type (getf message :type))
         (handler (gethash type bst/message-handlers)))
    (if handler
        (funcall handler message)
        (error "Unexpected BST message: '%s' from %s" message process))))

;;;;;;
;;; user commands

(defun bst/eval (form)
  (interactive)
  (bst/send-message `(:type "eval request" :form ,form)))

(defun bst/eval-region ()
  (interactive)
  (if mark-active
      (bst/eval (buffer-substring (region-beginning) (region-end)))
      (error "Please select a region")))

(defun bst/eval-toplevel-form-at-point ()
  (interactive)
  (destructuring-bind (start end) (bst/region-for-toplevel-form-at-point)
    (bst/flash-region start end)
    (bst/eval (buffer-substring start end))))

(defun bst/disconnect ()
  (interactive)
  (bst/send-message '(:type "disconnect")))

;;;;;;
;;; key bindings

(define-key lua-mode-map (kbd "C-c C-c") 'bst/eval-toplevel-form-at-point)
(define-key lua-mode-map (kbd "C-c C-r") 'bst/eval-region)

;(bst/start-serial-server)
;(bst/start-ip-server)
;(sleep-for 300)
;(bst/stop-server)
