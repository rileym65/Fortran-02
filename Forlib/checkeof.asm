#include    macros.inc
#include    bios.inc

; ************************************
; ***** Check if at EOF for file *****
; ***** D - handle number        *****
; ***** Returns: D=0 - Not EOF   *****
; *****          D=0ff = at EOF  *****
; ************************************
            proc    checkeof

            extrn   file1_

            smi     1            ; handle number minus 1
            shl                  ; handles are two bytes
            adi     file1_.0     ; add to file handle storage
            plo     rf
            ldi     file1_.1     ; propagate carry
            adci    0
            phi     rf
            lda     rf           ; retrieve FILDES address
            phi     rd
            ldn     rf
            adi     8            ; point to flags byte
            plo     rd
            ghi     rd           ; propage carry
            adci    0
            phi     rd           ; RD now points at flags
            ldn     rd           ; get flags
            ani     004h         ; check if in last lump
            lbz     eof_no       ; jump if not
            dec     rd           ; move to lsb of EOF
            ldn     rd           ; retrieve it
            plo     rf           ; put into rf
            dec     rd           ; point to msb of EOF
            ldn     rd           ; retrieve it
            ani     0fh          ; keep only low 4 bits
            phi     rf           ; RF now has EOF in lump
            dec     rd           ; move to LSB of offset
            dec     rd
            dec     rd
            ldn     rd           ; retrieve it
            str     r2           ; store for compare
            glo     rf           ; compare against EOF
            sm
            lbnz    eof_no       ; jump if no match
            dec     rd           ; move to next higher byte
            ldn     rd           ; and retrieve it
            ani     0fh          ; keep only bottom 4 bits
            str     r2           ; store for compare
            ghi     rf           ; get high byte of EOF
            sm                   ; and cmpare
            lbnz    eof_no       ; jump if not match
            ldi     0ffh         ; signal at EOF
            sep     sret         ; return to caller
eof_no:     ldi     0            ; signal not in eof
            sep     sret         ; and return

            endp

