C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
      SUBROUTINE USER96
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      INCLUDE  'blkcom.ins'
      INCLUDE  'sixty4.ins'
      INCLUDE  'labl03.ins'
      INCLUDE  'deksea.ins'
      EQUIVALENCE ( MONCAS(199), ICHAR )
      EQUIVALENCE ( MONCAS(218), NEWH96 )
      COMMON  / C0B069 /   BUS   (  167 )
      CHARACTER*6  BUS
      COMMON  / C0B051 /   C     (    1 )
      COMMON  / C0B047 /   CCHAR (    1 )
      COMMON  / C0B035 /   CURR  (    1 )
      COMMON  / C0B102 /   E     (    1 )
      COMMON  / C0B103 /   F     (    2 )
      COMMON  / C0B075 /   FINIT (    1 )
      COMMON  / C0B049 /   GSLOPE(    1 )
      COMMON  / C0B041 /   ILAST (    1 )
      COMMON  / C0B042 /   NLTYPE(    1 )
      COMMON  / C0B032 /   NONLAD(    1 )
      COMMON  / C0B033 /   NONLE (    1 )
      COMMON  / C0B044 /   NONLK (    1 )
      COMMON  / C0B045 /   NONLM (    1 )
      COMMON  / C0B052 /   TR    (    1 )
      COMMON  / C0B053 /   TX    (    1 )
      COMMON  / C0B048 /   VCHAR (    1 )
      COMMON  / C0B037 /   VECNL1(    2 )
      COMMON  / C0B034 /   VNONL (    1 )
      EQUIVALENCE ( LSTAT(15), I )
      EQUIVALENCE ( LSTAT(16), K )
      EQUIVALENCE ( LSTAT(17), M )
      EQUIVALENCE ( LSTAT(18), MORE96 )
      EQUIVALENCE ( FLSTAT(18), VSL )
      EQUIVALENCE ( FLSTAT(17), A )
      EQUIVALENCE ( FLSTAT(19), GUS1 )
      EQUIVALENCE ( FLSTAT(20), GUS2 )
      DIMENSION  LOCH96(13)
      DIMENSION INDEXH(42)
      CHARACTER*6   NAME6(8)
      SAVE  LOCH96, INDEXH, NAME6, NEXTRA, NEW, LASTI
      DATA  LIMH96  / 13 /
      DATA  LASTI  / 99999 /
      DATA  NEW   / 1 /
      DATA  NAME6   / 'SINE  ',  'COSINE',  'STEP  ',  'TIME  ',
     1                '      ',  '      ',  '      ',  '      '  /
      DATA  NUMENU / 4 /
      ABSZ(XS)     = DABS(XS)
      N8 = ISTEP
      IF ( IPRSUP .GE. 3 )
     1 WRITE (LUNIT6, 2307)  N8, I, K, M, VSL
 2307 FORMAT ( ' Begin  USER96.   ISTEP     I     K     M           VSL'
     1         ,/,  17X, 4I6, 1P, E14.5 )
      N6 = NONLAD(I)
      IF ( NEWH96 .LE. 0 )
     1 GO TO 5723
      IF ( I .LE. LASTI )
     1 NEW = 1
      IF ( I .NE. LOCH96(NEW) ) THEN
      WRITE (46, 7305) I
 7305 FORMAT (     ' ----------------------------------',
     1             '-----------------------------------'   ,/,
     2         ' USER96 documents variables of row I =',  I3,
     1            '  of List-9.  Old hysteresis.' )
      GO TO 5723
          ENDIF
      IF ( IPRSUP .LT. 3 )
     1 GO TO 2853
      IF ( N8 .LE. 0 )
     1 WRITE (46, *)  ' '
      WRITE (ANSI8, 2802) N8
 2802 FORMAT ( I8 )
      L = 7
 2803 IF ( ANSI8(L:L) .NE. ' ' ) THEN
      L = L - 1
      GO TO 2803
          ENDIF
      WRITE (46, 2805)  I, ANSI8(L:8)
 2805 FORMAT (     ' -----------------------------------',
     1             '------------------------------------'   ,/,
     2         ' USER96 documents variables of row I =',  I3,
     1            '  of List-9 at time step N8 =', A  )
      WRITE (46, 2807)  NEW
 2807 FORMAT ( 8X, 'This is "New hysteresis" element # NEW =', I4 )
      WRITE (46, 2809)  VSL
 2809 FORMAT ( 8X, 'Element voltage = VSL =',  1P, E15.7 )
      WRITE (46, 2814)  N6
 2814 FORMAT ( 8X, 'Index to List-10 storage = N6 = NONLAD(I) =',  I4 )
      WRITE (46, 2818)  GSLOPE(N6)
 2818 FORMAT ( 8X, 'Norton current = GSLOPE(N6)  =',  1P, E15.7 )
      WRITE (46, 2823)  GSLOPE(N6+1)
 2823 FORMAT ( 8X, 'Norton admittance = GSLOPE(N6+1) =',  1P, E15.7 )
      WRITE (46, 2861)
 2861 FORMAT (  8X,  '  Those were inputs.  Upon exit,  outputs are :' )
 2853 NEW = NEW + 1
      LASTI = I
 5723 N7 = CCHAR(N6)
      N9 = ILAST(I)
      N10 = N9 + N7 - 1
      N12 = N6 + 2
      VNONL(I) = VNONL(I) + VSL * DELTA2
      CCHAR(N6+3) = VSL * GSLOPE(N6+1) + GSLOPE(N6)
      GUS2 = CCHAR(N6+3)
      IF ( CCHAR(N6+4) .LT. 0  .AND.
     1     VSL .GT. FLZERO )
     2 CCHAR(N6+4) = 0
      IF ( IPRSUP .GE. 2 )
     1 WRITE (LUNIT6, 4317)  I, N6, N7, N9,  VNONL(I),
     2                       GSLOPE(N6),  GSLOPE(N6+1)
 4317 FORMAT (  24H     I    N6    N7    N9,   7X, 8HVNONL(I),
     1          5X, 10HGSLOPE(N6),  3X, 12HGSLOPE(N6+1)
     2          ,/,  4I6,  1P, 3E15.7  )
      IF ( VNONL(I)+FLZERO  .LT.  VCHAR(N6+2)     .AND.
     1   CCHAR(N6+1)  .EQ.  1 )   GO TO 1100
      IF ( VNONL(I)-FLZERO  .GT.  VCHAR(N6+2)     .AND.
     1     CCHAR(N6+1)  .EQ.  -1 )   GO TO 1100
      IF ( ABSZ ( VNONL(I) )  .LT.  VCHAR(N10) )   GO TO 1116
      VCHAR(N6) = 0.0
      VCHAR(N6+1) = 0.0
 1116 D7 = VCHAR(N6) * VNONL(I) + VCHAR(N6+1)
      IF ( D7 .GE. 0.0 ) GO TO 1118
      D7 = 0.0
      VCHAR(N6) = 0.0
      VCHAR(N6+1) = 0.0
 1118 IF ( CCHAR(N6+1) .EQ. 1 ) GO TO 1110
