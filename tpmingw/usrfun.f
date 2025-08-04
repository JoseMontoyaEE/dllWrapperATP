C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
      FUNCTION USRFUN ( TS, K )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      CHARACTER*132  JUNIT6
      EXPZ(XS)     = DEXP(XS)
      SINZ(XS)     = DSIN(XS)
      IF ( K .NE. 5 )  GO TO 1482
      TPEAK = 0.25D0
      IF ( TS .LE. TPEAK ) THEN
      D5 = 400.0 * TS
          ELSE
      D1 = TS - TPEAK
      D5 = 100.0 - 100.0 * D1
          ENDIF
      USRFUN = D5
      GO TO 9800
 1482 IF ( K .NE. 8  .AND.
     1     K .NE. 9 )  GO TO 3762
      D2 = 12.3D6 * TS + 1.22173D0
      D3 = SINZ ( D2 )
      D4 = -4.1D6 * TS
      D5 = 1.064D0 *  EXPZ ( D4 )
      D6 = -0.123D6 * TS
      D7 = 0.988D0 * EXPZ ( D6 )
      D8 = D7 - D5 * D3
      IF ( K .EQ. 8 )  THEN
      USRFUN = 850000.0 * D8
          ELSE
      USRFUN = 995000.0 * D8
          ENDIF
      GO TO 9800
 3762 IF ( K .NE. 2 ) GO TO 2960
      TT=TS*1.0D6
      A        = 0.3236533133E+10
      B        = 0.6066032935E-01
      C        = 0.8045110595E+01
      D        = 0.3120717040E+05
      E        = 0.3307170404E+04
      TX1      = 0.1033140718E-03
      TX2      = 0.1257380698E-06
      T90      = 0.4599397763E-05
      IF(TS.LE.T90)THEN
      D5 = A*TS+B*TT**C
      ELSE
      D5 = D*EXPZ(-(TS-T90)/TX1)-E*EXPZ(-(TS-T90)/TX2)
      ENDIF
      D5 = D5 / 31000.
      USRFUN = D5
      GO TO 9800
 2960 WRITE (JUNIT6, 2963)  K
 2963 FORMAT ( ' Halt in  USRFUN.  Type-15 USRFUN source has type',
     1         ' code  CREST =', I6,  '.  This has not been defined.' )
      CALL OUTSIX ( JUNIT6, 131 )
      CALL STOPTP
 9800 RETURN
      END
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
C USRFUN TO USRFUN.F BY HEVIA
