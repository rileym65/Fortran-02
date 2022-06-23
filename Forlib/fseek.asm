#include      macros.inc
#include      bios.inc

              proc     fseek

              extrn    ferr_no
              extrn    fildes
              extrn    iostatus_er

              sep      scall           ; get file record for file
              dw       fildes
              lbdf     iostatus_er     ; jump if invalid LUN
              inc      r7              ; retrieve offset
              lda      r7
              plo      rb
              lda      r7
              phi      rb
              lda      r7
              plo      r8
              ldn      r7
              phi      r8
              lda      rd              ; get file open flag
              lbz      ferr_no         ; jump if file not open
              ghi      r7              ; preserve R7
              stxd
              glo      r7
              stxd
              ghi      rb              ; move low word of offset
              phi      r7
              glo      rb
              plo      r7
              inc      rd
              inc      rd              ; move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              ldi      0               ; clear high byte of RC
              phi      rc
              sep      scall           ; call Elf/OS to seek the file
              dw       030fh
              dec      rd              ; move back to IOSTATUS field
              dec      rd
              dec      rd
              str      rd              ; store status into record
              dec      rd              ; point to IORESULT
              ldi      0               ; get restult flag
              shlc
              str      rd              ; and store it
              irx                      ; recover R7
              ldxa
              plo      r7
              ldx
              phi      r7
              sep      sret            ; and return to caller

              endp