*                                   WE ARE ON A DOWNER WITH NO REVERSAL.
      IF ( CCHAR(N6+5) .EQ. 1 ) GO TO 1127
*                           CALCULATE THE DISTANCE BETWEEN THE UPPER AND
*            LOWER HALF OF THE MAJOR LOOP AT THE PRESENT OPERATING POINT
      D8 = VNONL(I) + D7
      DO 1123  N11=N9, N10
      IF ( CCHAR(N6+3) .GT. CCHAR(N11) ) GO TO 1123
      CCHAR(N12) = N11
      GO TO 1125
 1123 CONTINUE
      CCHAR(N12) = N10 + 1
 1125 N13 = CCHAR(N12) + N7 + 1
      D9 = VCHAR(N13) * CCHAR(N6+3) + CCHAR(N13)
      D10 = D8 - D9
*               COMPARE AVAILABLE DISTANCE AND NORMAL OPERATION DISTANCE
      IF ( D7 .LE. (D10+FLZERO) ) GO TO 1119
      CCHAR(N6+5) = 1
 1127 DO 1130  N11=N9, N10
      IF ( VNONL(I) .GT. VCHAR(N11) ) GO TO 1130
      CCHAR(N12) = N11
      GO TO 1212
 1130 CONTINUE
      CCHAR(N12) = N10 + 1
      GO TO 1212
 1119 DO 1120  N11=N9, N10
      IF ( -D8 .GT. VCHAR(N11) ) GO TO 1120
      CCHAR(N12) = N11
      GO TO 1212
 1120 CONTINUE
      CCHAR(N12) = N10 + 1
      GO TO 1212
