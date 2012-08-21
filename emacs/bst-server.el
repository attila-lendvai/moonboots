;;; -*- mode: emacs-lisp; coding: utf-8 -*-

(require 'json)

(defvar bst/port 9999
  "port of the server")

(defvar bst/host nil
  "host of the server")

(defvar bst/eom-marker "\n\n\n"
  "End Of Message marker")

(defvar bst/process-name "bst listener")

(defvar bst/clients '())

(defvar bst/server-socket nil)

(defvar bst/server-message-buffer-name "*bst-server-log*"
  "The buffer where log messages are inserted.")

(defun bst/log (msg &rest args)
  (with-current-buffer (get-buffer-create bst/server-message-buffer-name)
    (goto-char (point-max))
    (insert (format "%s: %s\n"
		    (format-time-string "%Y-%m-%d %H:%M:%S")
		    (if (car-safe args)
			(apply 'format `(,msg ,@args))
                        msg)))))

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
    (setq bst/clients '())
    (with-current-buffer (get-buffer-create bst/server-message-buffer-name)
      (delete-region (point-min) (point-max)))))

(defun bst/stop ()
  (interactive)
  (when bst/server-socket
    (while bst/clients
      (delete-process (car (pop bst/clients))))
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
        (client-entry (assoc process bst/clients)))
    (unless client-entry
      (setq bst/clients (cons (list process :last-message-start 1) bst/clients))
      (setq client-entry (assoc process bst/clients)))
   (with-current-buffer buffer
     (let ((inhibit-read-only t)
           (message-start-point (getf (cdr client-entry) :last-message-start)))
       (save-excursion
         (goto-char (point-max))
         (insert data)
         (goto-char message-start-point)
         (when (re-search-forward bst/eom-marker nil 't)
           (let* ((message-string (buffer-substring message-start-point (point)))
                  (json-object-type 'plist)
                  (message (json-read-from-string message-string)))
             (bst/log "got message: '%s' %s" message process)
             (setf (getf (cdr client-entry) :last-message-start) (point))
             (bst/process-message process message)
             (bst/log "finished processing message: %s" process))))))))

(defun bst/process-message (process message-string)
  (cond
    ((string= (getf message :request) "hello bst!")
     (bst/log "got hello, sending result: %s" process)
     (bst/send-message '(:result t)))
    (t
     (error "Unexpected message: '%s' from %s" message process))))

(defun bst/send-message (object)
  (interactive)
  (let ((message (json-encode object))
        (process (get-buffer-process (current-buffer))))
    (bst/log "sending message: '%s' to %s" message process)
    (process-send-string process (concatenate 'string message bst/eom-marker))))

(defun bst/listen-sentinel (process status)
  (cond
    ;; Server status
    ((and (eq bst/server-socket process)
          (equal status "deleted\n"))
     (kill-buffer (get-buffer bst/server-message-buffer-name))
     (bst/log "server stopped"))

    ;; Client socket status
    ((equal status "connection broken by remote peer\n")
     (when (process-buffer process)
       (kill-buffer (process-buffer process))
       (setf bst/clients (remove* process bst/clients :key 'car))
       (bst/log "connection broken by remote peer")))

    ((equal status "open\n") ;; this says "open from ..."
     (bst/log "Elnode opened new connection"))

    ;; Default
    (t
     (bst/log "status: %s %s" process status))))

;(bst/start)
;(sleep-for 300)
;(bst/stop)
