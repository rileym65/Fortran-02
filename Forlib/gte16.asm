#include    macros.inc
#include    bios.inc

            proc    gte16

            extrn   false16
            extrn   sub16
            extrn   true16

            sep     scall
            dw      sub16
            sex     r7
            irx
            ldxa
            or
            sex     r2
            lbz     true16
            ldn     r7
            shl
            lbnf    true16
            lbr     false16

            endp

