#include    macros.inc
#include    bios.inc

            proc    ltfp

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
            lbz     false32
            ldn     r7
            shl
            lbdf    true32
            lbr     false32

            endp

