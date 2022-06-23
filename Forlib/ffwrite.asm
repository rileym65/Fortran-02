#include    macros.inc
#include    bios.inc

            proc    ffwrite

            extrn   ftoa
            extrn   iobuffer
            extrn   itoa32
            extrn   scratch1_
            extrn   scratch2_

            ldi     iobuffer.1         ; setup output buffer
            phi     rd
            ldi     iobuffer.0
            plo     rd
ffwrite_lp: lda     r9                 ; get next variable type
            lbz     ffwrite_dn         ; jump if no more
            plo     re                 ; set aside for a moment
            smi     34                 ; check for quoted text
            lbz     ffwrite_qt         ; jump if quoted value
            lda     r9                 ; get variable address
            phi     rf
            lda     r9
            plo     rf
            glo     re                 ; check for pointer type
            shl
            lbnf    ffwrite_x          ; jump if not
            lda     rf                 ; retrieve pointed to address
            str     r2
            lda     rf
            plo     rf
            ldn     r2
            phi     rf
            glo     re                 ; clear pointer bit
            ani     07fh
            plo     re
ffwrite_x:  glo     re                 ; recover variable type
            smi     'B'                ; check for byte
            lbz     ffwrite_b          ; jump if so
            glo     re                 ; recover variable type
            smi     'L'                ; check for logical
            lbz     ffwrite_l          ; jump if so
            glo     re                 ; recover variable type
            smi     'S'                ; check for short
            lbz     ffwrite_s          ; jump if so
            glo     re                 ; recover variable type
            smi     'I'                ; check for integer
            lbz     ffwrite_i          ; jump if so
            glo     re                 ; recover variable type
            smi     'R'                ; check for real
            lbz     ffwrite_r          ; jump if so
            lbr     ffwrite_dn         ; anything else terminates write
ffwrite_qt: lda     r9                 ; get byte from list
            lbz     ffwrite_lp         ; jump if end found
            str     rd                 ; store into output
            inc     rd
            lbr     ffwrite_qt
ffwrite_l:  ldn     rf                 ; get variable vlue
            lbnz    ffwrite_lt         ; jump if true
            ldi     'F'                ; need to output false
            lskp
ffwrite_lt: ldi     'T'                ; need to output true
            str     rd                 ; store into output
            inc     rd
ffwrite_sp: ldi     ' '                ; followed by a space
            str     rd
;            inc     rd
            lbr     ffwrite_lp         ; loop back for next variable
ffwrite_i:  ldi     (scratch2_+3).1    ; need to reverse the bytes
            phi     rb
            ldi     (scratch2_+3).0
            plo     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            ldi     scratch2_.1        ; point to data
            phi     rf
            ldi     scratch2_.0
            plo     rf
            ghi     r7                 ; save consumed variables
            stxd
            glo     r7
            stxd
            ghi     r9
            stxd
            glo     r9
            stxd
            ghi     ra
            stxd
            glo     ra
            stxd
            sep     scall              ; convert integer value
            dw      itoa32
ffwrite_r2: irx                        ; recover consumed registers
            ldxa
            plo     ra
            ldxa
            phi     ra
            ldxa
            plo     r9
            ldxa
            phi     r9
            ldxa
            plo     r7
            ldx
            phi     r7
            lbr     ffwrite_sp         ; add space, then next variable
ffwrite_s:  ldi     scratch1_.1        ; need to convert to 32 bits
            phi     rb
            ldi     scratch1_.0
            plo     rb
            ldi     0                  ; need two zeroes
            str     rb
            inc     rb
            str     rb
            inc     rb
            lda     rf                 ; then the two bytes from the variable
            str     rb
            inc     rb
            lda     rf
            str     rb
            ldi     scratch1_.1        ; now point rf to 32-bit value
            phi     rf
            ldi     scratch1_.0
            plo     rf
            lbr     ffwrite_i          ; now process as an integer
ffwrite_b:  ldi     scratch1_.1        ; need to convert to 32 bits
            phi     rb
            ldi     scratch1_.0
            plo     rb
            ldi     0                  ; need three zeroes
            str     rb
            inc     rb
            str     rb
            inc     rb
            str     rb
            inc     rb
            lda     rf                 ; then tthi byte from the variable
            str     rb
            ldi     scratch1_.1        ; now point rf to 32-bit value
            phi     rf
            ldi     scratch1_.0
            plo     rf
            lbr     ffwrite_i          ; now process as an integer
ffwrite_r:  ldi     (scratch2_+3).1    ; need to reverse the bytes
            phi     rb
            ldi     (scratch2_+3).0
            plo     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            dec     rb
            lda     rf
            str     rb
            ldi     scratch2_.1        ; point to data
            phi     rf
            ldi     scratch2_.0
            plo     rf
            ghi     r7                 ; save consumed variables
            stxd
            glo     r7
            stxd
            ghi     r9
            stxd
            glo     r9
            stxd
            ghi     ra
            stxd
            glo     ra
            stxd
            sep     scall              ; convert real value
            dw      ftoa
            lbr     ffwrite_r2         ; and finish up
ffwrite_dn: ldi     0                  ; write a terminator
            str     rd
            ldi     iobuffer.1         ; setup buffer for output
            phi     rf
            ldi     iobuffer.0
            plo     rf
            ghi     ra                 ; get LUN
            smi     6                  ; check for LUN6
            lbnz    ffwrite_d1         ; jump if not
            lda     rf                 ; get first byte
            smi     '+'                ; check for no advance
            lbz     ffwrite_d2         ; jump if no advance
            sep     scall              ; display cr/lf
            dw      f_inmsg
            db      10,13,0
ffwrite_d2: sep     scall              ; display it
            dw      f_msg
            sep     sret               ; return to caller
ffwrite_d1: sep     scall              ; display it
            dw      f_msg
            sep     scall              ; display cr/lf
            dw      f_inmsg
            db      10,13,0
            sep     sret               ; then return to caller

            endp

