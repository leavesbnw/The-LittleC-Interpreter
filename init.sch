(defmacro define (form body)
  (if (pair? form)
      `(set! ,(car form) (lambda ,(cdr form) ,body))
      `(set! ,form ,body)))

(defmacro let (var-val-list body)
  (progn
   (define (get-val-list vlist)
     (if (null? vlist)
	 ()
	 (cons (car (cdr (car vlist))) (get-val-list (cdr vlist)))))
   (define (get-var-list vlist)
     (if (null? vlist)
	 ()
	 (cons (car (car vlist)) (get-var-list (cdr vlist)))))
   `((lambda ,(get-var-list var-val-list) ,body) ,@(get-val-list var-val-list))))

(let ((a 1) (b 2)) (+ a b))




