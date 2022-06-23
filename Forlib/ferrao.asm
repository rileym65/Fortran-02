#include      macros.inc
#include      bios.inc

              proc     ferr_ao

              inc      rd              ; move to IOFLAG
              inc      rd
              ldi      1               ; signal error
              str      rd
              inc      rd
              ldi      021h            ; signal file already open
              str      rd              ; store into IORESULT
              smi      0               ; Set DF to signal error
              sep      sret            ; and return to caller

              endp

