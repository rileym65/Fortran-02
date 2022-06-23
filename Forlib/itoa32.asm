#include    macros.inc
#include    bios.inc

; ***************************************************
; ***** Convert 32-bit binary to ASCII          *****
; ***** RF - Pointer to 32-bit integer          *****
; ***** RD - destination buffer                 *****
; ***************************************************
            proc    itoa32

            extrn   tobcd32

            lda     rf           ; retrieve number into R7:R8
            plo     rb
            lda     rf
            phi     rb
            lda     rf
            plo     ra
            lda     rf
            phi     ra
            glo     r2           ; make room on stack for buffer
            smi     11
            plo     r2
            ghi     r2
            smbi    0
            phi     r2
            glo     r2           ; RF is output buffer
            plo     rf
            ghi     r2
            phi     rf
            inc     rf
            ghi     ra           ; get high byte
            shl                  ; shift bit to DF
            lbdf    itoa32n      ; negative number
itoa321:    sep     scall        ; convert to bcd
            dw      tobcd32
            glo     r2
            plo     rf
            ghi     r2
            phi     rf
            inc     rf
            ldi     10
            plo     rb
            ldi     9            ; max 9 leading zeros
            phi     rb
loop1:      lda     rf
            lbz     itoa32z      ; check leading zeros
            str     r2           ; save for a moment
            ldi     0            ; signal no more leading zeros
            phi     rb
            ldn     r2           ; recover character
itoa322:    adi     030h
            str     rd           ; store into output buffer
            inc     rd
itoa323:    dec     rb
            glo     rb
            lbnz    loop1
            glo     r2           ; pop work buffer off stack
            adi     11
            plo     r2
            ghi     r2
            adci    0
            phi     r2
            ldi     0            ; place terminator in destination
            str     rd
            sep     sret         ; return to caller
itoa32z:    ghi     rb           ; see if leading have been used up
            lbz     itoa322      ; jump if so
            smi     1            ; decrement count
            phi     rb
            lbr     itoa323      ; and loop for next character
itoa32n:    ldi     '-'          ; show negative
            str     rd           ; write to destination buffer
            inc     rd
            glo     rb           ; 2s compliment
            xri     0ffh
            adi     1
            plo     rb
            ghi     rb
            xri     0ffh
            adci    0
            phi     rb
            glo     ra
            xri     0ffh
            adci    0
            plo     ra
            ghi     ra
            xri     0ffh
            adci    0
            phi     ra
            lbr     itoa321        ; now convert/show number

            endp

