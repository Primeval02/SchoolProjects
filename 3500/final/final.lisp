;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Name: Bradley Tate
;; Course: CMPS 3500 
;; Assignment: LISP Final
;; Date: 5/21/2023
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

;; Problem 1 Funcs
(defun split-string (string delimiter)
  (loop with start = 0
        for end = (position delimiter string :start start)
        while end collect (subseq string start end) do
        (setf start (1+ end))))

(defun unique-words (infile outfile)
  (let* ((word-counts (make-hash-table :test #'equal))
         (input-stream (open infile :direction :input))
         (output (open outfile :direction :output :if-exists :supersede)))
    ;; Read the input file and count word occurrences
    (loop for line = (read-line input-stream nil)
          while line do
          (loop for word in (split-string line #\Space)
                unless (string= word "")
                do (let ((word-lowercase (string-downcase word)))
                     (incf (gethash word-lowercase word-counts 0)))))
    ;; Write the unique words to the output file
    (with-standard-io-syntax
      (loop for word being the hash-keys of word-counts
            do (format output "~a~%" word)))
    ;; Close the streams
    (close input-stream)
    (close output)))



;; Problem 2 Func
(defun reverse-list (lst)
  (if (null lst)
    nil
    (append (reverse-list (cdr lst)) (list (car lst)))))



;; Problem 3 Funcs
(defun word-counts (words)   ; counts the occurences
  (let ((counts (make-hash-table :test #'equal)))
    (dolist (word words)
      (let ((word-lowercase (string-downcase word)))
        (incf (gethash word-lowercase counts 0))))
    counts))

(defun split-string (string delimiter) ; divides the strings (lines) into words
  (let ((result '())
        (start 0)
        (end (position delimiter string)))
    (loop while end do
          (let ((substring (subseq string start end)))
            (unless (string= substring "") ; Exclude empty substrings: whitespaces etc.
              (push substring result)))
          (setq start (1+ end))
          (setq end (position delimiter string :start start)))
    (nreverse result))) ; Return the result without reversing

(defun most-common (filename)
  (let* ((words (with-open-file (stream filename :direction :input)
                  (loop for line = (read-line stream nil)
                        while line
                        append (split-string line #\Space))))
         (word-counts (word-counts words))
         (sorted-words (sort (loop for key being the hash-keys of word-counts
                                   collect key)
                             #'(lambda (a b)
                                 (> (gethash a word-counts)
                                    (gethash b word-counts))))))
    (loop for word in (subseq sorted-words 0 10)
          do (format t "~a: ~d occurrences~%"
                     word (gethash word word-counts)))))

;; Problem 4 Func
(defun power-set (lst)
  (if (null lst)
    '(())
    (let ((rest (power-set (cdr lst))))
      (append rest (loop for subset in rest
                         collect (cons (car lst) subset))))))

;; Problem 5 Func
(defun intercalate (list1 list2)
  (if (/= (length list1) (length list2))
    (print "Lists must have the same length, please try again!") ; if lists are not same size retry
    (if (or (null list1) (null list2))
      '()
      (cons (car list1)
            (cons (car list2)
                  (intercalate (cdr list1) (cdr list2)))))))


  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;; Problem 1
  (defun problem_1()
    (print "Test unique-words:")
    (print "------------------")
    (unique-words "/home/stu/btate/3500/final/infile.data" "/home/stu/btate/3500/final/outfile.data")
    (print "Output file has been generated! Please check it out")
    (terpri)
    (options)
    )

  ;; Problem 2
  (defun problem_2()
    (print "Test Recursive Reverse:")
    (print "-----------------------")
    (terpri)

    (let ((myList '(1)))
      (format t "Original list: ~a~%" myList)
      (let ((reversed (reverse-list myList)))
        (format t "Reversed list: ~a~%" reversed)))
    (terpri)

    (let ((myList '(1 2 3)))
      (format t "Original list: ~a~%" myList)
      (let ((reversed (reverse-list myList)))
        (format t "Reversed list: ~a~%" reversed)))
    (terpri)

    (let ((myList '(1 (2 4 6) 3)))
      (format t "Original list: ~a~%" myList)
      (let ((reversed (reverse-list myList)))
        (format t "Reversed list: ~a~%" reversed)))
    (terpri)



    (options)
    )

  ;; Problem 3
  (defun problem_3()
    (most-common "/home/stu/btate/3500/final/text.data")
    (terpri)
    (options)
    )

  ;; Problem 4
  (defun problem_4()
    (print "Test Recursive Power of a Set:")
    (print "------------------------------")
    (terpri)

    (let ((myList '(1)))
      (format t "Original list: ~a~%" myList)
      (let ((powerSet (power-set myList)))
        (format t "Powerset: ~a~%" powerSet)))
    (terpri)
    (let ((myList '(1 2 3)))
      (format t "Original list: ~a~%" myList)
      (let ((powerSet (power-set myList)))
        (format t "Powerset: ~a~%" powerSet)))
    (terpri)
    (let ((myList '(1 2 3 (4))))
      (format t "Original list: ~a~%" myList)
      (let ((powerSet (power-set myList)))
        (format t "Powerset: ~a~%" powerSet)))
    (terpri)
    (options)
    )

  ;; Problem 5
  (defun problem_5()
    (print "Test intercalate:")
    (print "-----------------")
    (terpri)
    (print "(print (intercalate '(1) '(3))):")
    (print (intercalate '(1) '(3)))
    (terpri)

    (print "(print (intercalate '(1 2) '(3 4))):")
    (print (intercalate '(1 2) '(3 4)))
    (terpri)

    (print "(print (intercalate '(1 3 5) '(2 4 6))):")
    (print (intercalate '(1 3 5) '(2 4 6)))
    (terpri)


    (print "(print(intercalate '(1 3 5) '(2))):")
    (print (intercalate '(1 3 5) '(2)))
    (terpri)

    (options)
    )

  ;; Call menu here
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (options)
