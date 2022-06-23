#include    macros.inc
#include    bios.inc

            proc   isnumeral

            plo    re              ; keep a copy
            smi    '0'             ; check for below digits
            lbnf   isnum_no        ; jump if not a numeral
            smi    10              ; check high of range
            lbdf   isnum_no        ; jump if too high
            smi    0               ; set DF to indicate numeral
            glo    re              ; recover original
            sep    sret            ; and return
isnum_no:   adi    0               ; clear DF to indicate not numeral
            glo    re              ; recover original character
            sep    sret            ; and return to caller

            endp

