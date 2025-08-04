C USERNL TO USERNL,F BY HEVIA
C USERNL TO USERNL,F BY HEVIA
C USERNL TO USERNL,F BY HEVIA
C USERNL TO USERNL,F BY HEVIA
C USERNL TO USERNL,F BY HEVIA
      SUBROUTINE USERNL
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      COMMON  / C0B036 /   ANONL (    2 )
      COMMON  / C0B069 /   BUS   (  167 )
      CHARACTER*6  BUS
      COMMON  / C0B047 /   CCHAR (    1 )
      COMMON  / C0B035 /   CURR  (    1 )
      COMMON  / C0B046 /   CURSUB(    1 )
      COMMON  / C0B102 /   E     (    1 )
      COMMON  / C0B103 /   F     (    2 )
      COMMON  / C0B049 /   GSLOPE(    1 )
      COMMON  / C0B041 /   ILAST (    1 )
      COMMON  / C0B073 /   KKNONL(    2 )
      COMMON  / C0B043 /   NLSUB (    1 )
      COMMON  / C0B042 /   NLTYPE(    1 )
      COMMON  / C0B032 /   NONLAD(    1 )
      COMMON  / C0B068 /   TEXVEC(   45 )
      CHARACTER*6  TEXVEC
      COMMON  / C0B048 /   VCHAR (    1 )
      COMMON  / C0B038 /   VECNL2(    2 )
      COMMON  / C0B037 /   VECNL1(    2 )
      COMMON  / C0B034 /   VNONL (    1 )
      COMMON  / C0B109 /   VOLT  (    6 )
      COMMON  / C0B107 /   VOLTI (  400 )
      COMMON  / C0B108 /   VOLTK (    7 )
      COMMON  / C0B040 /   VZERO (    2 )
      COMMON  / C0B074 /   ZNONL (    1 )
      INCLUDE  'dekspy.ins'
      INCLUDE  'sixty4.ins'
      DIMENSION        NSUBKM(1)
      EQUIVALENCE     ( KKNONL(1), NSUBKM(1) )
      EQUIVALENCE  ( PEAKND(27),  STATFR )
      EQUIVALENCE  ( PEAKND(37),  SQRT3  ),   ( PEAKND(47),  ONETRD )
      EQUIVALENCE  ( PEAKND(77),  XOPT   )
      EQUIVALENCE  ( PEAKND(80),  XUNITS )
      EQUIVALENCE     ( MONCAS(246),  LL3   ),   ( MONCAS(286),  LL6   )
      EQUIVALENCE     ( MONCAS(287),  LL2   ),   ( MONCAS(289),  LL4   )
      EQUIVALENCE     ( MONCAS(290),  LL5   ),   ( MONCAS(291),  LL7   )
      EQUIVALENCE     ( MONCAS(292),  LL8   ),   ( MONCAS(293),  LL9   )
      COMPLEX*16  CA1, CA2,  CZ0, CZ1, CZ2,  CRES
      COMPLEX*16  CMPLXZ
      EQUIVALENCE  ( LIMARR(1),  N7 )
      EQUIVALENCE  ( LIMARR(2),  I )
      EQUIVALENCE  ( LIMARR(3),  M5 )
      DIMENSION  RR(1),  XX(1),  FAC(1),  SUM(1)
      EQUIVALENCE     ( SUM(1),        CCHAR(1)  )
      EQUIVALENCE     ( XX(1),         VCHAR(1)  )
      EQUIVALENCE     ( RR(1), FAC(1), GSLOPE(1) )
      IF ( NCHAIN .NE. 3 )  GO TO 6237
      KOVR16 = JBEGOV(16) - 1
      KNTMAX = LSTAT(14)
      J = INDEX ( CARDIN, 'FILE=' )
      IF ( J .GT. 0 ) THEN
      BUFF77 = CARDIN(J+5:80)
      J = INDEX ( BUFF77, '}' )
      J = J - 1
      IF ( BUFF77(J:J) .EQ. ' ' )
     1 CALL BLKEND ( BUFF77, J )
      OPEN ( UNIT=52, FILE=BUFF77(1:J), STATUS='OLD' )
          ENDIF
      N8 = KNTMAX * KNTMAX
      N3 = INONL - KNTMAX + 1
      NDX2 = NONLAD(N3) - 1
      N22 = N8 + NDX2
      IF ( KNTMAX .LE. 3 ) THEN
      KNTEND = KNTMAX - 1
          ELSE
      KNTEND = 2
          ENDIF
      DO 7522  IK=1, KNTMAX
      DO 6251  IP=1, 2
      N3 = IK + NDX2
      N4 = 6 * KNTMAX + NDX2
      IF ( KNTMAX .LT. 6 )
     1 N4 = N22
      DO 4928  JK=1, KNTMAX, 6
      IF ( J .LE. 0 ) THEN
      CALL CIMAGE
          ELSE
 3772 READ (52, 1982) ABUFF
 1982 FORMAT ( A80 )
      IF ( ABUFF(1:2) .EQ. 'C ' )  GO TO 3772
      CARDIN = ABUFF
          ENDIF
      LSTAT(13) = IK
      IF ( IP .EQ. 1 ) THEN
      READ (ABUFF, 3417) ( RR(M), M=N3, N4, KNTMAX )
 3417 FORMAT ( 6E12.0 )
      WRITE (JUNIT6, 1953)  ( RR(N3+M*KNTMAX), M=0, KNTEND )
 1953 FORMAT (  1P,  3E12.4  )
      INDPRT = KENFNT(KOVR16+6) + 6
          ELSE
      READ (ABUFF, 3417) ( XX(M), M=N3, N4, KNTMAX )
      DO 5374  M=N3, N4, KNTMAX
 5374 XX(M) = XX(M) * XUNITS
      INDPRT = KENFNT(KOVR16+6) + 7
      WRITE (JUNIT6, 1953)  ( XX(N3+M*KNTMAX), M=0, KNTEND )
          ENDIF
      CALL IFLUSH
      IF ( JK .GE. KNTMAX - 5 )
     1 GO TO 4928
      N3 = N3 + 6 * KNTMAX
      N4 = N4 + 6 * KNTMAX
      IF ( N4 .GT. N8 )
     1 N4 = N22
 4928 CONTINUE
 6251 CONTINUE
 7522 CONTINUE
      GO TO 9800
 6237 J = ILAST(I)
      NDX2 = NONLAD(I) - 1
      IF ( IPRSUP .GE. 1 )
     1 WRITE (LUNIT6, 3085)   i, n7, J, texvec(J)
 3085 FORMAT ( ' USERNL considers next  I, N7, J =',  3I6  ,/,
     1         '        Model name  TEXVEC(J) =',  A6  )
      IF ( TEXVEC(J) .EQ. 'RTIME ' )
     1 CALL RTIME
      IF ( TEXVEC(J) .NE. 'Z0Z1Z2' .AND.
     1     TEXVEC(J) .NE. '[R][L]' )
     2 GO TO 9800
      IF ( ISTEP .GT. 1 ) GO TO 1837
      IF ( TEXVEC(J) .EQ. '[R][L]' )
     1 GO TO 2705
      IOF = NSUBKM(M5+3) - 1
      D3 = -ONEHAF
      D4 = SQRT3 * ONEHAF
      CA1 = CMPLXZ ( D3, D4 )
      D4 = -D4
      CA2 = CMPLXZ ( D3, D4 )
      IF ( XOPT .GT. 0.0 ) THEN
      D6 = XOPT
          ELSE
      D6 = STATFR
      INDPRT = KENFNT(KOVFNT+6) + 8
      FLSTAT(14) = STATFR
      CALL TFLUSH
          ENDIF
      D6 = D6 * TWOPI
      IF ( IPRSUP .GT. 0 )
     1 WRITE (LUNIT6, 1562) ( ANONL(IOF+J), J=1, 3 )
 1562 FORMAT ( ' USERNL begins with Lo, L1, L2 [H] =',  1P, 3E13.5 )
      D3 = D6 * ANONL(IOF+1)
      CZ0 = CMPLXZ ( VECNL1(IOF+1), D3 )
      D4 = D6 * ANONL(IOF+2)
      CZ1 = CMPLXZ ( VECNL1(IOF+2), D4 )
      D5 = D6 * ANONL(IOF+3)
      CZ2 = CMPLXZ ( VECNL1(IOF+3), D5 )
      IF ( IPRSUP .GT. 0 )
     1 WRITE (LUNIT6, 2834)  D3, D4, D5
 2834 FORMAT ( '           Converted to Xo, X1, X2 =',  1P, 3E13.5 )
      D4 = VECNL1(IOF+1) + VECNL1(IOF+2) + VECNL1(IOF+3)
      D5 = ANONL(IOF+1)  + ANONL(IOF+2)  + ANONL(IOF+3)
      D5 = D6 * D5
      RR(NDX2+1) = D4
      XX(NDX2+1) = D5
      RR(NDX2+5) = D4
      XX(NDX2+5) = D5
      RR(NDX2+9) = D4
      XX(NDX2+9) = D5
      CRES = CZ0 + CA1 * CZ1 + CA2 * CZ2
      D4 = REALZ  ( CRES )
      D5 = AIMAGZ ( CRES )
      RR(NDX2+4) = D4
      XX(NDX2+4) = D5
      RR(NDX2+8) = D4
      XX(NDX2+8) = D5
      RR(NDX2+3) = D4
      XX(NDX2+3) = D5
      CRES = CZ0 + CA2 * CZ1 + CA1 * CZ2
      D4 = REALZ  ( CRES )
      D5 = AIMAGZ ( CRES )
      RR(NDX2+7) = D4
      XX(NDX2+7) = D5
      RR(NDX2+2) = D4
      XX(NDX2+2) = D5
      RR(NDX2+6) = D4
      XX(NDX2+6) = D5
      DO 3043  K=NDX2+1, NDX2+9
      RR(K) = RR(K) * ONETRD
      XX(K) = XX(K) * ONETRD
 3043 CONTINUE
      WRITE (46, 7451)  D6
 7451 FORMAT ( ' 3x3 phase-domain impedance matrix in ohms follow.',
     1         '  For each row I,  X(I,J)  is below  R(I,J).  w =',
     2         1P,  E14.6,  ' rad/sec.' )
      WRITE (46, 2005)  RR(1),  RR(LL4),  RR(LL7)
      WRITE (46, 2005)  XX(1),  XX(LL4),  XX(LL7)
      WRITE (46, 2005)  RR(LL2), RR(LL5),  RR(LL8)
      WRITE (46, 2005)  XX(LL2),  XX(LL5),  XX(LL8)
      WRITE (46, 2005)  RR(LL3),  RR(LL6), RR(LL9)
      WRITE (46, 2005)  XX(LL3),  XX(LL6),  XX(LL9)
 2005 FORMAT ( 6F14.10 )
      IF ( NDX2 .GT. 0 )
     1 WRITE (46, *) '   Warning!  Previous output used bad subscripts.'
      DO 6832  K=NDX2+1, NDX2+9
      XX(K) = XX(K) / D6
 6832 CONTINUE
 2705 N7SQ = N7 * N7
      CALL MOVER0 ( VOLT(1), N7SQ )
      DO 4381  K=1, N7
      N3 = K + ( K - 1 ) * N7
 4381 VOLT(N3) = 1.0
      D4 = 1.0 / DELTA2
      DO 5843  K=1+NDX2, N7SQ+NDX2
      XX(K)  = XX(K) * D4
      SUM(K) = XX(K) - RR(K)
      FAC(K) = RR(K) - VOLTI(K) + XX(K)
 5843 CONTINUE
      CALL DGELG ( VOLT(1), FAC(1+NDX2), N7, N7, EPSILN, IER )
      IF ( IER .EQ. 0 ) GO TO 2693
      WRITE (46, *) ' Local error halt in USERNL/Z0Z1Z2.  IER =',  ier
      CALL STOPTP
 2693 CALL MOVER ( VOLT(1), FAC(1+NDX2), N7SQ )
 1837 CALL MOVER0 ( VOLT(1), N7 )
      N2 = 0
      DO 3692  K=1, N7
      N3 = 2 * K
      D6 = VOLTK(N3-1) + VOLTK(N3)
      N4 = K + NDX2
      N9 = NSUBKM(M5+3)
      DO 4738  M=1, N7
      D6 = D6 + SUM(N4) * CURR(N9)
      N9 = N9 + 1
      N4 = N4 + N7
 4738 CONTINUE
      DO 6239  M=1, N7
      N2 = N2 + 1
      VOLT(M) = VOLT(M) + FAC(N2) * D6
 6239 CONTINUE
 3692 CONTINUE
      N9 = NSUBKM(M5+3)
      DO 1237  K=1, N7
      CURR(N9) = VOLT(K)
      N9 = N9 + 1
      D6 = VOLTK(2*K-1)
      N4 = K
      DO 5732  M=1, N7
      D6 = D6 + VOLTI(N4) * VOLT(M)
 5732 N4 = N4 + N7
      VOLTK(2*K) = D6
 1237 CONTINUE
 9800 RETURN
      END
C USERNL TO USERNL.F BY HEVIA
C USERNL TO USERNL.F BY HEVIA
C USERNL TO USERNL.F BY HEVIA
C USERNL TO USERNL.F BY HEVIA
C USERNL TO USERNL.F BY HEVIA
