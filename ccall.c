/*
 *******************************************************************
 *** This software is copyright 2021 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include "header.h"

void ccall(char* line) {
  if (strncasecmp(line, "poke(", 5) == 0) {
    line += 5;
    line = cexpr(line, 0);
    if (*line != ',') {
      showError("Syntax error");
      return;
      }
    line++;
    Asm("           inc     r7                  ; Retrieve address");
    Asm("           lda     r7");
    Asm("           stxd                        ; save for now");
    Asm("           lda     r7");
    Asm("           stxd                        ; save for now");
    Asm("           inc     r7                  ; remove high word");
    Asm("           inc     r7");
    line = cexpr(line, 0);
    if (*line != ')') {
      showError("Syntax error");
      return;
      }
    line++;
    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    Asm("           irx                         ; recover address");
    Asm("           ldxa");
    Asm("           phi     rf");
    Asm("           ldx");
    Asm("           plo     rf");
    Asm("           inc     r7                  ; Retrieve data");
    Asm("           lda     r7");
    Asm("           str     rf                  ; write to memory");
    Asm("           inc     r7                  ; clear rest of data from expr stack");
    Asm("           inc     r7");
    Asm("           inc     r7");
    return;
    }

  if (strncasecmp(line, "out(", 4) == 0) {
    line += 4;
    line = cexpr(line, 0);
    if (exprErrors > 0) return;
    if (*line != ',') {
      showError("Syntax error");
      return;
      }
    line++;
    Asm("          ldi   0d3h                    ; Need a SEP R3 command");
    Asm("          stxd                          ; Place on stack");
    Asm("          inc   r7                      ; Get port");
    Asm("          lda   r7");
    Asm("          ani   7                       ; Keep only bottom 3 bits");
    Asm("          ori   060h                    ; Convert to OUT command");
    Asm("          stxd                          ; Place on stack");
    Asm("          inc   r7                      ; Remove high word");
    Asm("          inc   r7");
    line = cexpr(line, 0);
    if (exprErrors > 0) return;
    if (*line != ')') {
      showError("Syntax error");
      return;
      }
    line++;
    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    Asm("          irx                           ; Recover OUT command");
    Asm("          ldx");
    Asm("          plo   re                      ; Set aside for a moment");
    Asm("          inc   r7                      ; Write out value to stack");
    Asm("          lda   r7");
    Asm("          stxd");
    Asm("          inc   r7                      ; Remove high word");
    Asm("          inc   r7");
    Asm("          glo   re                      ; Recover OUT command");
    Asm("          str   r2                      ; Place back on stack");
    Asm("          sep   r2                      ; Execute code on stack");
    Asm("          dec   r2                      ; Put stack pointer back where it belongs");
    return;
    }
  }

