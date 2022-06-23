#include    macros.inc
#include    bios.inc

            proc    rnd16

            extrn   LFSR_
            extrn   lfsr_shift
            extrn   mod16

            ldi     16
            plo     rc
            sep     scall         ; Shift the register
            dw      lfsr_shift
            ldi     LFSR_.1
            phi     r9
            ldi     LFSR_.0
            plo     r9
            lda     r9
            plo     rf
            ldn     r9
            phi     rf
            inc     r7
            lda     r7
            plo     r9
            ldn     r7
            phi     r9
            ghi     rf
            ani     07fh
            str     r7
            dec     r7
            glo     rf
            str     r7
            dec     r7
            ghi     r9
            str     r7
            dec     r7
            glo     r9
            str     r7
            dec     r7
            lbr     mod16

            endp

