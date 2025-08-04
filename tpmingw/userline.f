C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
      SUBROUTINE USER3A
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)

      COMMON   /COM23/  CIK(20), CK(20), CI(20), LENGTH(20), NR(20)
      COMMON   /COM23/  VOLTI(20), VOLTK(20), XK(500), XM(500)
      COMMON   /COM23/  TR(20,20), QFD(210), ONEHAF, IPRSOV(3)
      COMMON   /COM23/  NPH, MN, LPAST, ISTEP, IPOINT
      CHARACTER*80  PROM80
      CHARACTER*10  DIGITS
      CHARACTER*52  ALPHAB
      CHARACTER*132  JUNIT6
      ALPHAB = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
      DIGITS = '1234567890'
      KOL132 = 132
      WRITE (46, *) ' Top of  SUBROUTINE USER3A  for  "User line"  use.'
      MOLDAT = 0
      NPH = 0
      IPHASE = 1
      ISTEP = -1
      IPOINT = 0
      DO 5285  IP=1, 9999
      READ (55, 1329, END=2309)  PROM80
 1329 FORMAT ( A80 )
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 4087)  IP, PROM80
 4087 FORMAT ( ' Next input card #',  I3,  2X,  A80 )
      IF ( PROM80(1:2) .EQ. 'C ' .OR.
     1     PROM80(1:2) .EQ. 'c ' )
     2 GO TO 5285
      IF ( PROM80(1:10) .NE. 'DIAGNOSTIC' )
     1 GO TO 6379
      READ (PROM80, 7297) IPRSOV
 7297 FORMAT ( BN, 24X, I2, 18X, I2, 6X, I2 )
      IPRSUP = IPRSOV(1)
      GO TO 5285
 6379 IF ( PROM80(1:8) .NE. '$VINTAGE' )
     1 GO TO 2315
      IF ( PROM80(1:11) .EQ. '$VINTAGE, 1' ) THEN
      MOLDAT = 1
          ELSE
      MOLDAT = 0
          ENDIF
      GO TO 5285
 2315 NPH = NPH + 1
      IF ( PROM80(1:1) .EQ. '-' )
     1 GO TO 5078
      IF ( NPH .GT. MAT )
     1 GO TO 7284
      IF ( NPH .GT. 20 ) THEN
      JUNIT6 = ' Error.  Too many phases.  The K.C. Lee branch cards'
     1     //  ' are limited to 20 phases.  Halt in USER3A.'
      GO TO 9200
          ENDIF
 5078 IF ( MOLDAT .NE. 1 ) THEN
      JUNIT6 = ' Error.  Data format is not wide.  Use of  $VINTAGE, 1'
     1     //  '  is required.  Halt in USER3A.'
      GO TO 9200
          ENDIF
      READ (PROM80, 3256)  ITYPE, H1, AA, H3, XLONG, ILINE,  MAT
 3256 FORMAT ( BN, I2, 24X, 4E12.0, I2, I3 )
      IF ( ITYPE .LT. 0 )
     1 ITYPE = -ITYPE
      IF ( ITYPE .NE. NPH )
     1 GO TO 7284
      IF ( XLONG .LT. 0.0 )
     1    XLONG = -XLONG
      N13 = 0
      IF ( PROM80(79:79) .EQ. ' ' .OR.
     1     PROM80(79:79) .EQ. '0' )
     2 GO TO 5942
      N13 = INDEX ( DIGITS, PROM80(79:79) )
      IF ( N13 .GT. 0 )
     1 GO TO 5942
      N16 = INDEX ( ALPHAB, PROM80(79:79) )
      IF ( N16 .GT. 0 ) THEN
      N13 = 9 + N16
      GO TO 5942
          ENDIF
      JUNIT6 = ' Error.  Halt within USER3A because the column-79'
     1     //  ' symbol for the # of coupled phases is not recognized.'
      GO TO 9200
 5942 IF ( IPHASE .GE. 2 ) GO TO 5877
      XLONG1 = XLONG
 5877 XLONG = XLONG1
      A = AA
      H2 = H3
      H2 = XLONG / H2
      CIK(NPH) = H2
      CK(NPH) = H1 * XLONG
      CI(NPH) = A
      LENGTH(1) = IPHASE
      GO TO 5285
 7284 NPH = NPH - 1
      READ (PROM80, 2721) ( TR(1,J), J=1, 6 )
      IF ( NPH .GT. 6 )
     1 READ (55, 2721) ( TR(1,J), J=7, NPH )
 2721 FORMAT ( BN, 6E12.0 )
      READ (55, 2721)  ( D3, J=1, NPH )
      DO 1795  K=2, NPH
      READ (55, 2721)  ( TR(K,J), J=1, NPH )
      READ (55, 2721)  ( D3, J=1, NPH )
 1795 CONTINUE
      GO TO 3721
 5285 CONTINUE
 2309 JUNIT6 = ' Error.  Halt within USER3A.  An EOF has been read'
     1     //  ' from the  "< User line >"  data that is assumed to'
     2     //  ' be connected to unit 55.'
      GO TO 9200
 3721 CLOSE ( UNIT=55 )
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 3941)  ( J, CI(J), CK(J), CIK(J), J=1, NPH )
 3941 FORMAT ( 7H  Row J,  10X, 5HCI(J),  10X, 5HCK(J),  9X, 6HCIK(J)
     1      ,/,  ( I7, 1P, 3E15.7 )  )
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, *)  ' Exit USER3A.  IP, NPH =',  ip, nph
      GO TO 9800
 9200 CALL BLANK6
      CALL OUTSIX ( JUNIT6(1:KOL132), KOL132 )
      CALL STOPTP
 9800 RETURN
      END
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER3A TO USER3A.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
      SUBROUTINE USER13 ( YRE )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      DIMENSION YRE(2)
      COMMON   /COM23/  CIK(20), CK(20), CI(20), LENGTH(20), NR(20)
      COMMON   /COM23/  VOLTI(20), VOLTK(20), XK(500), XM(500)
      COMMON   /COM23/  TR(20,20), QFD(210), ONEHAF, IPRSOV(3)
      COMMON   /COM23/  NPH, MN, LPAST, ISTEP, IPOINT
      CHARACTER*132  JUNIT6
      ABSZ(XS)     = DABS(XS)
      DATA LIMHST  /  500  /
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 3748) YRE(1), YRE(2)
 3748 FORMAT ( ' Top of USER13.  Incoming  DLTINV, TMAX =', 1P, 2E15.7 )
      IF ( LPAST .GT. 0 ) THEN
      CALL MOVER0 ( XK(1), LIMHST )
      CALL MOVER0 ( XM(1), LIMHST )
          ELSE
      ONEHAF = 0.5D0
          ENDIF
      LPAST = LIMHST
      IPRSUP = IPRSOV(2)
      DLTINV = YRE(1)
      TMAX   = YRE(2)
      K = 1
      ILINE = 1
      A = NPH
      II = 1
 3534 H2 = CIK(K)
      H3 = H2 * DLTINV
      I = int_convert ( H3 )
      IF ( H2 .GT. TMAX )
     1 I = -2
      N4 = int_convert ( H3 )
      D1 = N4
      CIK(K) = H3 - D1
      IF ( I .NE. 0 )  GO TO 1537
      JUNIT6 = ' Error.  Halt within USER13.  "User line" is too short'
     1     //  ' for dT being used.  The travel time is less than dT.'
      GO TO 9200
 1537 IF ( II .EQ. 1 ) LENGTH(K) = ILINE
      ILINE = ILINE + I + 2
      NR(K) = I
      IF ( ILINE .LT. LPAST ) GO TO 1536
      JUNIT6 = ' Error.  Halt within USER13.  Past history of'
     1     //  ' LPAST = 500 has overflowed for "User line" use.'
      GO TO 9200
 1536 CK1 = CK(K) * ONEHAF
      YX = CI(K)
      IF ( YX .LT. 0.0 ) GO TO 2532
      YX = YX + CK1 * ONEHAF
      CK(K) = ( YX - CK1 ) / YX
      IF ( CK(K) .GT. 0.0 ) GO TO 4533
      JUNIT6 = ' Error.  Halt within USER13.  Total modal resistance'
     1     //  ' is too large for "User line" use.'
      GO TO 9200
 2532 CK(K) = EXPZ ( CK1 / YX )
 4533 CI(K) = 1.0 / ( YX * A )
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 7014) K, YX, A, CI(K)
 7014 FORMAT ( ' USER13 defines  K, YX, A, CI(K) =',  I3, 1P, 3E15.7 )
      IF ( IPRSUP .GE. 4 )
     1 WRITE (46, 5334) K, II, LENGTH(K), ILINE, NPH, N4, H3, CIK(K)
 5334 FORMAT (  48H       K      II  LENGTH   ILINE     NPH      N4,
     1           18X, 2HH3,  14X,  6HCIK(K)   ,/,  6I8,  1P,  2E20.11 )
      IF ( II .LT. NPH ) GO TO 5375
      IT2 = 1
      GO TO 4520
 5375 IF ( II .GT. 1 ) GO TO 2538
      LENGTH(K+1) = LENGTH(K)
      LENGTH(K) = -NPH
 2538 II = II + 1
      K = K + 1
      GO TO 3534
 4520 IF ( IPRSUP .GE. 3 )
     1 WRITE (46, *) ' End of code from OVER12.  K, CI(K) =', K, CI(K)
      LPAST = ILINE - 1 + NPH
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, *) ' As in OVER13, USER13 reduces LPAST to',  LPAST
      MN = 0
      DO 4583  I=1, NPH
      DO 1396  J=1, NPH
      YX = 0.0
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 2743)  I,  J
 2743 FORMAT ( /, 'Ready for next cell G(I,J).  I, J =',  2I4  ,/,
     1            '           K          CI(K),        TR(I,K)',
     2            '        TR(J,K)             YX'  )
      DO 6592  K=1, NPH
      YX = YX + TR(I,K) * ABSZ ( CI(K) ) * TR(J,K)
      IF ( IPRSUP .GE. 4 )
     1 WRITE (46, 3696)   K,  CI(K),  TR(I,K),  TR(J,K),  YX
 3696 FORMAT ( 6X,  I6,  1P,  4E15.7 )
 6592 CONTINUE
      YX = YX * NPH
      IF ( J .GT. I )
     1 GO TO 1396
      MN = MN + 1
      QFD(MN) = YX
 1396 CONTINUE
 4583 CONTINUE
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 3945)  ( QFD(J), J=1, MN )
 3945 FORMAT ( ' G Matrix:', 1P,  4E15.7 ,/, ( 10X, 4E15.7 ) )
      CALL MOVER ( QFD(1),
     1             YRE(1), MN )
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 1746)  LENGTH(1), LENGTH(2)
 1746 FORMAT ( ' Exit  USER13.  LENGTH(1:2) =', 2I6 )
      GO TO 9800
 9200 CALL BLANK6
      CALL OUTSIX ( JUNIT6(1:KOL132), KOL132 )
      CALL STOPTP
 9800 RETURN
      END
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER1B TO USER13.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
      SUBROUTINE USER17 (
     1 VOLTS, VOLTR )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      DIMENSION  VOLTS(1), VOLTR(1)
      COMMON   /COM23/  CIK(20), CK(20), CI(20), LENGTH(20), NR(20)
      COMMON   /COM23/  VOLTI(20), VOLTK(20), XK(500), XM(500)
      COMMON   /COM23/  TR(20,20), QFD(210), ONEHAF, IPRSOV(3)
      COMMON   /COM23/  NPH, MN, LPAST, ISTEP, IPOINT
      SAVE  UNITY, IPRSUP
      ABSZ(XS)     = DABS(XS)
      IF ( ISTEP .NE. -1 )
     1 GO TO 3957
      IPRSUP = IPRSOV(3)
      UNITY  = 1.0
      IF ( IPRSUP .GE. 1 )
     1 WRITE (46, *) ' Next, 4 lines of initialization from SUBR15.'
      DO 5406  I=1, NPH
      CI1 = CI(I)
      CK1 = ABSZ ( CK(I) )
      A = 1.0
      CI(I) = CI(I) * ( A + CK1 )
 5406 CONTINUE
      IF ( IPRSUP .GE. 1 )  THEN
      WRITE (46, *) ' End code from SUBR15.  CI =',  ( CI(I), I=1, NPH )
      WRITE (46, *) ' End of step-0 initialization within USER17.'
          ENDIF
 3957 IPOINT = IPOINT + 1
      IF ( IPOINT .EQ. LPAST ) IPOINT = 0
      ISTEP  = ISTEP  + 1
      II = 1
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 6238)  NPH, ISTEP, IPOINT
 6238 FORMAT ( ' New step in USER17.  NPH, ISTEP, IPOINT =',  3I5 )
      N3 = 1
      DO 1197  I=1, NPH
      CI1 = 0.0
      CK1 = 0.0
      DO 1196  J=1, NPH
      CI1 = CI1 + TR(J,I) * VOLTS(J)
      CK1 = CK1 + TR(J,I) * VOLTR(J)
 1196 CONTINUE
      VOLTI(I) = CI1 * NPH
 1197 VOLTK(I) = CK1 * NPH
      IF ( IPRSUP .GE. 1 )
     1 WRITE (46, 2197) ( VOLTS(I),  VOLTR(I),
     2             VOLTI(I), VOLTK(I),  I=1, NPH )
 2197 FORMAT ( 37H Phase voltage at receive, send ends.,  3X,
     1         36H Mode voltage at receive, send ends.  ,/,
     2         10X, 8HVOLTS(I),  10X, 8HVOLTR(I), 13X, 8HVOLTI(I),
     3         10X, 8HVOLTK(I)   ,/,  ( 1X, 2E18.10, 4X, 2E18.10  )  )
      CK1 = 0.0
      CI1 = 0
      GUS1 = UNITY
      MSIGN = 1
      I = 0
 4189 I = I + 1
      N3 = NR(MSIGN)
      IF ( N3 .GE. 0 ) GO TO 1187
      H1 = 0.0
      D2 = 0.0
      GO TO 1188
 1187 IT1 = II + IPOINT
      N4 = IT1 + N3
      IF ( IT1 .GT. LPAST ) IT1 = IT1 - LPAST
      IF ( N4 .GT. LPAST ) N4 = N4 - LPAST
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 3482)  I, II, IPOINT, N3
 3482 FORMAT ( ' USER17 past history.  I, II, IPOINT, N3 =',  4I6 )
      IT21 = IT1 + 1
      N41 = N4 + 1
      IF ( IT21 .GT. LPAST ) IT21 = 1
      IF ( N41 .GT. LPAST ) N41 = 1
      XMN4 = XM(N4)
      A = CIK(MSIGN)
      D2 = 1.0 - A
      GUS2 = CI(MSIGN)
      H1 = ABSZ ( GUS2 )
      H2 = ABSZ ( CK(MSIGN) )
      IF ( IPRSUP .GT. 8 )
     1 WRITE (46, 3192)  I, MSIGN, IT1, IT21, CIK(MSIGN), CI(MSIGN),
     2                   CK(MSIGN), XM(IT1), XK(IT1), XM(IT21), XK(IT21)
 3192 FORMAT ( ' I, MSIGN, IT1, IT21 =', 4I5,  5X, 'CIK(MSIGN),',
     1 ' CI(MSIGN), CK(MSIGN), XM(IT1), XK(IT1), XM(IT21), XK(IT21) ...'
     2   ,/,  ( 1X, 5E25.16 ) )
      XM(N4) = H1 * VOLTI(I) - H2 * XK(N4)
      XK(N4) = H1 * VOLTK(I) - H2 * XMN4
      H1 = XK(IT1) * A + XK(IT21) * D2
      D2 = XM(IT1) * A + XM(IT21) * D2
      IF ( GUS2 .LT. 0.0 )  GO TO 1192
      A = H1 + D2
      D2 = ( H1 - D2 ) * H2
      H1 = ( A + D2 ) * ONEHAF
      D2 = ( A - D2 ) * ONEHAF
 1192 XK(N41) = H1
      XM(N41) = D2
      IF ( IPRSUP .GT. 6 )
     1 WRITE (46, 1193) I, MSIGN, IT1,N41, ( XK(J),XM(J),J=IT1, N41)
 1193 FORMAT (  ' I, MSIGN, IT1, N41 =', 4I5,  5X,
     1      ' (XK(J), XM(J), J=IT1, N41)  ....,'  ,/,  ( 1X, 5E25.16 ) )
 1188 II = II + N3 + 2
      VOLTI(I) = H1 * GUS1
      VOLTK(I) = D2 * GUS1
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 4287) I, H1, D2, GUS1
 4287 FORMAT ( ' For VOLTI/K cell I =', I3,
     1         '  H1, D2, GUS1 =',  1P,  3E15.7 )
      CI1 = CI1 + H1
      CK1 = CK1 + D2
      MSIGN = MSIGN + 1
      IF ( I .LT. NPH ) GO TO 4189
      DO 1203  J=1, NPH
      H1 = 0.0
      H2 = 0.0
      DO 1199  I=1, NPH
      H1 = H1 + TR(J,I) * VOLTI(I)
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 5636) N4, TR(J,I), VOLTI(I), H1
 5636 FORMAT ( ' N4, TR(J,I), VOLTI(I), H1 =', I4, 1P, 3E15.7 )
      H2 = H2 + TR(J,I) * VOLTK(I)
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 5639)  VOLTK(I), H2
 5639 FORMAT ( '              VOLTK(I), H2 =',  1P, 2E15.7 )
 1199 CONTINUE
      VOLTS(J) = H1
      VOLTR(J) = H2
      IF ( IPRSUP .GE. 3 )
     1 WRITE (46, 2199) J, NPH, H1, H2
 2199 FORMAT ( ' J, NPH =', 2I6, 5X, ' H1, H2 =',  1P,  4E15.7 )
 1203 CONTINUE
      IF ( IPRSUP .GE. 1 )
     1 WRITE (46, *)  'Exit USER17.'
      RETURN
      END
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
C USER17 TO USER17.F BY HEVIA
