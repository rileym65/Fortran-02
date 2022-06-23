#include      macros.inc
#include      bios.inc

              proc     iostatus

              extrn    fildes
              extrn    iostatus_er

              inc      r7              ; get LUN from stack
              lda      r7
              inc      r7              ; remove remainder of number
              inc      r7
              plo      re              ; set number aside
              ldi      0               ; high 3 bytes of result are zero
              str      r7
              dec      r7
              str      r7
              dec      r7
              str      r7
              dec      r7
              glo      re              ; recover file number
              sep      scall           ; get file record for file
              dw       fildes
              lbdf     iostatus_er     ; jump if bad file number
              inc      rd              ; point to iostatus field
              inc      rd
              inc      rd
              lda      rd              ; and retrieve it
              str      r7              ; place on expr stack
              dec      r7
              sep      sret            ; and return

              endp

              proc     iostatus_er

              ldi      0ffh            ; indicate bad file number
              str      r7              ; place on expr stack
              dec      r7
              sep      sret            ; and return

              endp