*                                   WE ARE ON AN UPPER WITH NO REVERSAL.
 1110 IF ( CCHAR(N6+5) .EQ. 1 ) GO TO 1147
*                           CALCULATE THE DISTANCE BETWEEN THE UPPER AND
*            LOWER HALF OF THE MAJOR LOOP AT THE PRESENT OPERATING POINT
      D8 = VNONL(I) - D7
      DO 1140  N11=N9, N10
      IF ( -CCHAR(N6+3) .GT. CCHAR(N11) ) GO TO 1140
      CCHAR(N12) = N11
      GO TO 1141
 1140 CONTINUE
      CCHAR(N12) = N10 + 1
 1141 N13 = CCHAR(N12) + N7 + 1
      D9 = VCHAR(N13) * CCHAR(N6+3) - CCHAR(N13)
      D10 = D9 - D8
*               COMPARE AVAILABLE DISTANCE AND NORMAL OPERATION DISTANCE
      IF ( D7 .LE. (D10+FLZERO) ) GO TO 1149
      CCHAR(N6+5) = 1
 1147 DO 1148  N11=N9, N10
      IF ( -VNONL(I) .GT. VCHAR(N11) ) GO TO 1148
      CCHAR(N12) = N11
      GO TO 1312
 1148 CONTINUE
      CCHAR(N12) = N10 + 1
      GO TO 1312
 1149 DO 1150  N11=N9, N10
      IF ( D8 .GT. VCHAR(N11) ) GO TO 1150
      CCHAR(N12) = N11
      GO TO 1312
 1150 CONTINUE
      CCHAR(N12) = N10 + 1
      GO TO 1312
*      THE PREVIOUS POINT WAS A REVERSAL POINT ON HYSTERESIS TRAJECTORY.
 1100 CCHAR(N6+1) = -CCHAR(N6+1)
      CCHAR(N6+4) =  CCHAR(N6+4) + 1
      CCHAR(N6+5) = 0.0
      IF ( CCHAR(N6+1) .NE. 1 ) GO TO 1195
*                     WE NOW SWITCH TO AN UPPER ( THERE WAS A REVERSAL).
      IF ( VCHAR(N6+2) .GT. -VCHAR(N10) ) GO TO 1580
      VCHAR(N6+5)  = VCHAR(N10)
      GSLOPE(N6+5) = CCHAR(N10)
      VCHAR(N6)   = 0.0
      VCHAR(N6+1) = 0.0
      VCHAR(N6+4) = VCHAR(N9)
      GSLOPE(N6+4) = CCHAR(N9)
      CCHAR(N12) = N9
      GO TO 1312
 1580 IF ( CCHAR(N6+4) .GT. 1 ) GO TO 1581
      VCHAR(N6+4)  = VCHAR(N10)
      GSLOPE(N6+4) = CCHAR(N10)
      D6 = 0.0
      GO TO 1575
 1581 DO 1701  N11=N9, N10
      IF ( GSLOPE(N6+4) .GT. CCHAR(N11) ) GO TO 1701
      CCHAR(N12) = N11
      GO TO 1710
 1701 CONTINUE
      CCHAR(N12) = N10 + 1
 1710 N13 = CCHAR(N12) + N7 + 1
      D9 = VCHAR(N13) * GSLOPE(N6+4) + CCHAR(N13)
      D6 = VCHAR(N6+4) - D9
 1575 DO 1750  N11=N9, N10
      IF ( VCHAR(N6+3) .GT. CCHAR(N11) ) GO TO 1750
      CCHAR(N12) = N11
      GO TO 1760
 1750 CONTINUE
      CCHAR(N12) = N10 + 1
 1760 N13 = CCHAR(N12) + N7 + 1
      D10 = VCHAR(N13) * VCHAR(N6+3) + CCHAR(N13)
      D11 = VCHAR(N6+2) - D10
      DO 1800  N11=N9, N10
      IF ( GSLOPE(N6+5) .GT. CCHAR(N11) ) GO TO 1800
      N14 = N11
      GO TO 1810
 1800 CONTINUE
      N14 = N10 + 1
 1810 N14 = N14 + N7 + 1
      D10 = VCHAR(N14) * GSLOPE(N6+5) + CCHAR(N14)
      D13 = VCHAR(N6+5) - D10
      IF ( VCHAR(N6+2) .GE. VCHAR(N6+5) ) GO TO 1675
      D14 = 0.0
      IF ( ABSZ ( D13 ) .GT. FLZERO )
     1 D14 = D11 * D6 / D13
      D9 = D9 + D14
      GO TO 1550
