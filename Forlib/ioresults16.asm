#include    macros.inc
#include    bios.inc

            proc   ioresults16

            extrn  v_ioflag
            extrn  v_ioresult

            plo    re              ; save return code
            ldi    v_ioresult.1    ; point to ioresult variable
            phi    rf
            ldi    v_ioresult.0
            plo    rf
            ldi    0               ; set msb to zero
            str    rf
            inc    rf
            glo    re              ; then set lsb to result code
            str    rf
            ldi    v_ioflag.1      ; point to ioflag variable
            phi    rf
            ldi    v_ioflag.0
            plo    rf
            ldi    0               ; clear msb of ioflag
            str    rf
            inc    rf
            shlc                   ; set D to DF
            str    rf              ; and store into ioflag
            sep    sret            ; return to caller

            endp

