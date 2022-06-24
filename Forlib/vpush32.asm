#include    macros.inc
#include    bios.inc

            proc    vpush32

            lda     r6
            phi     rf
            lda     r6
            plo     rf
            sex     r7
            lda     rf
            stxd
            lda     rf
            stxd
            lda     rf
            stxd
            lda     rf
            stxd
            sex     r2
            sep     sret

            endp

