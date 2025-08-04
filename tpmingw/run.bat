DEL  *.LIS
DEL  DC*.PS
DEL  DC*.DBG
DEL  DC*.GNU
DEL  DC*.PL4
DEL  8B1111*.*
DEL  DC*.PCH
DEL  DC*.HPG
DEL  *.EPS
DEL  *.TMP
DEL  d*.BIN
DEL  *.EXT
DEL  \ld\*.28*
DEL  \ld\*.59*
DEL  LMFS*.DAT
DEL  SHOT*.DAT
DEL  ATPPAPER.*
DEL  ATPHPGL.*
DEL  ATPPOST.*
DEL  ATPVIDEO.*
DEL  CODETACS.INC
DEL  DECKTAC*.INC
DEL  MODELS.1
COPY  STARTUP  DC37STAR.DAT
COPY  LISTSIZE.333  *.DAT
Rem  The following  SET  enables ATP interception of Ctrl-Break.  Anything
Rem  other than  =NOTRAP  will ensure no ATP trapping of Ctrl-Break.  So,
Rem  we enable at the beginning,  and cancel at the end (see bottom):
Rem SET CTRLBK=TRAP
time < blank  > date.lis
Rem   Beginning 7 June 2014,  RUNTP4.BAT  has precisely 4 arguments.  For a week
Rem   or two, it was the old (original) RUNTP.BAT, which had 6.  But typical use
Rem   involves just 4, so remove  "%5 %6"  leaving just args  "%1 %2 %3 %4".  As
Rem   for  RUNTP.BAT,  expand arguments of it to the MS-DOS limit of 9.
CALL RUNTP4  DISK  c:\data\dc1.  dc1.  -R
CALL RUNTP4  DISK  c:\data\dc2.  dc2.  -R
CALL RUNTP4  DISK  c:\data\dc3.  dc3.  -R
Rem    DC4.DAT  has become universal on  21 February 2015  because of the
Rem    use of installation-dependent  $INCLUDE  which next will be copied
Rem    to make sure that GNU ATP is not using Salford ATP files:
Rem   THL: since I run run.bat from \gnunt, not from \data, so:
rem   DC4INCL*.DAT
DEL   \data\DC4INCL*.DAT
COPY  \GNUNT\DC4INCL*.DAT \data
Rem    Use of MS Excel CSV-compatible data in  DC4.DAT  requires MSCSV = 1 or 2.
Rem    Value 2 would be more efficient but we use "1" as an illustration to show
Rem    that automatic detection of closed, in-line comments  {...}  is possible.
Rem    Another anamoly will be a mixture of upper and lower case,  to prove that
Rem    Salford ATP recognizes either without difficulty.  By analogy to  RUNTP4,
Rem    RUNTP8  involves 8 arguments:
CALL RunTP8  diSK  c:\data\dc4.  dc4.  -R  MsCsV 1  Notab 1
Rem   The preceding illustrates the use of 2 or more parameters at the same
Rem   time.  Adding  NOTAB1  does not change the output in any way other than
Rem   add one line of output at the very beginning.  Service begins 18 May 2014.  WSM.
CALL RUNTP4  DISK  c:\data\dc5.  dc5.  -R
Rem    Demonstrate that additional args can be handled by use of virtual blanks.
Rem    This is like DC-65.  MS-DOS treats the "#" sign as a non-blank so  -R#...
Rem    is the 4th and final argument.  Once inside ATP,  the "#" is erased.  The
Rem    resulting simulation is unchanged since  NOTAB = 1  and  NOPISA = 0  are
Rem    the default values of  STARTUP.  WSM.  7 June 2014
Rem  RUNTP4  DISK  c:\data\dc6.  S  -R  Notab  1  NoPisa  0  <---  What ATP sees
Rem  CALL RUNTP4  DISK  c:\data\dc6.  dc6.  -R##Notab##1##NoPisa##0

Rem    ---------------------- Begin embedded blank test
Rem    Illustrate a disk file name with an embedded blank.  WSM.  10 Sept 2015
del  c:\data\"dc 6.lis"
del  c:\data\"dc 6.dbg"
del  c:\data\dum.lis
Rem  CALL RUNTP4  DISK  "DC 6."  S  -R##Notab##1##NoPisa##0
CALL RUNTP4  DISK  "c:\data\DC 6."  "DC 6."  -R##Notab##1##NoPisa##0
Rem    Illustrate a directory name with an embedded blank.  WSM.  15 Sept 2015
del  "c:\data\Project One\DC6.lis"
del  "c:\data\Project One\DC6.dbg"
CALL RUNTP4  DISK  "c:\data\Project One\DC6."  S  -R##Notab##1##NoPisa##0
Rem    ----------------------   End embedded blank test

