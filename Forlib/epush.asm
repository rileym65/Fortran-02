#include    macros.inc
#include    bios.inc

            proc    epush

            sex     r7
            lda     r6
            stxd
            lda     r6
            stxd
            lda     r6
            stxd
            lda     r6
            stxd
            sex     r2
            sep     sret

            endp

