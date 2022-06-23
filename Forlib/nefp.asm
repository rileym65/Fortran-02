#include    macros.inc
#include    bios.inc

            proc    nefp

            extrn   false32
            extrn   subfp
            extrn   true32

            sep     scall
            dw      subfp
            sex     r7
            irx
            ldxa
            or
            ldxa
            or
            ldxa
            or
            ldx
            or
            sex     r2
            lbnz    true32
            lbr     false32

            endp

