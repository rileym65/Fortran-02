#include    macros.inc
#include    bios.inc

            proc    fenter

            irx                        ; recover return addresses
            ldxa                       ; and set them aside
            phi     r8
            ldxa
            plo     r8
            ldxa
            phi     r9
            ldxa
            plo     r9
            ldx                        ; retrieve passed argument count
            plo     ra                 ; and put it here
fenter_lp:  glo     ra                 ; get passedcount
            lbz     fenter_tf          ; jump if too few arguments
            lda     r6                 ; retrieve next address
            phi     rf
            str     r2
            lda     r6
            plo     rf
            or                         ; see if end of list
            lbz     fenter_tm          ; jump if too many arguments passed
            irx
            ldxa                       ; get next address
            str     rf                 ; and write to local var
            inc     rf
            ldx
            str     rf
            dec     ra                 ; decrement passed argument count
            lbr     fenter_lp          ; loop for next argument
fenter_tf:  lda     r6                 ; need to read rest of local table
            str     r2
            lda     r6
            or
            lbnz    fenter_tf          ; loop until done
            lbr     fenter_dn
fenter_tm:  glo     ra                 ; see if done
            lbz     fenter_dn          ; jump if so
            irx                        ; take two bytes off stack
            irx
            dec     ra                 ; decrement argument count
            lbr     fenter_tm          ; loop until done
fenter_dn:  glo     r9                 ; restore stack entries
            stxd
            ghi     r9
            stxd
            glo     r8
            stxd
            ghi     r8
            stxd
            sep     sret               ; and return to caller

            endp

