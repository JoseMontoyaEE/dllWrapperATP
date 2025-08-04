if exist newmods.o del newmods.o
if exist newmods.f del newmods.f
if exist dimdef.f  del dimdef.f
if exist dimdef.o  del dimdef.o
if exist comtac.o  del comtac.o
vardim <%1
rename  newmods.for  *.f
rename  dimdef.for  *.f
dir  newmods.*
dir  dimdef.*
