C********************************************************************
C NAME: Bradley Tate  
C ASGT: Activity 2
C ORGN: CSUB - CMPS 3500
C FILE: matrixops.f
C DATE: 02/19/2023
C********************************************************************
C This program implements matrix operations:
C    - Addition and subtraction
C    - Multiplication, dotproduct
C    - scalar times x
C ********************************************************************
C     Define variables as arrays of 2 and 1 dimensions
      PROGRAM matrixops
      REAL m1(5,5), m2(5,5), transp1(5,5), transp2(5,1) 
      REAL mat_sum(5,5), mat_subs(5,5), prod1(5,5), prod2(5,5) 
      REAL v1(1,5), power1(5,5), power2(5,5), op1(5,5)
      REAL dotprod
      INTEGER SIZE1, SIZE2, SIZE3, I
      REAL, ALLOCATABLE :: mtemp1(:), mtemp2(:), mtemp3(:)

      

C     ****************************************
C     Reading m1.in
C     ****************************************
      OPEN(1,FILE='m1.in',ERR=2002)
      SIZE1=0 
C     Getting size of matrix in file
  10  READ(1,*,END=20) 
      SIZE1 = SIZE1 + 1
      GO TO 10

C     if empty exit file
  20  CONTINUE
      IF(SIZE1.EQ.0) GO TO 2002

C     Reposition array to beginning
      REWIND(1)

C     Allocating size of new matrix
      ALLOCATE (mtemp1(1:SIZE1))

C   Set values of A and B
      DO 30 I=1,SIZE1
      READ(1,*) mtemp1(I)
  30  CONTINUE

C     Reshape function from a 1D to 2D array         
      m1 = RESHAPE(mtemp1,(/5, 5/))

C     ****************************************
C     Reading m2.in
C     ****************************************
      OPEN(2,FILE='m2.in',ERR=2002)
      SIZE2=0 

C     Getting size of matrix in file
  40  READ(2,*,END=50) 
      SIZE2 = SIZE2 + 1
      GO TO 40

C     if empty exit file
  50  CONTINUE
      IF(SIZE2.EQ.0) GO TO 2002

C     Reposition array to beginning
      REWIND(2)

C     Allocating size of new matrix
      ALLOCATE (mtemp2(1:SIZE2))

C   Set values of A and B
      DO 60 I=1,SIZE2
      READ(2,*) mtemp2(I)
  60  CONTINUE

C     Reshape function from a 1D to 2D array         
      m2 = RESHAPE(mtemp2,(/5, 5/))

C     ****************************************
C     Reading v1.in
C     ****************************************
      OPEN(3,FILE='v1.in',ERR=2002)
      SIZE3=0 

C     Getting size of matrix in file
  70  READ(3,*,END=80) 
      SIZE3 = SIZE3 + 1
      GO TO 70

C     if empty exit file
  80  CONTINUE
      IF(SIZE3.EQ.0) GO TO 2002

C     Reposition array to beginning
      REWIND(3)

C     Allocating size of new matrix
      ALLOCATE (mtemp3(1:SIZE3))

C   Set values of A and B
      DO 90 I=1,SIZE3
      READ(3,*) mtemp3(I)
  90  CONTINUE

      v1 = RESHAPE(mtemp3,(/1, 5/))


C     ****************************************
C     Performing Operations
C     ****************************************

C     MATMUL will perform matrix multiplication
      prod1 = matmul(m1, m2)
      prod2 = matmul(m2, m1)
      transp1 = transpose(m1)
      transp2 = transpose(v1)
      power1 = power(m1, 10)
      power2 = power(m2, 20)
      op1 = power(transpose(matmul(m1, m2)), 25)

C     ****************************************
C     Writing Outputs
C     ****************************************

C     Printing outputs
      print *
      write(*,*) 'Program to show some matrix and vector operations'
      write(*,*) '*************************************************'
      print *

      write(*,*) ' m1 = '
      write( *, 2000) ((m1(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' m2 = '
      write( *, 2000)  ((m2(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' v1 = '
      write( *, 2001) v1
      print *

      write(*,*) ' prod1 = m1 * m2 '
      write(*,2000) ((prod1(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' prod2 = m2 * m1 '
      write(*,2000) ((prod2(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' transp1 = transpose(m1) '
      write(*,2000) ((transp1(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' transp2 = tranpose(v1) '
      write(*,2000) ((transp2(i,j),j=1,1),i=1,5)
      print *

      write(*,*) ' power1 = power(m1, 10) '
      write(*,2000) ((power1(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' power2 = power(m2, 20) '
      write(*,2000) ((power2(i,j),j=1,5),i=1,5)
      print *

      write(*,*) ' op1 = power(transpose(m1*m2), 25) '
      write(*,2000) ((op1(i,j),j=1,5),i=1,5)
      print *





C     Format output arrays
C     Format output for 5x5 array
 2000 format(5x,5f20.1)

C    Format output for 5x1 array
 2001 format ((5x,f8.1))
      stop

C     Erroring out of the file cannot be open or empty
 2002 PRINT *,' Empty or missing input file: array3a.in'
      STOP
      
      contains

C   Matrix Power Func
      REAL FUNCTION power(m, p)
      IMPLICIT NONE

      DIMENSION power(5,5)
      REAL m(5,5), res(5,5)
      INTEGER p, I

      power = m

      DO 100 I=2,p
          power = matmul(power, m)
 100  CONTINUE

      RETURN  
      END FUNCTION power

      END PROGRAM matrixops






