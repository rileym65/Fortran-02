#include    macros.inc
#include    bios.inc

; *******************************************
; ***** Normalize and combine FP result *****
; ***** R7:R8 - Mantissa                *****
; ***** R9.0  - Exponent                *****
; ***** R9.1  - Sign                    *****
; ***** Returns: R7:R8 - FP number      *****
; *******************************************
            proc    fpnorm

            glo     r9           ; Get exponent
            lbz     fpnorm0      ; jump if zero
            glo     r8           ; zero check mantissa
            lbnz    fpnormnz     ; jump if not
            ghi     r8
            lbnz    fpnormnz
            glo     r7
            lbnz    fpnormnz
            ghi     r7
            lbnz    fpnormnz
fpnorm0:    ldi     0            ; set result to 0
            plo     r8
            phi     r8
            plo     r7
            phi     r7
            sep     sret         ; and return
fpnormi:    ldi     03fh         ; set infinity
            phi     r7
            ldi     080h
            plo     r7
            ldi     0
            phi     r8
            plo     r8
            sep     sret         ; and return
fpnormnz:   ghi     r7           ; check for need to right shift
            lbz     fpnorm_1     ; jump if no right shifts needed
            shr                  ; shift mantissa right
            phi     r7
            glo     r7
            shrc
            plo     r7
            ghi     r8
            shrc
            phi     r8
            glo     r8
            shrc
            plo     r8
            inc     r9           ; increment exponent
            glo     r9           ; get exponent
            smi     0ffh         ; check for exponent overflow
            lbz     fpnormi      ; jump if exponent overflow, returns 0
            lbr     fpnormnz     ; keep checking for bits too high in mantissa
fpnorm_1:   glo     r7           ; check for need to shift left
            shl                  ; shift high bit into DF
            lbdf    fpnorm_2     ; jump if high bit is set
            glo     r8           ; shift mantissa left
            shl
            plo     r8
            ghi     r8
            shlc
            phi     r8
            glo     r7
            shlc
            plo     r7
            dec     r9           ; decrement exponent
            glo     r9           ; check for exponent underflow
            lbz     fpnorm0      ; jump if underflow occured
            lbr     fpnorm_1     ; loop until high bit set
fpnorm_2:   glo     r7           ; prepare mantissa for merging exponent
            shl
            plo     r7
            ghi     r9           ; get sign
            shr                  ; shift into DF
            glo     r9           ; get exponent
            shrc                 ; shift in sign
            phi     r7           ; place into answer
            glo     r7           ; get high byte of mantissa
            shrc                 ; shift in least bit from exponent
            plo     r7           ; and put back
            sep     sret         ; return to caller

            endp

