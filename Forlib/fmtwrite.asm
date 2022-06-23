#include      macros.inc
#include      bios.inc

; ****************************************
; ***** Formatted write              *****
; ***** R8 - Pointer to format list  *****
; ***** R9 - Pointer to data list    *****
; ***** RA.1 LUN                     *****
; ***** data list format:            *****
; *****      1-byte data type        *****
; *****      2-bytes address of data *****
; ***** format list format:          *****
; *****      1-byte count            *****
; *****      1-byte format type      *****
; *****      1-byte size 1           *****
; *****      1-byte size 2           *****
; ***** Both lists end with a 0 in   *****
; ***** first byte for entry         *****
; ***** Uses: RA.0 - remaining items *****
; *****       RB   - output buffer   *****
; ****************************************
              proc    fmtwrite

              extrn   fromsci
              extrn   ftoa
              extrn   iobuffer
              extrn   itoa32
              extrn   scratch1_
              extrn   scratch2_
              extrn   writerec

              ldi     iobuffer.1       ; setup output buffer
              phi     rb
              ldi     iobuffer.0
              plo     rb
              ghi     r8               ; save position of format list
              stxd  
              glo     r8
              stxd  
              lbr     checkfmt         ; check format
fmtwrite_lp:  lda     r9               ; get next variable type
              lbz     fmtwrite_dn2     ; jump if no more variables
              plo     re               ; save type
              lda     r9               ; retrieve data address
              phi     rf
              lda     r9
              plo     rf
              glo     re               ; recover type
              shl                      ; see if high bit is set
              lbnf    fmtwrite_1       ; jump if not
              shr                      ; strip high bit
              plo     re               ; put back in re
              lda     rf               ; retrieve pointed to address
              str     r2               ; set aside
              ldn     rf               ; read low byte
              plo     rf
              ldx                      ; recover high byte
              phi     rf
fmtwrite_1:   ldn     r8               ; get conversion type
              smi     'A'              ; check for A conversion
              lbz     fmtwrt_ca        ; jump if so
              glo     re               ; recover variable type
              smi     'I'              ; is it an integer
              lbz     fmtwrt_i         ; jump if so
              glo     re
              smi     'R'              ; is it a real
              lbz     fmtwrt_r         ; jump if so
              glo     re
              smi     'S'              ; is it a short
              lbz     fmtwrt_i2        ; jump if so
              glo     re
              smi     'B'              ; is it a byte
              lbz     fmtwrt_i1        ; jump if so
              glo     re
              smi     'L'              ; is it a logical
              lbz     fmtwrt_i1        ; jump if so
              lbr     fmtwrite_dn2     ; consider done if unknown type
fmtwrt_i1:    ldi     0                ; high order 3 bytes are zero
              sex     r7               ; point x to expr stack
              stxd
              stxd
              stxd
              lbr     fmtwrt_c1        ; then copy 1 byte
fmtwrt_i2:    ldi     0                ; high order 2 bytes are zero
              sex     r7               ; point x to expr stack
              stxd
              stxd
              lbr     fmtwrt_c2        ; then 2 bytes from variable
fmtwrt_i:     sex     r7               ; point x to expr stack
              lda     rf               ; copy variable bytes to expr stack
              stxd
              lda     rf
              stxd
fmtwrt_c2:    lda     rf
              stxd
fmtwrt_c1:    lda     rf
              stxd
              sex     r2               ; point x back to system stack
              ldi     scratch1_.1      ; where to put conversion
              phi     rd
              ldi     scratch1_.0
              plo     rd
              ghi     r8               ; save consumed registers
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
              ghi     r7               ; point to data item
              phi     rf
              glo     r7
              plo     rf
              inc     rf
              sep     scall            ; convert integer to ascii
              dw      itoa32
              irx                      ; recover consumed registers
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
              ldx
              phi     r8
              inc     r7               ; remove item from expr stack
              inc     r7
              inc     r7
              inc     r7
              lbr     fmtwrt_go        ; now process ascii number
fmtwrt_r:     sex     r7               ; point x to expr stack
              lda     rf               ; copy variable bytes to expr stack
              stxd
              lda     rf
              stxd
              lda     rf
              stxd
              lda     rf
              stxd
              sex     r2               ; point x back to system stack
              ldi     scratch2_.1      ; where to put conversion
              phi     rd
              ldi     scratch2_.0
              plo     rd
              ghi     r8               ; save consumed registers
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
              ghi     r7               ; point to data item
              phi     rf
              glo     r7
              plo     rf
              inc     rf
              sep     scall            ; convert real to ascii
              dw      ftoa
              irx                      ; recover consumed registers
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
              ldx
              phi     r8
              inc     r7               ; remove item from expr stack
              inc     r7
              inc     r7
              inc     r7
              ldi     scratch2_.1      ; where to put conversion
              phi     rf
              ldi     scratch2_.0
              plo     rf
              ldi     scratch1_.1      ; where to put conversion
              phi     rd
              ldi     scratch1_.0
              plo     rd
              sep     scall            ; convert from scientific, if needed
              dw      fromsci