CALL RUNTP4  DISK  c:\data\dc7.  dc7.  -R
CALL RUNTP4  DISK  c:\data\dc8.  dc8.  -R
CALL RUNTP4  DISK  c:\data\dc9.  dc9.  -R
CALL RUNTP4  DISK  c:\data\dc10. dc10.  -R
Rem   As long as  DC11.DAT  has too long data lines due to  FAULTS TO GROUND
Rem   (never-read output is part of data),  we must exempt  DC-11  from any
Rem   possible check for  Tabs.   This is done by adding the optional  "NOTAB 0"  
Rem   which orders the setting of  NOTAB = 0.   The use of  "NOTAB  0"  could be
Rem   removed if we were to add  $DISABLE  and  $ENABLE  commands to exclude
Rem   long lines that are not ATP data.   But we prefer to illustrate  "NOTAB 0"
Rem   which raises the argument count to 6.  Thus RUNTP6 replaces  RUNTP4.  This
Rem   is the more elegant than the use of "#" with RUNTP4  as was illustrated by
Rem   by the  DC-6  simulation.  WSM.  7 June 2014
CALL RUNTP6  DISK  c:\data\dc11.  dc11.  -R  NOTAB 0
CALL RUNTP4  DISK  c:\data\dc12.  dc12.  -R
CALL RUNTP4  DISK  c:\data\dc13.  dc13.  -R 
CALL RUNTP4  DISK  c:\data\dc14.  dc14.  -R  
CALL RUNTP4  DISK  c:\data\dc15.  dc15.  -R 
CALL RUNTP4  DISK  c:\data\dc16.  dc16.  -R
Rem    Illustrate use of remote data but local .LIS and .DBG output files.  This
Rem    is especially tricky because only the main data file and two of the five
Rem    $INCLUDE  or  $INSERT  files are remote.  Three more are local.  Study
Rem    the several  $PREFIX  declarations to understand how the choice between
Rem    \MAIL  and  \DATA  is made.  WSM.  22 March 2015
CALL RUNTP4  DISK  \mail\DC17.  DC17.  -R
rem  CALL RUNTP4  DISK  c:\data\dc17.  dc17.  -R 
Rem  DEL  ATPVIDEO.*
CALL RUNTP4  DISK  c:\data\dc18.  dc18.  -R
DEL  DC18.001
DEL  DC18.002
RENAME  ATPHPGL.002  DC18.HPG
Rem  RENAME  ATPVIDEO.*  DC18.*
DIR  DC18.*
rem   The following makes  L4BYTE = 0  to produce an UNFORMATTED .PL4 file:
CALL RUNTP6  DISK  c:\data\dc19.  dc19.  -R  L4BYTE 0
CALL RUNTP4  DISK  c:\data\dc20.  dc20.  -R
Rem  CALL RUNTP4  DISK  c:\data\dc21.  dc21.  -R
Rem    Illustrate use of remote data but local .LIS and .DBG output files.  This
Rem    is especially tricky because  DC68*.DAT  also must be remote;  the  TACS
Rem    data of  DC-21  uses the  MODELS  data of  DC-68.  WSM.  22 March 2015
CALL RUNTP4  DISK  \mail\DC21.  DC21.  -R
CALL RUNTP4  DISK  c:\data\dc22.  dc22.  -R
CALL RUNTP4  DISK  c:\data\dc23.  dc23.  -R
REM    ========  Begin coupled pair of DC-24 and DC-40:
CALL RUNTP4  DISK  c:\data\dc24.  dc24.  -R
CALL RUNTP4  DISK  c:\data\dc40.  dc40.  -R
DEL  DC24AT40.BIN
REM      ==========   End of coupled pair  ===========
CALL RUNTP4  DISK  c:\data\dc25.  dc25.  -R
CALL RUNTP4  DISK  c:\data\dc26.  dc26.  -R
DEL  TAKUNODA.CCC
CALL RUNTP4  DISK  c:\data\dc27.  dc27.  -R
RENAME  TAKUNODA.CCC  DC27TAKU.LIS
DEL  \ld\*.016 
DEL  \ld\*.037 
DEL  \ld\*.038 
DEL  \ld\*.mtx
DEL  \ld\*.mat
CALL RUNTP4  DISK  c:\data\dc28.  dc28.  -R
CALL RUNTP4  DISK  c:\data\dc29.  dc29.  -R
CALL RUNTP4  DISK  c:\data\dc30.  dc30.  -R
Rem  CALL RUNTP4  DISK  c:\data\dc31.  dc31.  -R
Rem    Illustrate use of remote data but local .LIS and .DBG output files:
CALL RUNTP4  DISK  \mail\DC31.  DC31.  -R
REM    ========  Begin coupled pair of DC-32 and DC-49:
CALL RUNTP4  DISK  c:\data\dc32.  dc32.  -R
COPY  LISTSIZE.555  *.DAT
CALL RUNTP4  DISK  c:\data\dc49.  dc49.  -R
COPY  LISTSIZE.333  *.DAT
REM      ==========   End of coupled pair  ===========
CALL RUNTP4  DISK  c:\data\dc33.  dc33.  -R
RENAME  ATPHPGL.002  DC33.HPG
CALL RUNTP4  DISK  c:\data\DC34.  dc34.  -R
del dc35pcx.*
DEL  DC35LJ2.*
DEL  DC35HPGL.*
Rem  DEL  ATPVIDEO.* 
DEL  ATPPAPER.* 
DEL  ATPHPGL.*  
CALL RUNTP4  DISK  c:\data\dc35.  dc35.  -R
Rem  RENAME  ATPVIDEO.*  DC35PCX.*
RENAME  ATPPAPER.*  DC35LJ2.*
RENAME  ATPHPGL.*   DC35HPGL.*
DIR  DC35*.*
CALL RUNTP4  DISK  c:\data\dc36.  dc36.  -R
CALL RUNTP4  DISK  c:\data\dc37.  dc37.  -R
CALL RUNTP4  DISK  c:\data\dc38.  dc38.  -R
CALL RUNTP4  DISK  c:\data\dc39.  dc39.  -R
CALL RUNTP4  DISK  c:\data\dc41.  dc41.  -R
copy c:\data\dc42inc*.dat dc42inc*.dat
CALL RUNTP4  DISK  c:\data\dc42.  dc42.  -R
RENAME  ATPHPGL.006  DC42.HPG
CALL RUNTP4  DISK  c:\data\dc43.  dc43.  -R
CALL RUNTP4  DISK  c:\data\dc44.  dc44.  -R
CALL RUNTP4  DISK  c:\data\dc45.  dc45.  -R
rem     The following file  DC45PISA  was created by a special simulation of
rem     DC-45 using  NEWPL4 = 2.  This was the only change to STARTUP.  Signals
rem     are not alligned for Watcom (which requires  LENREC = 1, too).  The
rem     file used for Watcom is a little bigger (8 bytes).
rem       7 December 2012,  we  switch from old C-like to new (Pisa) format.  In
rem       fact,  data is being changed so that it could be either.  An added 4th
rem       subcase precedes the old one.  The new uses  DC45CLIK.PL4  rather than
rem       DC45PISA.PL4  of the old one.  DC-46  now illustrates both.  WSM.
COPY  \data\DC45PISA  DC45PISA.PL4 
COPY  \data\DC45CLIK  DC45CLIK.PL4
CALL RUNTP4  DISK  c:\data\dc46.  dc46.  -R
Rem       Introduce the use of ":" to separate a parameter name from its value.
Rem       Yes, "=" would be better but MS-DOS or Salford DBOS converts this to
Rem       a blank, unfortunately.  A colon is the best we can do.  For a number
Rem       that involves 2 or more decimal digits, use of ":" is mandatory.  For
Rem       the case of a single digit,  it is an option that may be clearer if
Rem       the parameter name itself ends in a decimal digit (as MAXL31 does).
Rem       Both parameters involve new limits on data cards which now are held in
Rem       RAM by ATP rather than on disk.  Data cards of the user are offset in
Rem       storage of size  LIMCRD  by  MAXL31,  which is List 31.  This example
Rem       illustrates that no offset is required as long as no IDEAL TRANSFORMER
Rem       data is involved.  LIMCRD  is the limit on the total storage.  LIMCRD
Rem       must not exceed List 31 that was used to link ATP (see LISTSIZE.BPA).
Rem       For our use with Salford ATP,  this limit is 24K.  So here the request
Rem       for 150K obviously is illegal and ATP will truncate it to 2 less than
Rem       the List-31 limit, so 23998.  Two slots are required for BNDC & BLANK
Rem       cards to terminate execution after the 1st subcase (had there been 2
Rem       or more,  only the first would be simulated).  WSM.  18 May 2014
Rem       One day later, accept "K" in place of three trailing zeros:
CALL RUNTP8  DISK  c:\data\dc47.  dc47.  -R  Maxl31 0  LIMcrd 150k
CALL RUNTP4  DISK  c:\data\dc48.  dc48.  -R
CALL RUNTP4  DISK  c:\data\dc50.  dc50.  -R
CALL RUNTP4  DISK  c:\data\dc51.  dc51.  -R  
CALL RUNTP4  DISK  c:\data\dc52.  dc52.  -R 
CALL RUNTP4  DISK  c:\data\dc53.  dc53.  -R
CALL RUNTP4  DISK  c:\data\dc54.  dc54.  -R
CALL RUNTP4  DISK  c:\data\dc55.  dc55.  -R
CALL RUNTP4  DISK  c:\data\dc56.  dc56.  -R
rem same as dc35, used to run with local data
Rem  DEL  ATPVIDEO.*
Rem    DC57.DAT  has become universal on  20 February 2015  because of the
Rem    use of installation-dependent  $INCLUDE  which next will be copied
Rem    to make sure that GNU ATP is not using Salford ATP files:
Rem   THL: since I run run.bat from \gnunt, not from \data, so:
Rem  DEL          DC57INC*.DAT
DEL   \data\DC57INC*.DAT
COPY  DC57INC*.DAT \data
Rem    CALL RUNTP4  DISK  DC57.  S  -R
CALL RUNTP4  DISK  c:\data\DC57.  dc57.  -R
DEL  DC57.001
DEL  DC57.002
Rem  RENAME  ATPVIDEO.*  DC57.*
DIR  DC57.*
CALL RUNTP4  DISK  c:\data\dc58.  dc58.  -R
DEL  TAKUNODA.CCC
CALL RUNTP4  DISK  c:\data\dc59.  dc59.  -R
RENAME  TAKUNODA.CCC  DC59TAKU.LIS
CALL RUNTP4  DISK  c:\data\dc60.  dc60.  -R
CALL RUNTP4  DISK  c:\data\dc61.  dc61.  -R
CALL RUNTP4  DISK  c:\data\dc62.  dc62.  -R
CALL RUNTP4  DISK  c:\data\dc63.  dc63.  -R
CALL RUNTP4  DISK  c:\data\dc64.  dc64.  -R
REM    Next case involves character conversion: "~" = "," and "#" = blank
REM         DISK  DC65.DAT , 0.42 , DC65.  -R  ----  What we really want
CALL RUNTP4  DISK##c:\data\dc65.DAT#~#0.42#~ DC65.  -R
REM    Yes, it would have been nice to replace the 2nd "DC65." by "*" but
REM    this results in "*" being the 2nd argument,  so  RUNTP  DIR  then
REM    produces an inventory of all files.  So abandon the reform.
CALL RUNTP4  DISK  c:\data\dc66.  dc66.  -R
CALL RUNTP4  DISK  c:\data\dc67.  dc67.  -R
Rem  CALL RUNTP4  DISK  c:\data\dc68.  dc68.  -R
Rem    Illustrate use of remote data but local .LIS and .DBG output files.
Rem    This is tricky business because of coupling between DC-21 and DC-68.
CALL RUNTP4  DISK  \mail\DC68.  DC68.  -R
REM    ========  Begin coupled pair of DC-69 and DC-70:
Rem    The following DC-69 has no useful purpose other than illustration of
Rem    the use of  UNIXON = 7  in  STARTUP.  But since  STARTUP  is not
Rem    conveniently changed,  we order  UNIXON = 7  via the command line.
Rem    The  UNIXON  parameter,  if given the exceptional value of 7,  will
Rem    result in the conversion of an MS-DOS data file to Unix.  But before
Rem    such creation,  we copy a larger output file (huge DC1.LIS) on top of
Rem    the old  DC69.LIS  to prove that the old really is destroyed rather
Rem    overwritten.  The same goes for the Unix output file  DC69.UNX  (the
Rem    is parallel to the input data file, with  .UNX  a fixed file type).
Rem    Also, we use  DIR  to add documentation of the  .UNX  file to the
Rem    .LIS  file:
COPY  DC1.LIS  DC69.LIS
COPY  DC1.LIS  DC69.UNX
CALL RUNTP6  DISK  c:\data\dc69.  dc69.  -R  UNIXON 7
Rem  DIR  DC69.UNX  >>  DC69.LIS
Rem       The preceding  DIR  is great for practical use.  But it is bad for
Rem       the verification of standard test cases because the date changes.
DELETE  dc70.dat
Rem COPY  DC69.UNX  DC70.DAT
COPY  c:\data\DC69.UNX  DC70.DAT
Rem    The following DC-70 has no useful purpose other than illustration
Rem    of the use of  UNIXON = 1  in  STARTUP.  But since  STARTUP  is not
Rem    conveniently changed,  we order  UNIXON = 1  via the command line.
Rem    This orders ATP to expect input data to be a Unix (not MS-DOS) file.
Rem    In this case,  note input data  DC70.DAT  is created by the preceding
Rem    simulation of DC-69.  Disk file  DUMBUNIX.DAT  will be created at the
Rem    start of execution.  As Unix data lines are read from  DC70.DAT,  they
Rem    will be copied to  DUMBUNIX.DAT  as MS-DOS lines.  So at the end of
Rem    execution,  DUMUNIX.DAT  should be the same as  DC69.DAT,  in case
Rem    the user might be interested.  It is essentially a scratch file.
DELETE  dumbunix.dat
CALL RUNTP6  DISK  DC70.  dc70.  -R  UNIXON 1
REM      ==========   End of coupled pair  ===========
Rem    The following DC-71 has no useful purpose other than illustration 
Rem    of the use of  NOTAB > 0  in  STARTUP.  But since  STARTUP  is not
Rem    conveniently changed,  we order  NOTAB = 1  via the command line.
Rem    NOTAB > 0  protects ATP from data that involves  Tab  characters
Rem    as well as non-comment data lines that are longer than 80 bytes.
Rem    The data of  DC71.DAT  will be rejected by ATP as documented by
Rem    the  .LIS  file.
CALL RUNTP6  DISK  c:\data\dc71.  dc71.  -R  NOTAB 1
copy \data\dc71.lis .
CALL RUNTP4  DISK  c:\data\dc72.  dc72.  -R
Rem    Demonstrate KILL = 274 which complains about a bad parameter of the
Rem    command line.  In the following, the number following MSCSV is missing:
CALL RUNTP6  DISK  c:\data\dc73.  S  -R  MSCSV
Rem  ----------------------------
Rem  New logic of GUTATP does a better job of rejecting errors
Rem  of the execution command line.  Next, we illustrate this
Rem  by adding extraneous  "Bad  yyy"  before otherwise-legal
Rem  parameters.  This addition is made  23 June 2015.  WSM.
CALL RUNTP6  Bad  yyy  DISK  DC74.  S  -R
Rem  ----------------------------
Rem      Next demonstrate multiple  FILE  requests.  Output will be DC75.LIS
Rem      Input data is  DC6.DAT  (the 3rd of 5 parameters):
CALL RUNTP6  FILE c:\data\dc75a.dat c:\data\DC6. FILE c:\data\dc75b.dat
Rem      About the preceding,  DC75B.DAT  will name the output file  DC75.LIS
Rem      The last parameter of  DC75A.DAT  is  DISK.  After evaluation of the
Rem      two  FILE  uses,  in effect,  ATP sees:
Rem      RUNTP  debug 9  atpdir c:\data\  disk  dc75. -r  notab 1  nopisa 0  kol132 132
Rem      The final parameter pair  KOL132 132  is exceptional.  Unlike the
Rem      preceding two,  which end  DC75B.DAT,  this final one comes from a
Rem      " | "  request within  DC75A.DAT  (anything following the vertical
Rem      bar is to be moved to the far right).       WSM.  29 June 2015
Rem        About that  DEBUG 9  request within  DC75A.DAT,  understand that this
Rem        is too late to see all associated diagnostic printout.  To see all,
Rem        DEBUG  must be the first argument.  It is not here (here, FILE is the
Rem        first argument).  To see more in the  .DBG  file,  move  DEBUG 9 from
Rem        DC75A.DAT  to  RUN.BAT  as  RUN75.BAT  demonstrates.  This has:
Rem        CALL RUNTP6  DEBUG 9  FILE dc75a.dat  DC6.  FILE dc75b.dat
Rem    --------------  End comments about DC-75.  WSM.  1 July 2015
Rem      Next demonstrate  TEST  feature which tests various imagined command
Rem      lines without actually performing any normal ATP execution.  In effect,
Rem      TEST 9  is  DEBUG 9  followed by the special  TEST  request.  The input
Rem      data file,  named after  DISK  or  BOTH,  will contain the desired
Rem      series of imagined command lines.  Study  DC76.DAT  to understand more.
RENAME  GLITZ.ARG  *.XXX
CALL RUNTP6  TEST 9  DISK c:\data\Dc76.  dc76. -R
RENAME  GLITZ.XXX  *.ARG
Rem    Demonstrate that any added, optional parameter beyond the argument
Rem    limit of  RUNTP4  or  RUNTP6  or  RUNTP8  or  RUNTP  is ignored.  To show
Rem    this,  it is easiest to use the simplest of the four,  which is  RUNTP4.
Rem    Add arguments 5 through 7.  These can be arbitrary since ATP does not see
Rem    them.  MS-DOS  logic of batch files ignore any text to the right of the 4
Rem    declared arguments of  RUNTP4.  Thus the extra text has no effect:
CALL RUNTP4  DISK  c:\data\dcN1.  dcn1.  -R  arg5  arg6  arg7
CALL RUNTP4  DISK  c:\data\dcN2.  dcn2.  -R
CALL RUNTP4  DISK  c:\data\dcN3.  dcn3.  -R
CALL RUNTP4  DISK  c:\data\dcN4.  dcn4.  -R
CALL RUNTP4  DISK  c:\data\dcN5.  dcn5.  -R
CALL RUNTP4  DISK  c:\data\dcN6.  dcn6.  -R
Rem    Demonstrate the definition of every possible parameter as of 8 June 2014.
Rem    In fact, no value will change compared with  STARTUP  but there will be 1
Rem    added line for each at the top of the  .LIS  file.  This also provides an
Rem    illustration that MS-DOS  allows very long parameters.  Virtual blanks as
Rem    introduced by  DC-6  will be used: 
Rem  RUNTP4  DISK  DCN7.  S  -R NOTAB 1 UNIXON 0 L4BYTE 1 NOPISA 0 MSCSV 0 LIMCRD 5K MAXL31 400
Rem    The preceding with "#" replacing " " after "-R" was too long. As with the
Rem    maximum number of arguments (9), MS-DOS simply misbehaved.  In this case,
Rem    it simply truncated the command line after 63 bytes (all following "LIMC"
Rem    was lost).  Well,  if 6 ATP parameters were too long, try 5 (this works).
Rem  THL Note that 4 arguments are plenty long (45 bytes) for our use:
CALL RUNTP4  DISK  c:\data\dcN7.  dcn7.  -R#NOTAB#1#UNIXON#0#L4BYTE#1#NOPISA#0
Rem    Note about available parameter names.  The preceding mentioned 7.  That
Rem    was true until 1 September 2014 when 21 more were added.  The complete
Rem    list now is:  NOTAB   UNIXON  L4BYTE  NOPISA  MSCSV   LIMCRD  MAXL31 1-7
Rem          8-14 :  IPRSUP  IPRSPY  NMAUTO  NOCOMM  NORUN   KOMPAR  LU6VRT
Rem         15-21 :  LUNTEX  KOL132  JTURBO  JJEATS  NOCALC  KROSEC  MAXZNO
Rem         22-28 :  NSMTH   NOBLAN  KOLWID  KOLSEP  KINSEN  LEFT16  LIMPNL
Rem    Any reader who believes that others might usefully be added is advised
Rem    to make his feelings know.  Addition is easy.  WSM.
CALL RUNTP4  DISK  c:\data\dcN8.  dcn8.  -R
CALL RUNTP4  DISK  c:\data\dcN9.  dcn9.  -R
CALL RUNTP4  DISK  c:\data\dcN10.  dcn10.  -R
CALL RUNTP4  DISK  c:\data\dcN11.  dcn11.  -R
CALL RUNTP4  DISK  c:\data\dcN12.  dcn12.  -R
Rem  CALL RUNTP4  DISK  c:\data\dcN13.  S  -R
CALL RUNTP4  DISK  c:\data\dcN14.  dcn14.  -R
CALL RUNTP4  DISK  c:\data\dcN15.  dcn15.  -R
RENAME  ATPHPGL.004  DCN15.HPG
CALL RUNTP4  DISK  c:\data\dcN16.  dcn16.  -R
CALL RUNTP4  DISK  c:\data\dcN17.  dcn17.  -R
RENAME  ATPHPGL.013  dcN17.HPG
CALL RUNTP4  DISK  c:\data\dcN18.  dcn18.  -R
CALL RUNTP4  DISK  c:\data\dcN19.  dcn19.  -R
CALL RUNTP4  DISK  c:\data\dcN20.  dcn20.  -R
CALL RUNTP4  DISK  c:\data\dcN21.  dcn21.  -R
CALL RUNTP4  DISK  c:\data\dcN22.  dcn22.  -R
CALL RUNTP4  DISK  c:\data\dcN23.  dcn23.  -R
CALL RUNTP4  DISK  c:\data\dcN24.  dcn24.  -R
DEL  DCN25.001
DEL  DCN25.002
DEL  DCN25.003
DEL  DCN25.004
DEL  DCN25.005
SET INCLUDE=
SET RESISTOR=ONEHALF
CALL RUNTP4  DISK  c:\data\dcN25.  dcn25.  -R
SET RESISTOR=
CALL RUNTP4  DISK  c:\data\dcN26.  dcn26.  -R
CALL RUNTP4  DISK  c:\data\dcN27.  dcn27.  -R
DEL  8B1111*.PL4
CALL RUNTP4  DISK  c:\data\dcN28.  dcn28.  -R
Rem    The preceding  "S"  of argument 3 is just a convenience.  It means
Rem    repeat the name of argument 2.  But use is optional as this demonstrates:
CALL RUNTP4  DISK  c:\data\dcN29.  DCN29.  -R
Rem    ----  Use  RUNSPY.BAT  for separate execution of one final test case:
Rem  The following will confirm  FOURIER  output of  SPY PLOT  using  HP-GL.
Rem  1st, replace REAL*8 .pl4 file of DC-3 by the required REAL*4 copy:
COPY  dc3pl4.032  dc3.pl4
Rem  Next, erase the HP-GL output of the 1st FOURIER bar chart, which
Rem  is the 3rd screen plot (2 time plots of DC-3 precede 2 Fourier plots):
del  atphpgl.003
del  inclspy0.hpg
Rem  Just to be extra sure, let delete output files (next 3 lines).  The name
Rem  DC3PL4OP.DAT  will be found in  INCLSPY0.DAT  so the  .DBG  and  .LIS
Rem  files are parallel to this.  As for .TMP,  we hope there will be none:
del  dc3pl4op.dbg
del  dc3pl4op.lis
Rem      del  *.tmp  ----  No, save  DEBUGxxx.TMP
Rem  Finally,  execute local  INCLSPY0.DAT  using SPY.  For Salford ATP, this
Rem  will involve DBOS windows.  Since GNU ATP has none,  the best we can do
Rem  is to send screen output to the disk via an implied "DISK".  Do not try
Rem  to make this explicit, and possibly change to BOTH, since the form of the
Rem  execution command must have just  "SPY @filename"  between  RUNTP  and
Rem  any parameter redefinition (we use just NOPISA as illustration).  WSM. 
CALL RUNTP4  SPY @0  NOPISA 1
Rem  Note that the preceding  NOPISA = 1  is required because  DC3PL4.032
Rem  is old C-like not Pisa format.  This is added 1 January 2013.  Of course,
Rem  if we were to replace  DC3PL4.032  by a Pisa-format alternative,  then
Rem  the appropriate flag would be  NOPISA = 0.  WSM.
dir  atphpgl.003  
RENAME  atphpgl.003  inclspy0.hpg
time < blank  >> date.lis
Rem  DEL  ATPVIDEO.*
DEL  ATPPAPER.*
DEL  8B1111*.*
rem  DEL  DC*.DBG
Rem    DEL  *.TMP  ----  No, save  DEBUGxxx.TMP
SET CTRLBK=NOTRAP
CALL FCGNU
