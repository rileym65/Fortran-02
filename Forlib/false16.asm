#include    macros.inc
#include    bios.inc

            proc    false16

            ldi     000h
            sex     r7
            stxd
            stxd
            sex     r2
            sep     sret

            endp