fmtwrt_go:    ldi     0                ; clear count
              plo     rc
              ldi     scratch1_.1      ; point to conversion
              phi     rf
              ldi     scratch1_.0
              plo     rf
              ldn     r8               ; get current specifier
              smi     'I'              ; is it I conversion
              lbz     fmtwrt_ci        ; jump if so
              ldn     r8               ; check for F conversion
              smi     'F'
              lbz     fmtwrt_cf        ; jump if so
              ldn     r8               ; check for L conversion
              smi     'L'
              lbz     fmtwrt_cl        ; jump if so
              lbr     fmtwrite_dn2     ; anything else aborts
; ************************
; ***** A Conversion *****
; ************************
fmtwrt_ca:    glo     re               ; is it an integer
              smi     'I'
              lbz     fmtwrt_ca4       ; jump if so
              glo     re               ; is it a real
              smi     'R'
              lbz     fmtwrt_ca4       ; jump if so
              glo     re               ; is it a short
              smi     'S'
              lbz     fmtwrt_ca2       ; jump if so
              ldi     1                ; one character
              lskp
fmtwrt_ca2:   ldi     2                ; two characters
              lskp
fmtwrt_ca4:   ldi     4                ; four characters
              phi     rc               ; store count
              ldi     0                ; clear width
              plo     rc
              ldi     scratch1_.1      ; point to conversion buffer
              phi     rd
              ldi     scratch1_.0
              plo     rd
fmtwrt_calp:  lda     rf               ; get byte from variable
              lbz     fmtwrt_caz       ; jump if zero
              str     rd               ; store into output
              inc     rd
              inc     rc               ; increment size
fmtwrt_caz:   ghi     rc               ; get count
              smi     1                ; decrement it
              phi     rc               ; put it back
              lbnz    fmtwrt_calp      ; loop until all bytes copied
              ldi     0                ; terminate string
              str     rd
              lbr     fmtwrt_cp        ; and then transfer to output


; ************************
; ***** I Conversion *****
; ************************
fmtwrt_ci:    lda     rf               ; get byte from conversion
              lbz     fmtwrt_cp        ; done with check
              smi     '.'              ; was a decimal encountered
              lbz     fmtwrt_ci_d      ; jump if so
              inc     rc               ; otherwise increment size
              lbr     fmtwrt_ci        ; loop back until end or decimal
fmtwrt_ci_d:  ldi     0                ; terminate at decimal
              dec     rf
              str     rf
              lbr     fmtwrt_cp        ; and then transfer to output
; ************************
; ***** F Conversion *****
; ************************
fmtwrt_cf:    inc     r8               ; get decimal size
              inc     r8
              ldn     r8
              phi     rc
              dec     r8
              dec     r8
fmtwrt_cf_1:  lda     rf               ; get byte from conversion
              lbz     fmtwrt_cf_z      ; Jump if no decimal found
              smi     '.'              ; was a decimal encountered
              lbz     fmtwrt_cf_d      ; jump if so
              inc     rc               ; otherwise increment size
              lbr     fmtwrt_cf_1      ; loop back until end or decimal
fmtwrt_cf_z:  dec     rf               ; move back to terminator
              ldi     '.'              ; add a decimal point
              str     rf
              inc     rf
fmtwrt_cf_z1: ghi     rc               ; get decimal count
              lbz     fmtwrt_cf_z2     ; jump if done
              smi     1                ; decrement count
              phi     rc               ; and put back
fmtwrt_cf_z3: ldi     '0'              ; otherwise add ascii zero
              str     rf
              inc     rf
              inc     rc               ; increment size
              lbr     fmtwrt_cf_z1     ; loop until enough decimal places
fmtwrt_cf_z2: ldi     0                ; terminate output
              str     rf
              inc     rc
              lbr     fmtwrt_cp        ; and copy result
fmtwrt_cf_d:  ghi     rc               ; get decimal count
              lbz     fmtwrt_cf_z2     ; done if enough decimal places
              smi     1                ; decrement count
              phi     rc
              lda     rf               ; get byte from input
              lbz     fmtwrt_cf_z4     ; jump if not enough decimals
              inc     rc               ; increment width
              lbr     fmtwrt_cf_d      ; and loop until done
fmtwrt_cf_z4: dec     rf               ; move back to terminator
              lbr     fmtwrt_cf_z3     ; and pad with zeroes
; ************************
; ***** L Conversion *****
; ************************
fmtwrt_cl:    lda     rf               ; get next byte
              lbz     fmtwrt_clf       ; false if terminator found
              smi     '0'              ; check for non-zero
              lbnz    fmtwrt_clt       ; nonzero is true
              lbr     fmtwrt_cl        ; loop until terminator or nonzero
fmtwrt_clf:   ldi     'F'              ; need to output false
              lskp
fmtwrt_clt:   ldi     'T'              ; need to output true
              plo     re               ; save output
              ldi     scratch1_.1      ; back to beginning of buffer
              phi     rf
              ldi     scratch1_.0
              plo     rf
              glo     re               ; recover result
              str     rf               ; write to buffer
              inc     rf
              ldi     0                ; terminate it
              str     rf
              ldi     1                ; width is 1
              plo     rc
