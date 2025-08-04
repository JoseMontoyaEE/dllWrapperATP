      SUBROUTINE DEVT69
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      INCLUDE  'blkcom.ins'
      INCLUDE  'sixty4.ins'
      COMMON  / C0B107 /   VOLTI (  400 )
      CHARACTER*6   NAMEA6(10)
      DIMENSION     NUMARG(10)
      DATA  NAMEA6  / 'DEV69A', 'DEV69B', 'DEV69C', 'DEV69D', 'DEV69E',
     1                'DEV69F', 'DEV69G', 'DEV69H', 'DEV69I', 'DEV69J' /
      DATA  NUMARG  /    2    ,    1    ,    5    ,    0    ,    0    ,
     1                   0    ,    0    ,    0    ,    0    ,    0     /
      DATA MAXFUN  / 10 /
      IF ( IPRSUP .GT. 0 )
     1 WRITE (LUNIT6, *)  ' Top of DEVT69.  Function # =',   LSTAT(12)
      IF ( LSTAT(12) .LE. 0  .OR.
     1     LSTAT(12) .GT. MAXFUN ) GO TO 9200
      GO TO ( 4100,  4200,  4300,  4400,  4500,
     1        4600,  4700,  4800,  4900, 5000 ), LSTAT(12)
 4100 D4 = 2.0 * VOLTI(1)
      D4 = D4 + VOLTI(2)
      VOLTI(1) = D4
      GO TO 9800
 4200 D4 = VOLTI(1) **2
      VOLTI(1) = D4
      GO TO 9800
 4300 D4 = 0.0
      DO 4318  I=1, 5
      D4 = D4 + VOLTI(I) * I
 4318 CONTINUE
      IF ( ISTEP .LE. 1 )  THEN
      WRITE (JUNIT6, 4370)  ( VOLTI(I), I=1, 5 )
 4370 FORMAT ( 'Function DEV69C, step 1.  Args =',  5F9.4 )
      CALL OUTSIX ( JUNIT6, 77 )
          ENDIF
      VOLTI(1) = D4
      GO TO 9800
 4400 D4 = COSZ ( 8.0 * T )
      IF ( ABSZ ( D4 ) .GT. 0.80 )
     1 D4 = 0.80
      VOLTI(1) = D4
      GO TO 9800
 4500 CONTINUE
 4600 CONTINUE
 4700 CONTINUE
 4800 CONTINUE
 4900 CONTINUE
 5000 CONTINUE
 9200 WRITE (JUNIT6, 2874)  LSTAT(12)
 2874 FORMAT ( ' Undefined user-supplied function number LSTAT(12) =',
     1         I5,  '.   Halt in  DEVT69.' )
 3481 CALL OUTSIX ( JUNIT6(1:KOL132), KOL132 )
      CALL STOPTP
      ENTRY INIT69
      DO 1834  J=1, MAXFUN
      IF ( BUS5 .NE. NAMEA6(J) )
     1 GO TO 1834
      LSTAT(12) = J
      LSTAT(14) = NUMARG(J)
      IF ( IPRSUP .GT. 0 )
     1 WRITE (LUNIT6, *)  ' INIT69 finds function.  J, BUS5 =',  J, BUS5
      GO TO 9800
 1834 CONTINUE
      WRITE (JUNIT6, 6952)  BUS5
 6952 FORMAT ( ' Undefined user-supplied function named  "',
     1         A6,  '".   Halt in  DEVT69.' )
      GO TO 3481
 9800 RETURN
      END
