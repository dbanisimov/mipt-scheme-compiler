;;;
;;; @file:std_forms.scm
;;; A number of standard scheme functions, implemented in scheme itself
;;; through means of more basic ones
;;;
;;; Copyright 2012 MIPT-COMPILER team
;;;

(define map
  (lambda (fun lst)
    (if (null? lst)
        '()
        (cons (fun (car lst)) (map fun (cdr lst))))))

(define cadr
  (lambda (lst)
    (car (cdr lst))))

(define first car)
(define second cadr)
(define third (lambda (lst) (cadr (cdr lst))))

(define let
  (macros (var-vals . body)
    `((lambda ,(map car var-vals)
        ,@body)
      ,@(map cadr var-vals))))

(define letrec
  (macros (var-vals . body)
    (let ((def (lambda (var-val) `(define ,(car var-val) ,(cadr var-val)))))
      `((lambda ()
          ,@(map def var-vals)
          ,@body)))))

(define let*
  (macros (var-vals . body)
    (if (null? var-vals)
        `((lambda () ,@body))
        `((lambda (,(car (car var-vals)))
            (let* ,(cdr var-vals) ,@body))
          ,(cadr (car var-vals))))))

(define append
  (lambda (l1 l2)
    (if (null? l1)
        l2
        (cons (car l1) (append (cdr l1) l2)))))

(define list
  (lambda elements
    elements))

(define reverse
  (lambda (lst)
    (letrec
        ((rev-rec (lambda (l tail)
                    (if (null? l)
                        tail
                        (rev-rec (cdr l) (cons (car l) tail))))))
      (rev-rec lst '()))))

(define member
  (lambda (elem list)
    (if (null? list)
        #f
        (if (equal? (car list) elem)
            #t
            (member elem (cdr list))))))

(define assoc
  (lambda (sym lst)
    (if (null? lst)
        #f
        (if (equal? (car (car lst)) sym)
            (car lst)
            (assoc sym (cdr lst))))))

(define when
  (macros (pred . body)
    `(if ,pred (begin ,@body))))

(define not
  (lambda (pred)
    (if pred #f #t)))

(define cond-expander
  (lambda (conditions)
    (when (not (null? conditions))
          `(if ,(car (car conditions))
               (begin ,@(cdr (car conditions)))
               ,(cond-expander (cdr conditions))))))

(define cond
  (macros (. conditions)
    (cond-expander conditions)))

(define or
  (macros (. preds)
    (if (null? preds)
        #f
        `(if ,(car preds)
             #t
             (or ,@(cdr preds))))))

(define and
  (macros (. preds)
    (if (null? preds)
        #t
        `(if ,(car preds)
             (and ,@(cdr preds))
             #f))))

(define do
  (macros (counters exit . body)
    (let ((var first)
          (init second)
          (incr third)
          (exit-cond (car exit))
          (exit-rez (cdr exit)))
      `(letrec ((loop
                 (lambda ,(map var counters)
                   (if ,exit-cond
                       (begin ,@exit-rez)
                       (begin ,@body
                              (loop ,@(map incr counters)))))))
         (loop ,@(map init counters))))))

(define string=?
  (lambda (a b)
    (let ((lena (string-length a))
          (lenb (string-length b)))
      (if (not (= lena lenb))
          #f
          (do ((i 0 (+ i 1)))
              ((or (= i lena)
                   (not (equal? (string-ref a i)
                                (string-ref b i))))
               (= i lena))))))) ; if i = lena then all characters are eq

(define vector=?
  (lambda (a b) 
    (let ((lena (vector-length a))
          (lenb (vector-length b)))
      (if (not (= lena lenb))
          #f
          (do ((i 0 (+ i 1)))
              ((or (= i lena)
                   (not (equal? (vector-ref a i)
                                (vector-ref b i))))
               (= i lena))))))) ; if i = lena then all characters are eq

(define match 
  (lambda (keywords pattern data)
    (cond ((pair? pattern)
           (if (not (pair? data))
               #f
               (let ((car-bindings (match keywords (car pattern) (car data))))
                 (if (not car-bindings)
                     #f
                     (let ((cdr-bindings (match keywords (cdr pattern) (cdr data))))
                       (if (not cdr-bindings)
                           #f
                           (append car-bindings cdr-bindings)))))))
          ((null? pattern)
           (if (null? data)
               '()
               #f))
          ((symbol? pattern)
           (if (member pattern keywords)
               (if (eqv? pattern data)
                   '()
                   #f)
               (list (cons pattern data))))
          (#t #f))))

(define replace-all
  (lambda (bindings expr)
    (cond ((pair? expr)
           (cons (replace-all bindings (car expr))
                 (replace-all bindings (cdr expr))))
          ((symbol? expr)
           (let ((rpl (assoc expr bindings)))
             (if rpl
                 (cdr rpl)
                 expr)))
          (#t expr))))

(define syntax-rules
  (macros (keywords . pattern->templates)
    `(macros (. args)
      (letrec ((choose-syntax-case
                (lambda (substs)
                  (if (null? substs)
                      '()
                      (let ((bindings (match ',keywords (first (car substs)) args)))
                        (if bindings
                            (replace-all bindings (second (car substs)))
                            (choose-syntax-case (cdr substs))))))))
        (choose-syntax-case ',pattern->templates)))))


(define redefine
  (syntax-rules ()
    ((name (old-name) body)
     (let ((old-name name))
       (set! name body)))))

;; Examples of usage redefine:
(redefine + (add)
  (lambda summands
    (if (null? summands)
        0
        (add (car summands (apply + (cdr summands)))))))

#|
(redefine let (old-let)
  (syntax-rules ()
    ((name var-inits . body)
     (old-let ()
       (define name)
       (set! name (lambda vars
|#
(define equal?
  (lambda (a b)
    (or (eqv? a b)
        (cond
            ((and (pair? a) (pair? b))
             (and (equal? (car a) (car b))
                  (equal? (cdr a) (cdr b))))
          ((and (string? a) (string? b))
           (string=? a b))
          ((and (vector? a) (vector? b))
           (vector=? a b))
          (#t #f)))))


