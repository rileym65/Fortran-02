#include    macros.inc
#include    bios.inc

            proc    vpush8p

            lda     r6
            phi     rf
            lda     r6
            plo     rf
            lda     rf
            plo     re
            lda     rf
            plo     rf
            glo     re
            phi     rf
            sex     r7
            ldi     0
            stxd
            stxd
            stxd
            lda     rf
            stxd
            sex     r2
            sep     sret

            endp

