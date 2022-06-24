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

void creturn(char* line) {
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  if (inSub == 0) {
    showError("Invalid use of RETURN");
    return;
    }
  if (*line != 0) {
    showError("Syntax error");
    return;
    }
  if (functionVar >= 0) {
    sprintf(buffer,"              ldi   %s.1            ; Point to result variable",
            variables[functionVar].name);
    Asm(buffer);
    Asm("              phi   rf");
    sprintf(buffer,"              ldi   %s.0",
            variables[functionVar].name);
    Asm(buffer);
    Asm("              plo   rf");
    Asm("              sex   r7");
    if (varType(functionVar) == 'I' || varType(functionVar) == 'R') {
      Asm("              lda   rf");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      }
    if (varType(functionVar) == 'S') {
      Asm("              lda   0");
      Asm("              stxd");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      }
    if (varType(functionVar) == 'B' || varType(functionVar) == 'L') {
      Asm("              ldi   0");
      Asm("              stxd");
      Asm("              stxd");
      Asm("              stxd");
      Asm("              lda   rf");
      Asm("              stxd");
      }
    Asm("              sex   r2");
    }
  Asm("              sep   sret               ; Return to caller");
  }

