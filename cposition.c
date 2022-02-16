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

void cposition(char* line) {
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();

  line = cexpr(line,0,module);
  if (exprErrors > 0) return;
  Asm("           inc     r7                  ; retrieve LUN");
  Asm("           lda     r7");
  Asm("           stxd                        ; set aside for now");
  Asm("           inc     r7                  ; remove remaining bytes");
  Asm("           inc     r7");
  if (*line != ',') {
    showError("Syntax error");
    return;
    }
  line++;
  line = cexpr(line,0,module);
  if (exprErrors > 0) return;
  if (*line != ',' && *line != ')') {
    showError("Syntax error");
    return;
    }
  if (*line == ',') {
    line++;
    line = cexpr(line,0,module);
    if (exprErrors > 0) return;
    Asm("           inc     r7                  ; retrieve whence");
    Asm("           lda     r7");
    Asm("           plo     rc");
    Asm("           inc     r7                  ; remove remaining bytes");
    Asm("           inc     r7");
    }
  else {
    Asm("           ldi     0                   ; from beginning");
    Asm("           plo     rc");
    }
  Asm("           irx                         ; recover LUN");
  Asm("           ldx");
  Asm("           sep     scall               ; Perform seek");
  Asm("           dw      fseek");
  addDefine("FSEEK",1,1);
  }

