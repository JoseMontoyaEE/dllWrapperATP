Listsize.dat is useful only to _reduce_ the size of ATP tables.

With the available memory in new computers, it is rarely
necessary.

To increase the table sizes, it is necessary to compile/link the
code.

It is a simple job when one knows how to do it.

To make easy the life of the users, I generate a giga version of
ATP, tpgig.exe,

When any user founds table size problems, the first solution to
try is to use the giga version.

If it fails, it is mandatory to recompile the code with bigger
numbers in the tables being overflowed. Because the total size
must be more or less the same, some table sized _must_ be reduced
to maintain the total size.

If you found too difficult to obtain the correct sizes, send me
the sizes you need for your specially dimensioned ATP, or your
data case (not ATPDRaw files, please), and I will try to obtain a
tpbig with adequate sizes.


To compile/link the code, it is necessary to download the file
libmingw.zip.

In this file, there is the instruction text readme.txt, that I
include here:



Instructions to compile the ATP
-------------------------------

I- To change the dimensions

1.  Edit the file  listsize.ext, and set  the  size of  each
table as required.

2.  Run  the  command  file  vardimn.bat.  Vardimn.bat  runs
vardim.exe and the files dimdef.f and newmods.f are created.

     > vardimn listsize.ext


3.  Edit makefile, and change the directory where the DISLIN
graphic library was installed.

If DISLIN graphic library is  not present, a  dummy  library
dislin.a can be used, as explained in makefile.

DISLIN  is  available from the following site:

https://www.mps.mpg.de/dislin/


4. Run make with makefile as argument.

     > make

II. To obtain a COMPILED TACS version of tpbig.

At first, set  environment  variable COMPTACS as "MAKE", and
run.

     > set COMPTACS=MAKE
     > del comtac.o
     > runtp datafile

   Then, the following three files are created.

    decktac1.inc
    decktac2.inc
    codetacs.inc

This  files  must be copied  to  comta1.ins,  comta2.ins and
comta3.ins respectively, as required by comtac.f.

It is better to include "SINGLE STEP IF MAKE" in data files.
By  this command, tpbig stops immediately after creating the
above three files.  Without this command, tpbig doesn't stop
until the simulation reaches tmax.

The batch file  comtac1.bat does this work,  with your  data
file as argument

    > comtac1 data

The above three files  are included by comtac.f module.  The
new tpbig  is created by compiling comtac.f and linking with
other modules.


2. Run make with makefile as argument.

    > make

A lot  of  warning  messages  can  be  avoided  by  changing
dimensions in comtac.f to the same value as SPTACS dimension
in newmods.f.

3. Then, set environment variable COMPTACS as "USE", and run
the new tpbig.

    > COMPTACS=USE
    > tpbig  DISK datafile. s -r
    > COMPTACS=

The batch file  comtac2.bat does  this  work  with your data
file as argument.

    > comtac2 data

The original  tpbig  in the  distribution  was compiled with
the following three files whose  sizes are  0.

comtac1.ins
comtac2.ins
comtac3.ins


 Some tips for the correct operating of compiled TACS
  - Some variable names should not be used; I, IA, T, ...
  - The integer number should not be used as a denominator;
    1/2 should be 1.D0/2.D0 , .....
  - Logical expression should be changed by hand;

    For example,
      SIGAP   = .NOT. SIGA
      This code should be changed like as follows;
AASIGAP   = .NOT.SIGA
BB  IF ( SIGA .GT. 0.0 ) then
BB       SIGAP = 0.0
BB         ELSE
BB       SIGAP = 1.0
BB      ENDIF

    Other example,
     SIGNLC  = SIGN( ERRORC ) * ( ABS( ERRORC ) .GT. DELTAI )
     This code should be changed like as follows;
AASIGNLC  = SIGN( ERRORC ) * ( ABS( ERRORC ) .GT. DELTAI )
BB   IF ( DABS( ERRORC ) .GT. DELTAI ) THEN
BB     SIGNLC  = SIGNZ( ERRORC ) * 1.0D0
BB   ELSE
BB     SIGNLC = 0.D0
BB   ENDIF

     As for the AA/BB features of Compiled TACS, please refer to the
     following URL:
     http://www.jaug.gr.jp/~atp/develop-e.htm#tacs

III. To add a foreign function to MODELS.

1. Edit the file fgnmod.f and add your function.

   Follow the instructions in the fgnmod.f

2. Run make with makefile as argument.

    > make


IV. To add a device type-79 to TACS.

1. Edit the file devt69.f and add your function.

   Use one of the samples as guide.
   
2. Run make with makefile as argument.

    > make



V. MINGW compiler documentation

MinGW -- Installation instructions
==================================


1. Download the Mingw compiler from:

  
https://dl.dropboxusercontent.com/u/31810002/varios/MINGW5.ZIP  

2. Install the package by unpacking the file MINGW5.ZIP. Choose a
directory as C:\mingw\


3. Setting up environment variables:

   You'll still need to add the mingw bin directory,
   C:\mingw\bin, to your PATH for GCC to be able to find various
   other tools such the linker, assembler, etc.

   The following is all you need for this release:

     C:\> PATH=C:\mingw\BIN;%PATH%


Good luck.

Links:


Compiler:

https://dl.dropboxusercontent.com/u/31810002/varios/MINGW5.ZIP

Graphics library:

https://www.mps.mpg.de/dislin/
