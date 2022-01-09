# Fortran-02
Fortran compiler that generates CDP1802 code

This is a VERY preliminary version of my Fortran/02 compiler.  This is the
next entry in my /02 series of CDP1802 development tools.

Fortran/02 like Basic/02 is meant to run on a PC but can generate code for
a wide variety of 1802-based machines.

At this point it can compile some very basic Fortran programs.  Look at the
fortran.txt to see what is supported and what is still planned

Here is a sample program that can be compiled with this version of the
compiler:

      integer f,c

      do 100 f=0,212
      c=(f-32)*5./9.
      write(0,10)f,c
10    format('F=',i3,' =',2x,'C=',i3)
100   continue
      stop
      end

