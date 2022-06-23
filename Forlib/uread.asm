#include    macros.inc
#include    bios.inc

            proc    uread

            extrn   ferr_no
            extrn   fildes
            extrn   iobuffer

            ghi     ra                 ; get lun
            sep     scall              ; get file record for file
            dw      fildes
            lbdf    uread_rt           ; return if invalid file
            ldn     rd                 ; get file open flag
            lbz     ferr_no            ; error if the file is not open
            inc     rd                 ; move to Elf/OS FILDES
            inc     rd
            inc     rd
            inc     rd
            inc     rd
            inc     rd
            ghi     r9                 ; save variable list
            stxd
            glo     r9
            stxd
            ldi     iobuffer.1         ; setup output buffer
            phi     rf
            ldi     iobuffer.0
            plo     rf
            sep     scall              ; call Elf/OS to read file
            dw      0309h
            plo     re                 ; save returned status
            irx                        ; recover variable list
            ldxa
            plo     r9
            ldx
            phi     r9
            glo     re                 ; recover status
            lbdf    uread_er           ; jump on read error
            glo     rc                 ; get bytes read
            str     rd                 ; write to status
            dec     rd                 ; point to io flag
            ldi     0                  ; indicate success
            str     rd
            ldi     iobuffer.1         ; point to i/o buffer
            phi     rf
            ldi     iobuffer.0
            plo     rf
uread_lp:   lda     r9                 ; get next variable type
            lbz     uread_dn           ; jump if done
            plo     re                 ; keep a copy
            lda     r9                 ; get address
            phi     rd
            lda     r9
            plo     rd
            glo     re                 ; recover var type
            smi     'B'                ; is it a byte
            lbz     uread_1            ; jump if so
            glo     re                 ; check for logical
            smi     'L'
            lbz     uread_1            ; jump if so
            glo     re                 ; check for short
            smi     'S'
            lbz     uread_2            ; jump if so
            ldi     4
            lskp
uread_1:    ldi     1
            lskp
uread_2:    ldi     2
            plo     ra                 ; set count
uread_l2:   lda     rf                 ; get byte from io buffer
            str     rd                 ; write to variable
            inc     rd
            dec     ra                 ; decrement count
            glo     ra                 ; see if done
            lbnz    uread_l2           ; loop until done
            lbr     uread_lp           ; on to next variable
uread_dn:   adi     0                  ; signal no error
            glo     rc                 ; get byte count
            sep     sret               ; and return
uread_er:   str     rd                 ; store status
            dec     rd                 ; point to ioflag
            ldi     0
            shlc                       ; mv DF to D
            str     rd                 ; store status
            shr                        ; restore DF
            sep     sret               ; and return to caller
uread_rt:   smi     0                  ; indicate error
            sep     sret               ; and return to caller

            endp

