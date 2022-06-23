#include    macros.inc
#include    bios.inc

            proc    ffread

            extrn   atof
            extrn   atoi32
            extrn   iobuffer
            extrn   scratch1_
            extrn   scratch2_

            ldi     iobuffer.1         ; setup buffer
            phi     rb
            ldi     iobuffer.0
            plo     rb
            ldi     0                  ; clear count
            str     rb
ffread_lp:  ldn     r9                 ; get next variable type
            lbnz    ffread_1           ; jump if not at end
            sep     sret               ; otherwise return to caller
ffread_1:   ldn     rb                 ; need to see if we have some input
            lbnz    ffread_2           ; jump if we do
ffread_1a:  ldi     '?'                ; show input prompt
            sep     scall
            dw      f_type
            ldi     iobuffer.1         ; setup buffer
            phi     rf
            ldi     iobuffer.0
            plo     rf
            sep     scall              ; get input from user
            dw      f_input
            sep     scall              ; output cr/lf
            dw      f_inmsg
            db      10,13,0
            ldi     iobuffer.1         ; point to input
            phi     rb
            ldi     iobuffer.0
            plo     rb
ffread_2:   lda     rb                 ; move past any spaces
            lbz     ffread_1a          ; jump if ran out of input
            smi     ' '                ; is character a space
            lbz     ffread_2           ; loop until no more spaces
            dec     rb                 ; move back to non-space
            ldi     scratch1_.1        ; point to scratch buffer
            phi     rf
            ldi     scratch1_.0
            plo     rf
ffread_3:   lda     rb                 ; read byte from input
            str     rf                 ; write to scratch buffer
            inc     rf
            lbz     ffread_40          ; jump if terminator written
            smi     ' '                ; check for space
            lbnz    ffread_3           ; loop until space or terminator
            dec     rf                 ; change space to terminator
            str     rf
            skp
ffread_40:  dec     rb                 ; move back to terminator
ffread_4:   ldi     scratch1_.1        ; setup buffers for conversion
            phi     rf
            ldi     scratch1_.0
            plo     rf
            ldi     scratch2_.1
            phi     rd
            ldi     scratch2_.0
            plo     rd
            ldn     r9                 ; get variable type
            smi     'L'                ; check for logical
            lbz     ffread_l           ; jump if so
            ghi     r7                 ; save important registers
            stxd
            glo     r7
            stxd
            ghi     r9
            stxd
            glo     r9
            stxd
            ghi     rb
            stxd
            glo     rb
            stxd
            ldn     r9                 ; get variable type
            smi     'R'                ; is it a real
            lbz     ffread_r           ; jump if so
            sep     scall              ; otherwise convert integer
            dw      atoi32
            lbr     ffread_5           ; then move past real conversion
ffread_r:   sep     scall              ; convert real
            dw      atof
ffread_5:   irx                        ; recover consumed registers
            ldxa
            plo     rb
            ldxa
            phi     rb
            ldxa
            plo     r9
            ldxa
            phi     r9
            ldxa
            plo     r7
            ldx
            phi     r7
            lda     r9                 ; get variable type
            plo     re                 ; set aside for a moment
            lda     r9                 ; get variable address
            phi     rd
            lda     r9
            plo     rd
            ldi     scratch2_.1        ; point to converted data
            phi     rf
            ldi     scratch2_.0
            plo     rf
            glo     re                 ; recover type
            smi     'B'                ; is it a byte
            lbz     ffread_b           ; jump if so
            glo     re                 ; recover type
            smi     'S'                ; is it a short
            lbz     ffread_s           ; jump if so
            ldi     4                  ; need to copy 4 bytes
            plo     rc
ffread_cp:  lda     rf                 ; read byte from conversion
            str     rd                 ; write to variable
            inc     rd
            dec     rc                 ; decrement count
            glo     rc
            lbnz    ffread_cp          ; loop back if not done
            lbr     ffread_lp          ; loop back for next variable
ffread_s:   inc     rf                 ; point to lower two bytes
            inc     rf
            lda     rf                 ; copy low 2 bytes to variable
            str     rd
            inc     rd
            lda     rf
            str     rd
            lbr     ffread_lp          ; loop back for next variable
ffread_b:   inc     rf                 ; move to LSB
            inc     rf
            inc     rf
            ldn     rf                 ; retrieve it
            str     rd                 ; and store to variable
            lbr     ffread_lp          ; loop back for next variable
ffread_l:   inc     r9                 ; get variable address
            lda     r9
            phi     rd
            lda     r9
            plo     rd
            ldi     0                  ; set it initially to false
            str     rd
ffread_la:  lda     rf                 ; read byte from input
            lbz     ffread_lp          ; loop to next variable if done
            smi     'T'                ; check for T
            lbz     ffread_t           ; jump if so
            smi     32                 ; check for t
            lbnz    ffread_la          ; jump if not
ffread_t:   ldi     0ffh               ; set variable to true
            str     rd
            lbr     ffread_lp          ; then to next variable

            endp

