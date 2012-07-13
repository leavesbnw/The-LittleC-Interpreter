(defmacro defun (name arg body)
  `(set! ,name (lambda ,arg ,body)))

(defun append (list1 list2)
  (if (null list1)
      list2
      (cons (car list1) (append (cdr list1) list2))))

(defmacro let (var-val-list body)
  (progn
    (defun get-val-list (vlist)
      (if (null vlist)
	  nil
	  (cons (car (cdr (car vlist))) (get-val-list (cdr vlist)))))
    (defun get-var-list (vlist)
      (if (null vlist)
	  nil
	  (cons (car (car vlist)) (get-var-list (cdr vlist)))))
    (append `((lambda ,(get-var-list var-val-list) ,body)) (get-val-list var-val-list))))

(let ((x 1) (y 2)) (+ x y))
