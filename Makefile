PROJECT = fortran
CC = gcc
DEFS = -Wall
INCS =
INCDIR =
LIBS =
LIBDIR =
OBJS = \
	addvariable.o \
	arrayref.o \
	asm.o \
	casm.o \
	cassign.o \
	ccall.o \
	ccommon.o \
	ccontinue.o \
	cdata.o \
	cdimension.o \
	cdo.o \
	cend.o \
	cendfile.o \
	cexpr.o \
	cformat.o \
	cif.o \
	clet.o \
	cgoto.o \
	copen.o \
	cpause.o \
	crewind.o \
	cstop.o \
	ctype.o \
	cwrite.o \
	compileline.o \
	findline.o \
	findvariable.o \
	getarg.o \
	getcommon.o \
	gethex.o \
	getnumber.o \
	getvariable.o \
	getvarname.o \
	library.o \
	main.o \
	match.o \
	nextstatement.o \
	optionfile.o \
	pass.o \
	prepass.o \
	processoption.o \
	showerror.o \
	startup.o \
	trim.o \
	validvar.o \
	vartype.o

$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

library.h: library.asm
	cat library.asm | ./mklibrary.pl > library.h

clean:
	-rm *.o
	-rm $(PROJECT)

addvariable.o:   header.h addvariable.c
arrayref.o:      header.h arrayref.c
asm.o:           header.h asm.c
casm.o:          header.h casm.c
cassign.o:       header.h cassign.c
ccall.o:         header.h ccall.c
ccontinue.o:     header.h ccontinue.c
cdata.o:         header.h cdata.c
cdimension.o:    header.h cdimension.c
cdo.o:           header.h cdo.c
cend.o:          header.h cend.c
cendfile.o:      header.h cendfile.c
cexpr.o:         header.h cexpr.c
cformat.o:       header.h cformat.c
cgoto.o:         header.h cgoto.c
cif.o:           header.h cif.c
ctype.o:         header.h ctype.c
clet.o:          header.h clet.c
ccommon.o:       header.h ccommon.c
compileline.o:   header.h compileline.c
copen.o:         header.h copen.c
cpause.o:        header.h cpause.c
crewind.o:       header.h crewind.c
cstop.o:         header.h cstop.c
cwrite.o:        header.h cwrite.c
findline.o:      header.h findline.c
findvariable.o:  header.h findvariable.c
getarg.o:        header.h getarg.c
getcommon.o:     header.h getcommon.c
gethex.o:        header.h gethex.c
getnumber.o:     header.h getnumber.c
getvariable.o:   header.h getvariable.c
getvarname.o:    header.h getvarname.c
library.o:       header.h library.h library.c
main.o:          header.h main.c
match.o:         header.h match.c
nextstatement.o: header.h nextstatement.c
optionfile.o:    header.h optionfile.c
pass.o:          header.h pass.c
prepass.o:       header.h prepass.c
processoption.o: header.h processoption.c
showerror.o:     header.h showerror.c
startup.o:       header.h startup.c
trim.o:          header.h trim.c
validvar.o:      header.h validvar.c
vartype.o:       header.h vartype.c
