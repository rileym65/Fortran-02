#include      macros.inc
#include      bios.inc

              proc     fopen

              extrn    ferr_ao
              extrn    fildes

              sep      scall           ; get file record for file
              dw       fildes
              lbdf     fopen_rt        ; return if invalid file
              ldn      rd              ; get file open flag
              lbnz     ferr_ao         ; jump if already open
              inc      rd              ; move to record length
              glo      rc              ; store record length
              str      rd
              inc      rd              ; then move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              ghi      r7              ; preserver R7
              stxd
              glo      r7
              stxd
              glo      r8              ; Get file flags
              plo      r7
              ldi      0
              phi      r7
              sep      scall           ; call Elf/OS to open the file
              dw       0306h
              dec      rd              ; move back to IOSTATUS field
              dec      rd
              dec      rd
              str      rd              ; store status into record
              dec      rd              ; point to IORESULT
              ldi      0               ; get restult flag
              shlc
              str      rd              ; and store it
              shr                      ; put success back into DF
              irx                      ; recover R7
              ldxa
              plo      r7
              ldx
              phi      r7
              lbdf     fopen_rt        ; return on error
              dec      rd              ; move back to open flag
              dec      rd
              ldi      1               ; signal file is open
              str      rd
fopen_rt:     sep      sret            ; return to caller

              endp

