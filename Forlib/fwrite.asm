#include      macros.inc
#include      bios.inc

              proc     fwrite

              extrn    ferr_no
              extrn    fildes

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
              sep      scall           ; call Elf/OS to write the record
              dw       030ch
              dec      rd              ; move back to IOSTATUS field
              dec      rd
              dec      rd
              str      rd              ; store status into record
              dec      rd              ; point to IORESULT
              ldi      0               ; get restult flag
              shlc
              str      rd              ; and store it
              lbz      fwrite_gd       ; jumpt if good read
              sep      sret            ; return to caller
fwrite_gd:    inc      rd              ; back to status field
              glo      rc              ; get bytes written
              str      rd
return:       sep      sret

              endp

