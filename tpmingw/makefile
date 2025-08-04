# File: makefile
# Purpose: compile and link atp
#
# to compile without dislin library, change the dislin.a library to
# dislin.a dummy library
#
#
# Change this line (and directories if necessary)
#
#
include makefile.paths

CC=$(MY_CC)
FOR=$(MY_FOR)
#
# OBJECTS is a customized list of modules that are being worked on
# and are present in the current directory

OBJECTS =dimdef.o \
	newmods.o \
	comtac.o \
	fgnmod.o \
	usernl.o \
	analyt.o \
	devt69.o \
	usrfun.o \
	hopcod.o \
	user10.o \
	userline.o \
	nlelem.o \
	user96.o crandom.o cmodel.o cfun.o \
	icdll_wrapper.o
#
#---------------------------------------------------
# windows NT
# ==================
CFLAGS = -O2
FFLAGS = -O2
INCFLAGS = -I..\include
IMAGE = my_tpgig.exe
#
# To link without DISLIN library, disable first following line and enable
# the second.
#
#
#
#LIBRARY = tpbig.a f:\\dislin32\\dismg.a -luser32 -lgdi32 -lopengl32
LIBRARY = tpbig.a dislin.a -luser32 -lgdi32 -lcomdlg32

all: $(IMAGE)

clean:
	del $(OBJECTS)

.PHONY: all clean

icdll_wrapper.o: ../src/icdll_wrapper.c
	$(CC) -c $(CFLAGS) $(INCFLAGS) $<

.f.o:
	$(FOR) -c $(FFLAGS) $<
.c.o:
	$(CC) -c $(CFLAGS) $(INCFLAGS) $<
$(IMAGE) : $(OBJECTS)
	$(FOR) -s -o $(IMAGE) $(OBJECTS) $(LIBRARY)
#
# WARNING:
# The <tab> as the first character signifies the action to
# take for a given dependancy.
# The back slash causes a continuation of the current line
# even if the line is commented out.
#
# end of makefile

