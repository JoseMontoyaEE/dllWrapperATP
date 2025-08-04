rem Creates the files which are needed to compile the new 
rem tpbig.exe : codetacs.inc decktac1.inc decktac2.inc
rem Recommended to insert "SINGLE STEP IF MAKE" in a data file
SET COMPTACS=MAKE
tpbig disk %1. s -r
SET COMPTACS=
del comtac.o
copy decktac1.inc comta1.ins
copy decktac2.inc comta2.ins
copy codetacs.inc comta3.ins
