;;; -*- mode: emacs-lisp; coding: utf-8 -*-

(require 'json)

(defvar bst/port 9999
  "port of the server")

(defvar bst/host nil
  "host of the server")

(defvar bst/eom-marker "\n\n\n"
  "End Of Message marker")

(defvar bst/process-name "bst listener")

(defvar bst/connections '())

(defvar bst/server-socket nil)

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

;;;;;;
;;; start/stop

(defun bst/start ()
  (interactive)
  (unless (process-status bst/process-name)
    (setq bst/server-socket
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

(defun bst/stop ()
  (interactive)
  (when bst/server-socket
    (while bst/connections
      (bst/delete-connection/by-process (pop bst/connections)))
    (delete-process bst/server-socket)
    (setq bst/server-socket nil)))

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
      (setq bst/connections (cons (list process :last-message-start 1) bst/connections))
      (setq connection (assoc process bst/connections)))
   (with-current-buffer buffer
     (let ((inhibit-read-only t)
           (message-start-point (getf (cdr connection) :last-message-start)))
       (save-excursion
         (goto-char (point-max))
         (insert data)
         (goto-char message-start-point)
         (when (re-search-forward bst/eom-marker nil 't)
           (let* ((message-string (buffer-substring message-start-point (point)))
                  (json-object-type 'plist)
                  (message (json-read-from-string message-string)))
             (bst/log "got message: '%s' %s" message process)
             (setf (getf (cdr connection) :last-message-start) (point))
             (bst/process-message process message)
             (bst/log "finished processing message: %s" process))))))))

(defun bst/listen-sentinel (process status)
  (cond
    ;; Server status
    ((and (eq bst/server-socket process)
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
     (bst/log "Elnode opened new connection"))

    ;; Default
    (t
     (bst/log "status: %s %s" process status))))

(defun bst/process-message (process message)
  (let ((type (getf message :type)))
    (cond
      ((string= type "bst connect request")
       (bst/log "got hello, sending result: %s" process)
       (bst/send-message '(:type "bst connect response")))
      ((string= type "eval response")
       (bst/log "got eval response: %s" process)
       (if (eq (getf message :result) t)
           (message (prin1-to-string (getf message :return-value) nil))
           (message (prin1-to-string (getf message :error) nil))))
      (t
       (error "Unexpected message: '%s' from %s" message process)))))

;;;;;;
;;; commands

(defun bst/send-message (object)
  (interactive)
  (let ((message (json-encode object))
        (process (car (bst/default-connection))))
    (bst/log "sending message: '%s' to %s" message process)
    (process-send-string process (concatenate 'string message bst/eom-marker))))

(defun bst/eval (form)
  (interactive)
  (bst/send-message `(:type "eval request" :form ,form)))

(defun bst/eval-region ()
  (interactive)
  (if mark-active
      (bst/eval (buffer-substring (region-beginning) (region-end)))
      (error "Please select a region")))

(defun bst/flash-region (start end &optional timeout)
  "Temporarily highlight region from START to END."
  (let ((overlay (make-overlay start end)))
    (overlay-put overlay 'face 'secondary-selection)
    (run-with-timer (or timeout 0.2) nil 'delete-overlay overlay)))

(defun bst/eval-toplevel-form-at-point ()
  (interactive)
  (destructuring-bind (start end) (bst/region-for-toplevel-form-at-point)
    (bst/flash-region start end)
    (bst/eval (buffer-substring start end))))

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

(defun bst/disconnect (form)
  (interactive)
  (bst/send-message '(:type "disconnect")))

;;;;;;
;;; key bindings

(define-key lua-mode-map (kbd "C-c C-c") 'bst/eval-toplevel-form-at-point)
(define-key lua-mode-map (kbd "C-c C-r") 'bst/eval-region)

;(bst/start)
;(sleep-for 300)
;(bst/stop)
