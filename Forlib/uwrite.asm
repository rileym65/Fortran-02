#include    macros.inc
#include    bios.inc

            proc    uwrite

            extrn   fildes
            extrn   iobuffer

            ldi     iobuffer.1         ; setup output buffer
            phi     rb
            ldi     iobuffer.0
            plo     rb
            ldi     0                  ; clear count
            plo     rc
            phi     rc
uwrite_lp:  lda     r9                 ; get next variable type
            lbz     uwrite_dn          ; jump if done with variables
            plo     re                 ; keep type
            lda     r9                 ; get variable address
            phi     rf
            lda     r9
            plo     rf
            glo     re
            smi     'B'                ; check for byte
            lbz     uwrite_1           ; jump if so
            glo     re
            smi     'L'                ; check for logical
            lbz     uwrite_1           ; jump if so
            glo     re
            smi     'S'                ; check for short
            lbz     uwrite_2           ; jump if so
            ldi     4                  ; four bytes to transfer
            lbr     uwrite_go
uwrite_1:   ldi     1                  ; 1 byte to transfer
            lskp
uwrite_2:   ldi     2                  ; 2 bytes to transfer
uwrite_go:  plo     rd                 ; will count them here
uwrite_c1:  lda     rf                 ; get byte from variable
            str     rb                 ; put into output buffer
            inc     rb
            inc     rc                 ; increment byte count
            dec     rd                 ; decrement variable bytes
            glo     rd                 ; see if all copied
            lbnz    uwrite_c1          ; jump if not
            lbr     uwrite_lp          ; loop back for more variables
uwrite_dn:  ghi     ra
            sep     scall              ; get file record for file
            dw      fildes
            lbdf    uwrite_rt          ; return if invalid file
            lda     rd                 ; get file open flag
            lbz     uwrite_rt          ; error if file is not open
            inc     rd                 ; move to Elf/OS FILDES
            inc     rd
            inc     rd
            inc     rd
            inc     rd
            ldi     iobuffer.1         ; setup output buffer
            phi     rf
            ldi     iobuffer.0
            plo     rf
            sep     scall              ; call Elf/OS to write
            dw      030ch
            dec     rd                 ; move to status field
            dec     rd
            dec     rd
            lbdf    uwrite_er          ; jump if error occured
            glo     rc                 ; otherwise get count written
            str     rd
            dec     rd
            ldi     0
            str     rd
            glo     rc
            sep     sret
uwrite_er:  str     rd                 ; write D to status
            dec     rd                 ; point to flag field
            ldi     0
            shlc                       ; move DF to D
            str     rd                 ; and write to flag
            shr                        ; restore DF
            sep     sret               ; and return to caller
uwrite_rt:  smi     0                  ; make sure DF is set for error
            sep     sret               ; and return to caller

            endp

