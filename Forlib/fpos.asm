#include      macros.inc
#include      bios.inc

              proc     fpos

              extrn    fildes

              inc      r7              ; retrieve LUN
              lda      r7
              inc      r7              ; remove remaining bytes
              inc      r7
              sep      scall           ; get file record for file
              dw       fildes
              lbnf     fpos_1          ; jump if valid LUN
fpos_2:       sex      r7              ; on invalid file, push 0
              ldi      0
              stxd
              stxd
              stxd
              stxd
              sex      r2              ; point x back to system stack
              sep      sret            ; and return
fpos_1:       lda      rd              ; check if file is open
              lbz      fpos_2          ; push 0 if file is not open
              inc      rd              ; move to Elf/OS FILDES
              inc      rd
              inc      rd
              inc      rd
              inc      rd
              sex      r7              ; push file position
              lda      rd
              stxd
              lda      rd
              stxd
              lda      rd
              stxd
              lda      rd
              stxd
              sex      r2              ; point x back to system stack
              sep      sret            ; and return

              endp

