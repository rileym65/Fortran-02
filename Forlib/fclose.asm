#include      macros.inc
#include      bios.inc

; *****************************************
; ***** Close a file                  *****
; ***** D - File number (starts at 1) *****
; *****************************************
              proc     fclose

              extrn    ferr_no
              extrn    fildes

              sep      scall           ; get file record for file
              dw       fildes
              lbdf     fclose_rt       ; return if invalid file
              ldn      rd              ; get file open flag
              lbz      ferr_no         ; jump if file not open
              ldi      0               ; mark file as closed
              str      rd
              inc      rd              ; now move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              sep      scall           ; call Elf/OS to close the file
              dw       0312h
              dec      rd              ; move back to IOSTATUS field
              dec      rd
              dec      rd
              str      rd              ; store status into record
              dec      rd              ; point to IORESULT
              ldi      0               ; get restult flag
              shlc
              str      rd              ; and store it
fclose_rt:    sep      sret            ; return to caller

              endp

