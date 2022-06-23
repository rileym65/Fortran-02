#include    macros.inc
#include    bios.inc

            proc    fmtread

            extrn   atof
            extrn   atoi32
            extrn   fread
            extrn   iobuffer
            extrn   isnumeral
            extrn   scratch1_
            extrn   scratch2_

            ghi     ra                 ; get lun
            xri     5                  ; terminal is LUN 5
            lbz     fmtread_0          ; jump if from terminal
            xri     5                  ; restore LUN
            ldi     iobuffer.1
            phi     rf
            ldi     iobuffer.0
            plo     rf
            sep     scall              ; othereise read from disk
            dw      fread
            lbdf    fmtread_er         ; jump on file error
            lbnz    fmtread_st         ; jump if bytes read
fmtread_er: sep     sret               ; return to caller
fmtread_0:  sep     scall              ; display prompt
            dw      f_inmsg
            db      '? ',0
            ldi     iobuffer.1         ; point to io buffer
            phi     rf
            ldi     iobuffer.0
            plo     rf
            sep     scall              ; read line from terminal
            dw      f_input
            sep     scall              ; output a cr/lf
            dw      f_inmsg
            db      10,13,0
fmtread_st: ldi     iobuffer.1         ; point to io buffer
            phi     rb
            ldi     iobuffer.0
            plo     rb
fmtread_lp: lda     r8                 ; get count for next format item
            lbz     fmtrd_dn           ; jump if no more items
            plo     ra
            ldn     r8                 ; get format type
            smi     'X'                ; check for X
            lbz     fmtrd_x            ; jump if so
fmtr_loop:  inc     r8                 ; get size of field
            ldn     r8
            plo     rc                 ; place into count
            phi     rc                 ; keep a copy
            dec     r8
            ldi     scratch1_.1        ; where to copy
            phi     rf
            ldi     scratch1_.0
            plo     rf
fmtrd_c_1:  lda     rb                 ; copy bytes from input
            str     rf
            inc     rf
            dec     rc
            glo     rc
            lbnz    fmtrd_c_1
            ldi     0                  ; terminate
            str     rf
            ghi     rc                 ; reset size
            plo     rc
            ldi     scratch1_.1        ; Point RF back to here
            phi     rf
            ldi     scratch1_.0
            plo     rf
            ldn     r8                 ; recover type
            smi     'I'                ; check for integer
            lbz     fmtrd_i            ; jump if so
            ldn     r8                 ; recover type
            smi     'L'                ; check for logical
            lbz     fmtrd_l            ; jump if so
            ldn     r8                 ; recover type
            smi     'F'                ; check for real
            lbz     fmtrd_r            ; jump if so
            ldi     0x22               ; error code
            smi     0                  ; invalid type, so error
            sep     sret               ; return to caller
fmtrd_x:    glo     ra                 ; get item count
            str     r2                 ; store for add
            glo     rb                 ; add to buffer position
            add
            plo     rb
            ghi     rb
            adci    0
            phi     rb
            inc     r8                 ; move to next format item
            inc     r8
            inc     r8
            lbr     fmtread_lp         ; loop for next item

fmtrd_l:    ldi     0                  ; set field initially to false
            phi     rc
fmtrd_l_1:  lda     rf                 ; get byte from input
            smi     'T'                ; check for true
            lbz     fmtrd_l_t          ; jump if so
            smi     32                 ; lowercase t
            lbz     fmtrd_l_t          ; jump if so
fmtrd_l_2:  dec     rc                 ; decrement width count
            glo     rc                 ; see if done
            lbnz    fmtrd_l_1          ; loop back if not
            lda     r9                 ; get next variable type
            lbz     fmtrd_dn           ; jump if no more variables
            plo     re                 ; save variable type
            lda     r9                 ; retrieve address
            phi     rd
            lda     r9
            plo     rd
            glo     re                 ; get variable type
            smi     'B'                ; check for byte
            lbz     fmtrd_l_b          ; jump if so
            glo     re                 ; get variable type
            smi     'L'                ; check for logical
            lbz     fmtrd_l_b          ; jump if so
            glo     re                 ; get variable type
            smi     'S'                ; check for short
            lbz     fmtrd_l_s          ; jump if so
            ldi     4                  ; 4 bytes to set
            lskp
fmtrd_l_s:  ldi     2
            lskp
fmtrd_l_b:  ldi     1
            plo     rf                 ; set counter
