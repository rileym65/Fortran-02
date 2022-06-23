#include      macros.inc
#include      bios.inc
; *****************************************
; ***** Rewind a file                 *****
; ***** D - File number (starts at 1) *****
; *****************************************
              proc     frewind

              extrn    ferr_no
              extrn    fildes

              sep      scall           ; get file record for file
              dw       fildes
              lbdf     frewind_rt      ; return if invalid file
              ldn      rd              ; get file open flag
              lbz      ferr_no         ; jump if file not open
              inc      rd              ; now move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              ghi      r7              ; preserver R7
              stxd
              glo      r7
              stxd
              ldi      0               ; set seek position to zero
              plo      r7
              phi      r7
              plo      r8
              phi      r8
              plo      rc              ; seek from beginning
              phi      rc
              sep      scall           ; call Elf/OS to rewind the file
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
frewind_rt:   sep      sret            ; return to caller

              endp

