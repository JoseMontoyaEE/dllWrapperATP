      SUBROUTINE DIMDEF ( LSIZE, NCHAIN )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)                              
      DIMENSION  LSIZE(50)
      IF ( NCHAIN  .NE.   0 )   GO TO 2000
      LSIZE( 1)  =  100002
      LSIZE( 2)  =  120000
      LSIZE( 3)  =  500000
      LSIZE( 4)  =   20000
      LSIZE( 5)  = 2000000
      LSIZE( 6)  =   50000
      LSIZE( 7)  =  200000
      LSIZE( 8)  = 2000000
      LSIZE( 9)  =   10000
      LSIZE(10)  =   16000
      LSIZE(11)  =    3000
      LSIZE(12)  =   12000
      LSIZE(13)  = 2000000
      LSIZE(14)  =    2000
      LSIZE(15)  = 3000000
      LSIZE(16)  =    3200
      LSIZE(17)  =     360
      LSIZE(18)  =    1016
      LSIZE(19)  = 3000000
      LSIZE(20)  = 3000000
      LSIZE(21)  =   12000
      LSIZE(22)  = 1000000
      LSIZE(23)  = 3000000
      LSIZE(24)  =      36
      LSIZE(25)  =  240000
      LSIZE(26)  = 2000000
      LSIZE(27)  =  100000
      LSIZE(28)  = 3000000
      LSIZE(29)  =99420960
      LSIZE(30)  =     120
      LSIZE(31)  =  300002
      LSIZE(32)  =    1200
      N7 = 32 + 1
      LSIZE(N7)  =  99420960
      RETURN
 2000 IF ( NCHAIN  .NE.  20 )   GO TO 2100
      LSIZE( 1)  =      23
      LSIZE( 2)  = 3000000
      LSIZE( 3)  =       1
      LSIZE( 4)  =  100002
      LSIZE( 5)  =       0
      LSIZE( 6)  =30000000
      LSIZE( 7)  =      29
      LSIZE( 8)  =99420960
      RETURN
 2100 IF ( NCHAIN  .NE.  21 )   GO TO 2300
      LSIZE( 1)  =      13
      LSIZE( 2)  = 2000000
      RETURN
 2300 IF ( NCHAIN  .NE.  23 )   GO TO  900
      LSIZE( 1)  =      71
      LSIZE( 2)  =    2968
      RETURN
  900 IF ( NCHAIN  .NE.   9 )   GO TO 2500
      LSIZE( 1)  =       1
      LSIZE( 2)  =  100002
      LSIZE( 3)  =      23
      LSIZE( 4)  = 3000000
      LSIZE( 5)  =      52
      LSIZE( 6)  =   80000
      LSIZE( 7)  =       4
      LSIZE( 8)  =   20000
      RETURN
 2500 IF ( NCHAIN  .NE.  25 )   GO TO 2600
      LSIZE( 1)  =       9
      LSIZE( 2)  =   10000
      LSIZE( 3)  =      71
      LSIZE( 4)  =    1148
      LSIZE( 5)  =      73
      LSIZE( 6)  =    1149
      RETURN
 2600 IF ( NCHAIN  .NE.  26 )   GO TO 2700
      LSIZE( 1)  =       9
      LSIZE( 2)  =   10000
      RETURN
 2700 IF ( NCHAIN  .NE.  27 )   GO TO 2800
      LSIZE( 1)  =       9
      LSIZE( 2)  =   10000
      RETURN
 2800 IF ( NCHAIN  .NE.  28 )   GO TO 9900
      LSIZE( 1)  =      74
      LSIZE( 2)  =      15
      RETURN
 9900 CONTINUE
      RETURN
      END
