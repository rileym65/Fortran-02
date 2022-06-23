#include      macros.inc
#include      bios.inc

              proc     fread

              extrn    ferr_no
              extrn    fildes

              ghi      ra
              sep      scall           ; get file record for file
              dw       fildes
              lbdf     return          ; return if invalid file
              ldn      rd              ; get file open flag
              lbz      ferr_no         ; jump if file not open
              inc      rd              ; point to record size
              lda      rd
              plo      rc              ; put into count
              ldi      0
              phi      rc
              inc      rd              ; move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              sep      scall           ; call Elf/OS to read the record
              dw       0309h
              dec      rd              ; move back to IOSTATUS field
              dec      rd
              dec      rd
              str      rd              ; store status into record
              dec      rd              ; point to IORESULT
              ldi      0               ; get restult flag
              shlc
              str      rd              ; and store it
              lbz      fread_gd        ; jump if good read
              sep      sret            ; return to caller
fread_gd:     inc      rd              ; back to status field
              glo      rc              ; get bytes read
              str      rd
return:       sep      sret

              endp

