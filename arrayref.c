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

char* arrayRef(char* line, int v) {
  int dims;
  int d1;
/* *********************************** */
/* ***** Process array reference ***** */
/* *********************************** */
  if (v < 0) return NULL;
  if (variables[v].dimensions == 0) {
    showError("Array reference on non-array variable");
    return NULL;
    }
  dims = 1;
  line++;
  while (*line != ')' && *line != 0) {
    line = cexpr(line, 0);
    if (exprErrors > 0) return NULL;
    Asm("           sep     scall                     ; subtract 1");
    Asm("           dw      dec32");
    if (dims > 1) {
      if (dims == 2) d1 = variables[v].sizes[0];
        else d1 = variables[v].sizes[0] * variables[v].sizes[1];
      sprintf(buffer,"           ldi     %d                        ; multiply by size",(d1 >> 24) & 0xff);
      Asm(buffer);
      Asm("           sex     r7                        ; point x to expr stack");
      Asm("           stxd");
      sprintf(buffer,"           ldi     %d",(d1 >> 16) & 0xff);
      Asm(buffer);
      Asm("           stxd");
      sprintf(buffer,"           ldi     %d",(d1 >> 8) & 0xff);
      Asm(buffer);
      Asm("           stxd");
      sprintf(buffer,"           ldi     %d",d1 & 0xff);
      Asm(buffer);
      Asm("           stxd");
      Asm("           sex     r2                        ; point x back to stack");
      Asm("           sep     scall                     ; perform multiply");
      Asm("           dw      mul32");
      Asm("           sep     scall                     ; then add first dimension");
      Asm("           dw      add32");
      addDefine("MUL32",1,1);
      addDefine("ADD32",1,1);
      }
    if (*line == ',') {
      dims++;
      line++;
      }
    }
  if (*line == 0) {
    showError("Syntax error");
    return NULL;
    }
  if (*line != ')') {
    showError("Syntax error");
    return NULL;
    }
  line++;
  if (dims != variables[v].dimensions) {
    showError("Incorrect number of dimensions referenced");
    return NULL;
    }
  Asm("           inc     r7                        ; Recover offset");
  Asm("           lda     r7");
  Asm("           plo     rf");
  Asm("           lda     r7");
  Asm("           phi     rf");
  Asm("           inc     r7");
  
  if (varType(v) != 'L' && varType(v) != 'B') {
    Asm("           glo     rf                        ; Shift for data size");
    Asm("           shl");
    Asm("           plo     rf");
    Asm("           ghi     rf");
    Asm("           shlc");
    Asm("           phi     rf");
    }
  if (varType(v) == 'I' || varType(v) == 'R') {
    Asm("           glo     rf");
    Asm("           shl");
    Asm("           plo     rf");
    Asm("           ghi     rf");
    Asm("           shlc");
    Asm("           phi     rf");
    }
  addDefine("DEC32",1,1);
  return line;
  }