fmtrd_l_3:  ghi     rc                 ; get read value
            str     rd                 ; write into variable
            inc     rd
            dec     rf                 ; decrement count
            glo     rf                 ; see if done
            lbnz    fmtrd_l_3          ; loop back if not
            lbr     fmtrd_nx           ; otherwise on to the next
fmtrd_l_t:  ldi     0ffh               ; mark as true
            phi     rc
            lbr     fmtrd_l_2          ; finish checking field

fmtrd_i:    lda     rf                 ; get byte from input
            sep     scall              ; is it a numeral
            dw      isnumeral
            lbdf    fmtrd_i_1          ; jump if so
            dec     rf                 ; replace with '0'
            ldi     '0'
            str     rf
            inc     rf
fmtrd_i_1:  dec     rc                 ; decrement width
            glo     rc                 ; see if done
            lbnz    fmtrd_i            ; loop back if not
            ldi     scratch1_.1        ; back to field data
            phi     rf
            ldi     scratch1_.0
            plo     rf
fmtrd_i_2:  lda     rf                 ; move past any spaces
            lbz     fmtrd_i_3          ; jump if terminator hit
            smi     ' '                ; check for space
            lbz     fmtrd_i_2          ; loop until no more spaces
            dec     rf                 ; back to nonspace
fmtrd_i_3:  ldi     scratch2_.1        ; where to put conversion
            phi     rd
            ldi     scratch2_.0
            plo     rd
            ghi     r7                 ; save important registers
            stxd
            glo     r7
            stxd
            ghi     r8
            stxd
            glo     r8
            stxd
            ghi     r9
            stxd
            glo     r9
            stxd
            ghi     ra
            stxd
            glo     ra
            stxd
            ghi     rb
            stxd
            glo     rb
            stxd
            sep     scall              ; convert ascii to integer
            dw      atoi32
            irx                        ; recover important registers
            ldxa
            plo     rb
            ldxa
            phi     rb
            ldxa
            plo     ra
            ldxa
            phi     ra
            ldxa
            plo     r9
            ldxa
            phi     r9
            ldxa
            plo     r8
            ldxa
            phi     r8
            ldxa
            plo     r7
            ldx
            phi     r7
            lda     r9                 ; get next variable type
            plo     re                 ; set aside
            lda     r9                 ; get data address
            phi     rd
            lda     r9
            plo     rd
            ldi     scratch2_.1        ; where to number
            phi     rf
            ldi     scratch2_.0
            plo     rf
            glo     re                 ; recover variable type
            smi     'B'                ; is it a byte
            lbz     fmtrd_i_b          ; jump if so
            glo     re                 ; recover variable type
            smi     'L'                ; is it a logical
            lbz     fmtrd_i_b          ; jump if so
            glo     re                 ; recover variable type
            smi     'S'                ; is it a short
            lbz     fmtrd_i_s          ; jump if so
            lda     rf                 ; copy converted number
            str     rd
            inc     rd
            lda     rf
            str     rd
            inc     rd
fmtrd_i_c2: lda     rf
            str     rd
            inc     rd
fmtrd_i_c1: lda     rf
            str     rd
            lbr     fmtrd_nx           ; process end of entry
fmtrd_i_s:  inc     rf                 ; skip high 2 bytes
            inc     rf
            lbr     fmtrd_i_c2         ; and copy 2 bytes
fmtrd_i_b:  inc     rf                 ; skip high 3 bytes
            inc     rf
            inc     rf
            lda     rf                 ; and copy final byte
            str     rd
fmtrd_r:    ldi     0                  ; indicate no period found
            phi     rc
fmtrd_ra:   lda     rf                 ; get byte from input
            lbz     fmtrd_r0           ; jump if terminator found
            sep     scall              ; is it a numeral
            dw      isnumeral
            plo     re                 ; save character
            lbdf    fmtrd_r_1          ; jump if so
            smi     '-'                ; check for minus sign
            lbz     fmtrd_r_1          ; jump if minus
            glo     re                 ; test for +
            smi     '+'
            lbz     fmtrd_r_1
            glo     re                 ; test for E
            smi     'E'
            lbz     fmtrd_r_1
            smi     32                 ; test for e
            lbz     fmtrd_r_1
            smi     '.'                ; check for period
            lbnz    fmtrd_r_1          ; jump if not
            ldi     0xff               ; indicate period found
            phi     rc
            dec     rf                 ; replace with '0'
            ldi     '0'
            skp
            str     rf
            inc     rf
fmtrd_r_1:  dec     rc                 ; decrement width
            glo     rc                 ; see if done
            lbnz    fmtrd_ra           ; loop back if not
