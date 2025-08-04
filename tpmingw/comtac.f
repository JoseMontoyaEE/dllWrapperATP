C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
      SUBROUTINE COMTAC
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      INCLUDE  'dekspy.ins'
      INCLUDE  'sixty4.ins'
      COMMON  / C0B061 /   BVALUE(    1 )
      COMMON  / C0B102 /   E     (    1 )
      COMMON  / C0B100 /   KPOS  (    1 )
      COMMON  / C0B014 /   SPTACS( 9600 )
      COMMON  / C0B098 /   TCLOSE(    1 )
      COMMON  / C0B068 /   TEXVEC(   45 )
      CHARACTER*6  TEXVEC
      COMMON  / C0B002 /   YKM   (    6 )
      INCLUDE  'tacsar.ins'
      EQUIVALENCE       ( MONCAR(61),  LSWTCH )
      EQUIVALENCE    ( MONCAS(16),  MULPPF ),   ( MONCAS(146),  LIST01 )
      EQUIVALENCE    ( MONCAS(233), NUMTY1 ),   ( MONCAS(234),  LSUP   )
      EQUIVALENCE    ( MONCAS(235), INLINE ),   ( MONCAS(236),  NEXTAC )
      EQUIVALENCE    (PEAKND(4),  DLTINV),    (PEAKND(14), TENFLZ)
      EQUIVALENCE    (PEAKND(38), PICON ),    (PEAKND(71), TIMTAC)
      EQUIVALENCE                               ( MONCAS(141),  LIMTAC )
      EQUIVALENCE                                ( MONCAR(39),  NIUNRS )
      EQUIVALENCE ( E(1), KKE )
      INCLUDE  'comta1.ins'
      INCLUDE  'comta2.ins'
          DATA  FLEVEL  /  0.0  /
      IF ( IPRSUP .GE. 1 )
     1 WRITE (LUNIT6, *)  ' Top  COMTAC.   NIU, NUK, NSUP, ISTEP =',
     2                                     NIU, NUK, NSUP, ISTEP
      KOVM16 = JBEGOV(16) - 1
      IF ( KOVM16 .LT. 0 )
     1 GO TO 9999
      IF ( ISTEP .NE. 1 ) GO TO 9999
      N5 = LOCINT ( KKE )
      IF ( N5 .NE. KPOS(LSWTCH) ) THEN
      JUNIT6 = ' Inconsistent location of E.  Correct the'  //
     1         ' serialization of C0BXXX top of COMTAC.  Halt.'
      CALL OUTSIX ( JUNIT6, 132 )
      CALL STOPTP
          ENDIF
      IF ( FLEVEL .NE. 1.234567E8 )
     1 WRITE (LUNIT6, *) ' Begin  COMPILED TACS USE  on step 1.'
 9999 KJSUP = KINSUP + LSTAT(65)
      KKSUP = KJSUP  + LSTAT(65)
      INCLUDE  'comta3.ins'
      RETURN
      END
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
C COMTAC TO COMTAC.F BY HEVIA