*                    WE NOW SWITCH TO A DOWNER ( THERE WAS A REVERSAL ).
 1195 IF ( VCHAR(N6+2) .LT. VCHAR(N10) ) GO TO 1590
      VCHAR(N6+5)  = VCHAR(N9)
      GSLOPE(N6+5) = CCHAR(N9)
      VCHAR(N6)   = 0.0
      VCHAR(N6+1) = 0.0
      VCHAR(N6+4)  = VCHAR(N10)
      GSLOPE(N6+4) = CCHAR(N10)
      CCHAR(N12) = N9
      GO TO 1212
 1590 IF ( CCHAR(N6+4) .GT. 1 ) GO TO 1591
      VCHAR(N6+4)  = VCHAR(N9)
      GSLOPE(N6+4) = CCHAR(N9)
      D6 = 0.0
      GO TO 1515
 1591 DO 1600  N11=N9, N10
      IF ( -GSLOPE(N6+4) .GT. CCHAR(N11) ) GO TO 1600
      CCHAR(N12) = N11
      GO TO 1610
 1600 CONTINUE
      CCHAR(N12) = N10 + 1
 1610 N13 = CCHAR(N12) + N7 + 1
      D9 = VCHAR(N13) * GSLOPE(N6+4) - CCHAR(N13)
      D6 = D9 - VCHAR(N6+4)
 1515 DO 1650  N11=N9, N10
      IF ( -VCHAR(N6+3) .GT. CCHAR(N11) ) GO TO 1650
      CCHAR(N12) = N11
      GO TO 1660
 1650 CONTINUE
      CCHAR(N12) = N10 + 1
 1660 N13 = CCHAR(N12) + N7 + 1
      D10 = VCHAR(N13) * VCHAR(N6+3) - CCHAR(N13)
      D11 = D10 - VCHAR(N6+2)
      DO 1850  N11=N9, N10
      IF ( -GSLOPE(N6+5) .GT. CCHAR(N11) ) GO TO 1850
      N14 = N11
      GO TO 1860
 1850 CONTINUE
      N14 = N10 + 1
 1860 N14 = N14 + N7 + 1
      D10 = VCHAR(N14) * GSLOPE(N6+5) - CCHAR(N14)
      D13 = D10 - VCHAR(N6+5)
      IF ( VCHAR(N6+2) .GT. VCHAR(N6+5) ) GO TO 1680
 1675 VCHAR(N6+5)  = VCHAR(N6+4)
      GSLOPE(N6+5) = GSLOPE(N6+4)
      D13 = D6
      GO TO 1690
 1680 D14 = 0.0
      IF ( ABSZ ( D13 ) .GT. FLZERO )
     1 D14 = D11 * D6 / D13
      D9 = D9 - D14
 1550 VCHAR(N6+5)  = D9
      GSLOPE(N6+5) = GSLOPE(N6+4)
      D13 = D14
 1690 VCHAR(N6+4)  = VCHAR(N6+2)
      GSLOPE(N6+4) = VCHAR(N6+3)
      D12 =D11
      IF ( VCHAR(N6+4) .LT. (VCHAR(N6+5)-FLZERO)) GO TO 1700
      IF ( VCHAR(N6+4) .GT. (VCHAR(N6+5)+FLZERO)) GO TO 1700
      IF ( CCHAR(N6+1) .EQ. 1 ) GO TO 1720
      VCHAR(N6+5)  = VCHAR(N9)
      GSLOPE(N6+5) = CCHAR(N9)
      D13 = 0.0
      GO TO 1700
 1720 VCHAR(N6+5)  = VCHAR(N10)
      GSLOPE(N6+5) = CCHAR(N10)
      D13 = 0.0
 1700 D15 = D12 * ( VCHAR(N10) - VCHAR(N6+5) ) /
     1 ( VCHAR(N10) - VCHAR(N6+4) )
      IF (CCHAR(N6+1) .EQ. -1. )
     1 D15 = D12 * ( -VCHAR(N10) - VCHAR(N6+5) ) /
     2 ( -VCHAR(N10) - VCHAR(N6+4) )
      IF ( D13 .LE. D15 )  GO TO 1705
      D13 = D15
      IF ( IPRSUP  .GE.  1 )
     1 WRITE (LUNIT6, 1706)  BUS(K), BUS(M), T
 1706 FORMAT ( /,  5X,  'NOTE ---- For the Type-96 hysteretic inductor',
     1  ' that connects bus  "',  A6,  8H"  to  ",  A6,  '",   a',
     2  ' trajectory was'  ,/, 15X, 'initially created that would have',
     3  ' caused operation outside the major hysteresis loop.  The'  ,/,
     4  15X,  'trajectory has been modified to prevent this.  Accuracy',
     5  ' of the results should be unaffected.'  ,/,  5H  T =,  E15.6  )
      IF ( IPRSUP  .GE.  1 )
     1 WRITE (LUNIT6, 7706)  I, D13, D15
 7706 FORMAT (    15X,  32H ROW NUMBER OF  N.L.  ELEMENT IS,
     1             I4,  26H .   VARIABLES  D13, D15 =, 2E13.4 )
 1705 VCHAR(N6) = (D12 - D13) / (VCHAR(N6+4)-VCHAR(N6+5))
      VCHAR(N6+1) = D12 - VCHAR(N6) * VCHAR(N6+4)
      IF ( CCHAR(N6+1) .EQ. 1 ) GO TO 1312
 1212 N14 = CCHAR(N12)
      N13 = N14 + N7 + 1
      IF (CCHAR(N6+5) .EQ. 1 ) GO TO 1213
      D13 = GSLOPE(N14) * (1.0 + VCHAR(N6) )
      D13 = 1.0 / D13
      D14 = GSLOPE(N13) -  GSLOPE(N14) * VCHAR(N6+1)
      D14 = D14 * D13
      GO TO 1315
 1213 D13 = VCHAR(N13)
      D14 = CCHAR(N13)
      GO TO 1315
 1312 N14 = CCHAR(N12)
      N13 = N14 + N7 + 1
      IF ( CCHAR(N6+5) .EQ. 1 ) GO TO 1313
      D13 = GSLOPE(N14) * ( 1.0 - VCHAR(N6) )
      D13 = 1.0 / D13
      D14 = GSLOPE(N14) * VCHAR(N6+1) - GSLOPE(N13)
      D14 = D14 * D13
      GO TO 1315
 1313 D13 = VCHAR(N13)
      D14 = -CCHAR(N13)
 1315 IF ( GSLOPE(N12) .EQ. CCHAR(N6+1)  .AND.
     1     GSLOPE(N6+3) .EQ. CCHAR(N12) ) GO TO 1319
      D15 = DELTA2 / D13
      A = D15 - GSLOPE(N6+1)
      GSLOPE(N6+1) = D15
      GO TO 1322
 1319 A = 0.0
 1322 D16 = VNONL(I) - D14 + DELTA2 * VSL
      D16 = D16 / D13
      GUS1 = D16 - GSLOPE(N6)
      GSLOPE(N6) = D16
      GSLOPE(N12) = CCHAR(N6+1)
      GSLOPE(N6+3) = CCHAR(N12)
      VCHAR(N6+3) = CCHAR(N6+3)
      VCHAR(N12) = VNONL(I)
      VNONL(I) = VNONL(I) + VSL * DELTA2
      IF ( NEWH96 .GT. 0  .AND.
     1     IPRSUP .GE. 3 ) THEN
      WRITE (46, 2864)  A
 2864 FORMAT (  8X,  'Change in Norton admittance = A =',  1P,  E15.7  )
      WRITE (46, 2867)  GUS1
 2867 FORMAT (  8X,  'Change in Norton current = GUS1 =',  1P,  E15.7  )
      WRITE (46, *) ' '
          ENDIF
      IF ( NUMSEA .LE. 0 ) GO TO 5195
      DO 7382  J=1, NEXTRA
      IF ( INDEXH(J) .EQ. 1 ) THEN
      BVCHEN(J) = SINZ ( T )
          ELSEIF ( INDEXH(J) .EQ. 2 ) THEN
      BVCHEN(J) = COSZ ( T )
          ELSEIF ( INDEXH(J) .EQ. 3 ) THEN
      BVCHEN(J) = ISTEP
          ELSEIF ( INDEXH(J) .EQ. 4 ) THEN
      BVCHEN(J) = T
          ENDIF
 7382 CONTINUE
 5195 IF ( IPRSUP .GE. 2 )
     1 WRITE (LUNIT6, 4372)  A, GUS1, GUS2,  D14,
     2            ( IP, CCHAR(IP), VCHAR(IP), GSLOPE(IP),  IP=N6, N6+5 )
 4372 FORMAT ( ' Exit  USER96.',  14X, 1HA,  11X, 4HGUS1,  11X, 4HGUS2,
     1         12X, 3HD14  ,/,  1P,  14X,  4E15.7  ,/,
     2         17X,  3HRow,  10X, 5HCCHAR,  10X, 5HVCHAR,  9X, 6HGSLOPE
     3                                   ,/,    ( 1X,  I19,  3E15.7 )  )
      GO TO 9800
      ENTRY DATA96
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 2893)  INONL, NEWH96, MORE96, ICHAR, ABUFF
 2893 FORMAT ( ' Begin  DATA96.  INONL, NEWH96, MORE96, ICHAR =',  4I8
     1    ,/,  '                 ABUFF =',  A80  )
      IF ( MORE96 .NE. -7799 )
     1 GO TO 3841
      NLTYPE(INONL) = -96
      NONLAD(INONL) = ICHAR + 1
      ILAST(INONL) = NONLAD(INONL) + 6
      NUM99 = NUM99 + 1
      VECNL1(INONL) = C(IT)
      IF ( N3*N4 .NE. 1 ) THEN
      DO 1748  J=1, NEWH96
      L = LOCH96(J)
      IF ( N3 .NE. NONLK(L)  .OR.
     1     N4 .NE. IABS ( NONLM(L) ) )
     2 GO TO 1748
      NEWH96 = NEWH96 + 1
      LOCH96(NEWH96) = INONL
      GO TO 9800
 1748 CONTINUE
      GO TO 9800
          ENDIF
      WRITE (JUNIT6, 4738)  TR(IT), TX(IT), C(IT),  CARDIN
 4738 FORMAT ( 12HHysteresis. ,  1P,  3E12.4, '  |',  A80 )
      CALL OUTSIX ( JUNIT6, 131 )
      N7 = INDEX ( CARDIN , 'New hysteresis' )
      IF ( N7 .LE. 0 )
     1 GO TO 6859
      IF ( NEWH96 .EQ. 0 ) THEN
      NEXTRA = 0
      IF ( NUMSEA .EQ. -1 ) GO TO 1940
      JUNIT6 = ' Simultaneous use of  "New hysteresis"  and  USE'  //
     1         ' SEATTLE XFORMER  are not allowed.  Halt.'
      CALL OUTSIX ( JUNIT6, 131 )
      CALL STOPTP
          ENDIF
      IF ( NEWH96 .GE. LIMH96 ) THEN
      WRITE (JUNIT6, 3945)  LIMH96
 3945 FORMAT ( ' Overflow storage limit  LIMH96 =',  I4,  '  within',
     1         ' user-supplied Type-96 hysteresis.  Halt.'  )
      CALL OUTSIX ( JUNIT6, 131 )
      CALL STOPTP
          ENDIF
 1940 NEWH96 = NEWH96 + 1
      LOCH96(NEWH96) = INONL
 7951 CALL CIMAGE
      IF ( ABUFF(1:6) .NE. 'OUTPUT' )
     1 GO TO 5296
      WRITE (JUNIT6, 2912)  CARDIN
 2912 FORMAT ( 'Special hysteresis variables for output vector.   |',
     1         A80 )
      CALL OUTSIX ( JUNIT6, 131 )
      L = 7
 4853 IF ( ABUFF(L:L) .NE. ' '  )
     1 GO TO 3284
      IF ( L .GE. 80 )
     1 GO TO 7951
 1071 L = L + 1
      GO TO 4853
 3284 K = L
 6928 IF ( L .GE. 80 )
     1 GO TO 5743
      L = L + 1
      IF ( ABUFF(L:L) .EQ. ' ' ) THEN
      L = L - 1
      GO TO 5743
          ENDIF
      GO TO 6928
 5743 N8 = L - K + 1
      DO 1372  J=1, NUMENU
      IF ( ABUFF(K:L) .EQ. NAME6(J)(1:N8) )
     1 GO TO 6397
 1372 CONTINUE
      WRITE (JUNIT6, 4325) ABUFF(K:L)
 4325 FORMAT ( '   ---  Ignore the request  "',  A,  '"  of the',
     1         '  preceding  OUTPUT  card.  The name is unrecognized.' )
      CALL OUTSIX ( JUNIT6, 131 )
 7108 ABUFF(K:L) = BLAN80(K:L)
      GO TO 1071
 6397 NEXTRA = NEXTRA + 1
      INDEXH(NEXTRA) = J
      WRITE (BUS4, 3482)  NEWH96
 3482 FORMAT ( 'HYST', I2 )
      JCHEN6(NEXTRA) = BUS4
      KCHEN6(NEXTRA) = NAME6(J)
      GO TO 7108
 5296 IF ( ABUFF(1:4) .EQ. 'MENU' ) THEN
      CALL STOPTP
          ENDIF
      JUNIT6 = 'Model of user will read more data from this file  |'
     1         //  CARDIN
      CALL OUTSIX ( JUNIT6, 131 )
      CCHAR(ICHAR+6) = 0.0
      ICHAR = ICHAR + 7
      CCHAR(ICHAR+1) = 1.0D0 * TR(IT)
      VCHAR(ICHAR+1) =-1.4D0 * TX(IT)
      CCHAR(ICHAR+2) = 2.0D0 * TR(IT)
      VCHAR(ICHAR+2) = 1.8D0 * TX(IT)
      CCHAR(ICHAR+3) = 3.5D0 * TR(IT)
      VCHAR(ICHAR+3) = 2.0D0 * TX(IT)
      ICHAR = ICHAR + 3
      NUMSEA = NEXTRA
      GO TO 9800
 6859 MORE96 = 1
      GO TO 9800
 3841 IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 7482)  CARDIN
 7482 FORMAT ( ' ENTRY DATA96 simply watches characteristic.',
     1         '  CARDIN =',  A80 )
      GO TO 9800
      ENTRY INIT96
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 3924)  NEWH96, LASTI, NEW
 3924 FORMAT ( ' Begin INIT96.  NEWH96, LASTI, NEW =', 3I8  )
      IF ( NEWH96 .LE. 0 )
     1 GO TO 9800
      IF ( I .LE. LASTI )
     1 NEW = 1
      IF ( I .NE. LOCH96(NEW) )
     1 GO TO 9800
      IF ( IPRSUP .LT. 3 )
     1 GO TO 9800
      WRITE (46, 5401)  I, K, M, E(K), F(K), E(M), F(M)
 5401 FORMAT ( /,  ' ---------------------------------------',
     1             '---------------------------------------'   ,/,
     2  ' INIT96.    I    K    M',
     1         10X, 4HE(K), 10X, 4HF(K),  10X, 4HE(M), 10X, 4HF(M)
     2    ,/,  8X, 3I5,  1P, 4E14.6 )
      WRITE (46, 5403)  I
 5403 FORMAT ( /, ' INIT96 documents variables of row I =',  I3,
     1            '  of the List-9 N.L. element table:'  )
      WRITE (46, 5405)  BUS(K), BUS(M)
 5405 FORMAT ( 8X, 'The A6 "from" bus name = BUS(K) =  "', A6,  1H"
     1    ,/,  8X, 'The A6  "to"  bus name = BUS(M) =  "', A6,  1H"  )
      WRITE (46, 5408)  OMEGA
 5408 FORMAT ( 8X, 'Radian frequency of phasors = OMEGA =',  1P, E15.7 )
      WRITE (46, 5413)  CURR(I)
 5413 FORMAT ( 8X, 'Phasor solution current = CURR(I) =',  1P, E15.7 )
      WRITE (46, 5417)  VECNL1(I)
 5417 FORMAT (8X, 'Residual flux of cols. 39-44 = RESID = VECNL1(I) ='
     1        , 1P, E15.7 )
      CI1 = ( F(K) - F(M) ) / OMEGA
      WRITE (46, 5422)  CI1
 5422 FORMAT ( 8X, 'Phasor solution flux = ( F(K) - F(M) ) / OMEGA =',
     1         1P, E15.7 )
      D6 = E(K) - E(M)
      WRITE (46, 5425)  D6
 5425 FORMAT (8X, 'Phasor solution voltage = E(K) - E(M) =', 1P, E15.7 )
      WRITE (46, 5478)
 5478 FORMAT ( '        ---------------------------------',
     1         '---------------------' ,/  )
      NEW = NEW + 1
      LASTI = I
 9800 RETURN
      END
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
C USER96 TO USER96.F BY HEVIA
