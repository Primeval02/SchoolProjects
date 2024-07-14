;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; LISP menu Demo
;; CMPS 3500 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Menu Set Up
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun options ()
  (print "----------------------------")
  (print "---- Final test in LISP ----")
  (print "----------------------------")
  (print "Please pick an Option:      ")
  (print "----------------------------")
  (print "For problem 1 press 1.")
  (print "For problem 2 press 2.")
  (print "For problem 3 press 3.")
  (print "For problem 4 press 4.")
  (print "For problem 5 press 5.")
  (print "To Quit press 6.      ")
  (terpri)
  (let ((opt (read))) 
    (case opt
      (1 (problem_1))
      (2 (problem_2))
      (3 (problem_3))
      (4 (problem_4))
      (5 (problem_5))
      (6 (quit))
      (otherwise (options))))
)

;; Function Definintons
(defun reverseList (lst)
  (if (null lst)
    nil
    (append (reverseList (cdr lst)) (list (car lst)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Problem 1
(defun problem_1()
    (print "Code for problem 1 goes here")


    (defvar *input-file* (open "/path/to/input/file.txt" :if-does-not-exist nil))

    (defvar *output-file* (open "/path/to/output/file.txt" :direction :output :if-exists :supersede))

    (defun read-from-file ()
        (read-line *input-file*))

    (defun write-to-file (data)
        (format *output-file* "~a~%" data))

    (defun close-files ()
        (close *input-file*)
          (close *output-file*))

    (terpri)
    (options)
)

;; Problem 2
(defun problem_2(lst)
    (print "Code for problem 2 goes here")
    (let ((myList '(1 2 3 4 5)))
      (format t "Original list: ~a~%" myList)
      (let ((reversed (reverseList myList)))
        (format t "Reversed list: ~a~%" reversed)))
    (terpri)
    (options)
)

;; Problem 3
(defun problem_3()
    (print "Code for problem 3 goes here")
    (terpri)
    (options)
)

;; Problem 4
(defun problem_4()
    (print "Code for problem 4 goes here")
    (terpri)
    (options)
)

;; Problem 5
(defun problem_5()
    (print "Code for problem 5 goes here")
    (terpri)
    (options)
)

;; Call menu here
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(options)
