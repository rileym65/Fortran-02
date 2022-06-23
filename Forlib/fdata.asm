#include    macros.inc
#include    bios.inc

            proc    fdata

            lda     r6                 ; get size of next item
            phi     rc
            str     r2
            lda     r6
            plo     rc
            or
            lbz     fdata_rt           ; return if done
            plo     rc
            lda     r6                 ; get address for data
            phi     rf
            lda     r6
            plo     rf
fdata_lp:   lda     r6                 ; get next byte of data
            str     rf                 ; write to destination
            inc     rf
            dec     rc                 ; decrement count
            glo     rc                 ; see if done
            lbnz    fdata_lp           ; loop back if not
            ghi     rc                 ; check high byte as well
            lbnz    fdata_lp
            lbr     fdata              ; loop back for more data
fdata_rt:   sep     sret               ; return to caller

            endp
