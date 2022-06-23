#include      macros.inc
#include      bios.inc
; **********************************************
; ***** Find file record for a file number *****
; ***** D - File number (starts at 1)      *****
; ***** Returns: DF=0 - file good          *****
; *****          DF=1 - Invalid number     *****
; *****          RD   - File record        *****
; **********************************************
              proc     fildes

              extrn    file1_
              extrn    file2_
              extrn    file3_
              extrn    file4_

              smi      1               ; File numbers start at 1
              lbz      fildes1         ; jump if file 1
              smi      1               ; check for file 2
              lbz      fildes2
              smi      1               ; check for file 3
              lbz      fildes3
              smi      1               ; check for file 4
              lbz      fildes4
              smi      0               ; set DF to indicate invalid
              sep      sret            ; and return to caller
fildes1:      ldi      file1_.1        ; point file 1 record
              phi      rd
              ldi      file1_.0
              plo      rd
              adi      0               ; signal good
              sep      sret            ; and return
fildes2:      ldi      file2_.1        ; point file 1 record
              phi      rd
              ldi      file2_.0
              plo      rd
              adi      0               ; signal good
              sep      sret            ; and return
fildes3:      ldi      file3_.1        ; point file 1 record
              phi      rd
              ldi      file3_.0
              plo      rd
              adi      0               ; signal good
              sep      sret            ; and return
fildes4:      ldi      file4_.1        ; point file 1 record
              phi      rd
              ldi      file4_.0
              plo      rd
              adi      0               ; signal good
              sep      sret            ; and return

              endp

