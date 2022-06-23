#include      macros.inc
#include      bios.inc

; ********************************
; ***** Write record         *****
; ***** D - LUN              *****
; ***** RF - Pointer to data *****
; ********************************
              proc    writerec

              extrn   fwrite

              plo     re               ; keep copy of LUN
              smi     5                ; check for terminal
              lbz     wrtrec_t         ; jump if so
              smi     1                ; check for terminal carriage control
              lbz     wrtrec_tcc       ; jump if so
              glo     re               ; recover LUN
              sep     scall            ; call disk write
              dw      fwrite
              sep     sret             ; and return to caller
wrtrec_t:     sep     scall            ; display buffer
              dw      f_msg
              sep     scall            ; followed by cr/lf
              dw      f_inmsg
              db      10,13,0
              sep     sret             ; then return
wrtrec_tcc:   lda     rf               ; get carriage control character
              plo     re               ; keep a copy
              smi     '+'              ; check for no advance
              lbz     wrtrec_tccg      ; jump if so
              glo     re               ; check for clear screen
              smi     '0'              ; check for double space
              lbz     wrtrec_tds
              smi     1                ; check for clear screen
              lbz     wrtrec_tcls      ; jump if so
              glo     re               ; check for triple space
              smi     '-'
              lbz     wrtrec_tts       ; jump if so

              sep     scall            ; otherwise advance 1 line
              dw      f_inmsg
              db      10,13,0
              lbr     wrtrec_tccg      ; then print line
wrtrec_tds:   sep     scall            ; advance two lines
              dw      f_inmsg
              db      10,13,10,13,0
              lbr     wrtrec_tccg      ; then print line
wrtrec_tts:   sep     scall            ; advance three lines
              dw      f_inmsg
              db      10,13,10,13,10,13,0
              lbr     wrtrec_tccg      ; then print line
wrtrec_tcls:  sep     scall            ; clear the screen 
              dw      f_inmsg
              db      27,'[H',27,'[2J',0 
wrtrec_tccg:  sep     scall            ; display buffer
              dw      f_msg
              sep     sret             ; and return to caller

              endp

