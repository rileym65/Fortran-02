#include    macros.inc
#include    bios.inc

            proc    nextvar

            extrn   next

            irx          ; move to varAddr
            ldxa
            plo     rd
            ldxa
            phi     rd
            irx
            irx
            glo     rc   ; check for correct address
            sm           ; against stack
            lbnz    nv1  ; jump if not
            irx          ; move to msb
            ghi     rc
            sm           ; compare
            lbnz    nv2  ; jump if not
            dec     r2   ; entry found, move back
            dec     r2
            dec     r2
            dec     r2
            dec     r2
            dec     r2
            lbr     next
nv1:        irx          ; move past bad entry
nv2:        irx          ; move past bad entry
            irx
            irx
            irx
            ghi     rd
            stxd
            glo     rd
            stxd
            lbr     nextvar

            endp