; ******************************
; ***** now copy to output *****
; ******************************
fmtwrt_cp:    ldi     scratch1_.1      ; point to converted data
              phi     rf
              ldi     scratch1_.0
              plo     rf
              inc     r8               ; get field width
              ldn     r8
              dec     r8
              str     r2               ; compare to converted width
              glo     rc
              sd                       ; field.width-converted.width
              lbnf    fmtwrt_cp_n      ; jump if converted width greater
fmtwrt_cp_y:  glo     rc               ; check width
              sm                       ; subtract field width
              lbz     fmtwrt_cp_cp     ; copy remaining characters
              ldi     ' '              ; write a blank to the output
              str     rb
              inc     rb
              inc     rc               ; added a character
              lbr     fmtwrt_cp_y      ; loop until padding done
fmtwrt_cp_n:  ldi     '*'              ; flag data to wide
              str     rb
              inc     rb
              dec     rc               ; remove 1 element
              inc     rf
              inc     rf
              ldn     r2               ; recover field width
              smi     1                ; now 1 smaller
              str     r2
fmtwrt_cp_n1: dec     rc               ; decrement width
              glo     rc               ; see if remaining will fit
              sm
              lbz     fmtwrt_cp_cp     ; jump if so
              inc     rf               ; increment pointer
              lbr     fmtwrt_cp_n1     ; and loop until it fits
fmtwrt_cp_cp: lda     rf               ; read next byte from data
              lbz     fmtwrt_en        ; done with entry
              str     rb               ; store into output
              inc     rb
              lbr     fmtwrt_cp_cp     ; loop until all copied
fmtwrt_en:    dec     ra               ; decrement specifier count
              glo     ra               ; see if done
              lbnz    fmtwrite_lp      ; next variable if still good
              inc     r8               ; move to next specifier
              inc     r8
              inc     r8
              lbr     checkfmt         ; and check it
fmtwrite_dn2: glo     rb               ; see if anything has been output
              smi     iobuffer.0
              lbnz    fmtwrite_dn3     ; jump if so
              ghi     rb               ; check high byte as well
              smi     iobuffer.1
              lbz     fmtwrite_dn      ; jump if no output needed
fmtwrite_dn3: ldi     0                ; terminate output
              str     rb
              ldi     iobuffer.1       ; point to output data
              phi     rf
              ldi     iobuffer.0
              plo     rf
              ghi     ra               ; get LUN
              sep     scall            ; write the record
              dw      writerec
              lbr     fmtwrite_dn      ; and then done
checkfmt:     lda     r8               ; get count for current item
              plo     ra
              lbnz    checkfmt_1       ; jump if not end of list
              ldi     0                ; terminate io buffer
              str     rb
              ldi     iobuffer.1       ; point to output data
              phi     rf
              ldi     iobuffer.0
              plo     rf
              ghi     ra               ; get LUN
              sep     scall            ; write the record
              dw      writerec
              ldn     r9               ; see if done with data list
              lbz     fmtwrite_dn      ; jump if done
              ldi     iobuffer.1       ; reset io buffer
              phi     rb
              ldi     iobuffer.0
              plo     rb
              irx                      ; recover beginning of format data
              ldxa
              plo     r8
              ldx
              phi     r8
              dec     r2               ; restore stack pointer
              dec     r2
              lbr     checkfmt         ; back to checking format
fmtwrite_dn:  irx                      ; remove format address from stack
              irx
              sep     sret             ; and return to caller
checkfmt_1:   ldn     r8               ; get format type
              smi     'X'              ; check for X
              lbz     fmtwrt_x         ; jump if so
              ldn     r8               ; check for new record
              smi     '/'
              lbz     fmtwrt_nr        ; jump if so
              ldn     r8               ; check for hollerith data
              smi     'H'
              lbnz    fmtwrite_lp      ; On to variables if not
fmtwrt_h1:    inc     r8               ; move past H
fmtwrt_h:     lda     r8               ; get byte from format data
              str     rb               ; write to output
              inc     rb
              dec     ra               ; decrement character count
              glo     ra               ; get count
              lbnz    fmtwrt_h         ; loop back if not done
              lbr     checkfmt         ; otherwise to next specifier
fmtwrt_x:     ldi     ' '              ; write a blank to output
              str     rb
              inc     rb
              dec     ra               ; decrement count
              glo     ra               ; see if done
              lbnz    fmtwrt_x         ; loop back if not done
fmtwrt_x1:    inc     r8               ; move to next specifier
              inc     r8
              inc     r8
              lbr     checkfmt         ; check next format specifier
fmtwrt_nr:    ldi     0                ; terminate current record
              str     rb
              ldi     iobuffer.1       ; point to output data
              phi     rf
              ldi     iobuffer.0
              plo     rf
              ghi     ra               ; get LUN
              sep     scall            ; write the record
              dw      writerec
              ldi     iobuffer.1       ; reset io buffer
              phi     rb
              ldi     iobuffer.0
              plo     rb
              lbr     fmtwrt_x1        ; then to next specifier

              endp