fmtrd_r_0b: ldi     scratch1_.1        ; back to field data
            phi     rf
            ldi     scratch1_.0
            plo     rf
            ghi     rc                 ; was a decimal encountered
            lbnz    fmtrd_r_2          ; jump if so
fmtrd_r_x1: lda     rf                 ; find terminator
            lbnz    fmtrd_r_x1
            inc     r8                 ; get decimal size
            inc     r8
            ldn     r8
            plo     rc
            dec     r8
            dec     r8
            ldi     0                  ; write new terminator
            str     rf
fmtrd_r_x2: dec     rf                 ; move to prior character
            glo     rc                 ; done moving characters?
            lbz     fmtrd_r_x3         ; jump if so
            dec     rf                 ; move prior character
            lda     rf
            str     rf
            dec     rc                 ; decrement count
            lbr     fmtrd_r_x2         ; loop until done
fmtrd_r_x3: ldi     '.'                ; put in decimal point
            str     rf
            ldi     scratch1_.1        ; back to field data
            phi     rf
            ldi     scratch1_.0
            plo     rf
fmtrd_r_2:  lda     rf                 ; move past any spaces
            lbz     fmtrd_r_3          ; jump if terminator hit
            smi     ' '                ; check for space
            lbz     fmtrd_r_2          ; loop until no more spaces
            dec     rf                 ; back to nonspace
fmtrd_r_3:  ldi     scratch2_.1        ; where to put conversion
            phi     rd
            ldi     scratch2_.0
            plo     rd
            ghi     r7                 ; save important registers
            stxd
            glo     r7
            stxd
            ghi     r8
            stxd
            glo     r8
            stxd
            ghi     r9
            stxd
            glo     r9
            stxd
            ghi     ra
            stxd
            glo     ra
            stxd
            ghi     rb
            stxd
            glo     rb
            stxd
            sep     scall              ; convert ascii to integer
            dw      atof
            irx                        ; recover important registers
            ldxa
            plo     rb
            ldxa
            phi     rb
            ldxa
            plo     ra
            ldxa
            phi     ra
            ldxa
            plo     r9
            ldxa
            phi     r9
            ldxa
            plo     r8
            ldxa
            phi     r8
            ldxa
            plo     r7
            ldx
            phi     r7
            lda     r9                 ; get next variable type
            lbz     fmtrd_dn
            plo     re                 ; set aside
            lda     r9                 ; get data address
            phi     rd
            lda     r9
            plo     rd
            ldi     scratch2_.1        ; where to number
            phi     rf
            ldi     scratch2_.0
            plo     rf
            glo     re                 ; recover variable type
            smi     'B'                ; is it a byte
            lbz     fmtrd_r_b          ; jump if so
            glo     re                 ; recover variable type
            smi     'L'                ; is it a logical
            lbz     fmtrd_r_b          ; jump if so
            glo     re                 ; recover variable type
            smi     'S'                ; is it a short
            lbz     fmtrd_r_s          ; jump if so
            lda     rf                 ; copy converted number
            str     rd
            inc     rd
            lda     rf
            str     rd
            inc     rd
fmtrd_r_c2: lda     rf
            str     rd
            inc     rd
fmtrd_r_c1: lda     rf
            str     rd
            lbr     fmtrd_nx           ; process end of entry
fmtrd_r_s:  inc     rf                 ; skip high 2 bytes
            inc     rf
            lbr     fmtrd_r_c2         ; and copy 2 bytes
fmtrd_r_b:  inc     rf                 ; skip high 3 bytes
            inc     rf
            inc     rf
            lda     rf                 ; and copy final byte
            str     rd
fmtrd_r0:   dec     rf                 ; move back to terminator
fmtrd_r0c:  glo     rc                 ; see if done with field
            lbz     fmtrd_r_0a         ; jump if so
            ldi     '0'                ; write a zero
            str     rf
            inc     rf
            dec     rc
            lbr     fmtrd_r0c          ; loop until full field set
fmtrd_r_0a: ldi     0                  ; write terminator
            str     rf
            lbr     fmtrd_r_0b



fmtrd_nx:   dec     ra                 ; see if done with specifier
            glo     ra
            lbnz    fmtr_loop          ; jump if not, process next var
            inc     r8                 ; move to next specifier
            inc     r8
            inc     r8
            lbr     fmtread_lp         ; and process it

fmtrd_dn:   ldi     1                  ; signal not eof
            adi     0                  ; clear DF, no errors
            sep     sret               ; return to caller

            endp

