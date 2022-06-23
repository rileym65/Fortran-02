#include    macros.inc
#include    bios.inc

            proc    lfsr_shift

            extrn   LFSR_

            ldi     LFSR_.1
            phi     r9
            ldi     LFSR_.0
            plo     r9
            inc     r9
            inc     r9
            inc     r9
            ldn     r9
            plo     re
            shr
            str     r2
            glo     re
            xor
            plo     re
            ldn     r2
            shr
            str     r2
            glo     re
            xor
            plo     re
            ldn     r2
            shr
            shr
            str     r2
            glo     re
            xor
            plo     re
            ldn     r2
            shr
            shr
            str     r2
            glo     re
            xor
            plo     re
            dec     r9
            dec     r9
            dec     r9
            ldn     r9
            shl
            shlc
            str     r2
            glo     re
            xor
            xri     1
            shr
            ldn     r9
            shrc
            str     r9
            inc     r9
            ldn     r9
            shrc
            str     r9
            inc     r9
            ldn     r9
            shrc
            str     r9
            inc     r9
            ldn     r9
            shrc
            str     r9
            dec     rc
            glo     rc
            lbnz    lfsr_shift
            sep     sret

            endp

