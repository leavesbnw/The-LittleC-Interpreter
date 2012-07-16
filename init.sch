(defmacro defun (name arg body)
  `(set! ,name (lambda ,arg ,body)))

(defun append (list1 list2)
  (if (null? list1)
      list2
      (cons (car list1) (append (cdr list1) list2))))

(defmacro let (var-val-list body)
  (progn
   (defun get-val-list (vlist)
     (if (null? vlist)
	 ()
	 (cons (car (cdr (car vlist))) (get-val-list (cdr vlist)))))
   (defun get-var-list (vlist)
     (if (null? vlist)
	 ()
	 (cons (car (car vlist)) (get-var-list (cdr vlist)))))
   (append `((lambda ,(get-var-list var-val-list) ,body)) (get-val-list var-val-list))))


(defun addx (x)
 (lambda (y) (+ y x)))

(set! add8 (addx 8))
(set! add9 (addx 9))

(add8 100)
(add9 100)


(defun addx (x) (lambda (y) (+ x y)))

(defun add1 (y) ((addx 1) y))

((addx 1) 2)

(add1 2)
