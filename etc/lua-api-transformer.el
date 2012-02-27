
(defun lua/trim-string (str)
  (while (string-match "\\`\n+\\|^\\s-+\\|\\s-+$\\|\n+\\'"
                       str)
    (setq str (replace-match "" t t str)))
  str)

(defun lua/constant ()
  (interactive)
  (re-search-forward "^#define[ \t]+\\([_a-zA-Z0-9]+\\)[ \t]+")
  (let ((name (buffer-substring-no-properties (match-beginning 1) (match-end 1))))
    (goto-char (match-beginning 0))
    (delete-region (match-beginning 0) (match-end 0))
    (insert "\tBIND(")
    (insert name)
    (insert ")")
    (insert "\t\t\t// ")))

(global-set-key (kbd "C-\\") 'lua/constant)








;;; dead code below



(defun lua/map-type (c-type &optional pointer? result-type-length)
  (let ((match (assoc (lua/trim-string c-type)
                      '(("BYTE" "unsigned-char")
                        ("DWORD" "unsigned-int")
                        ("ULONG" "unsigned-long")
                        ("STR" "unsigned-char")
                        ("BOOL" "boolean")
                        ("UCHAR" "unsigned-char")
                        ("USHORT" "unsigned-short")
                        ("void" "void")))))
    (unless match
      (error "lua/map-type: unable to match %s" c-type))
    (let ((mapped-type (second match)))
      (cond
        (result-type-length
         (concat "(result (array " mapped-type " " result-type-length "))"))
        (pointer?
         (cond
           ((string= mapped-type "unsigned-char") "string")
           (t (concat "(pointer " mapped-type ")"))))
        (t
         mapped-type)))))

(defun lua/function ()
  (interactive)
  (re-search-forward "^\\([_a-zA-Z0-9]+\\)[ \t]+CTOS_\\([_a-zA-Z0-9]*\\)[ \t]*(\\(.*\\));")
  (goto-char (match-beginning 0))
  (flet ((match-value (index)
           (buffer-substring-no-properties (match-beginning index) (match-end index))))
    (let ((return-type (match-value 1))
          (name (match-value 2))
          (args (match-value 3)))
      (delete-region (match-beginning 0) (match-end 0))
      (insert "(")
      (insert (lua/map-type return-type))
      (insert " ")
      (insert name)
      (unless (string= args "void")
        (let ((args (split-string args ",")))
          (dolist (entry args)
            (insert " ")
            (flet ((match-value (index)
                     (let ((begin (match-beginning index))
                           (end (match-end index)))
                       (when (and begin end)
                         (subseq entry begin end)))))
              (cond
                ((string-match "[ \t]*\\(.*?\\)\\([*]\\)[ \t]*[_a-zA-Z0-9]+\\(\\[\\([0-9]*\\)\\]\\)?" entry)
                 (let ((type (match-value 1))
                       (result-type-length (match-value 4)))
                   (insert (lua/map-type type t result-type-length))))
                ((string-match "[ \t]*\\(.*?\\)[ \t][_a-zA-Z0-9]+\\(\\[\\([0-9]*\\)\\]\\)?" entry)
                 (let ((type (match-value 1))
                       (result-type-length (match-value 3)))
                   (insert (lua/map-type type nil result-type-length)))))))))
      (insert ")"))))

(global-set-key (kbd "C-\\") 'lua/function)
