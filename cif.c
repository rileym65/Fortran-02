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

void cif(char* line) {
  int falseLabel;
  word l[3];
  int  c;
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  if (*line != '(') {
    showError("Syntax error");
    return;
    }
  line++;
  line = cexpr(line, 0, module);
  if (*line != ')') {
    showError("Syntax error");
    return;
    }
  line++;
  if (*line >= '0' && *line <= '9') {
    c = 0;
    l[0] = 0; l[1] = 0; l[2] = 0;
    while ((*line >= '0' && *line <= '9') || *line == ',') {
      if (*line >= '0' && *line <= '9') l[c] = (l[c] * 10) + (*line - '0');
      if (*line == ',') {
        c++;
        if (c > 2) {
          showError("Syntax error");
          return;
          }
        }
      line++;
      }
    c++;
    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    if (c != 3) {
      showError("Syntax error");
      return;
      }
    Asm("          inc   r7                        ; Get expression result");
    Asm("          lda   r7");
    Asm("          str   r2                        ; store for zero check");
    Asm("          lda   r7                        ; next byte");
    Asm("          or                              ; combine with last byte");
    Asm("          str   r2                        ; and keep it");
    Asm("          lda   r7                        ; next byte");
    Asm("          or                              ; combine with last byte");
    Asm("          str   r2                        ; and keep it");
    Asm("          ldn   r7                        ; next byte");
    Asm("          or                              ; combine with last byte");
    sprintf(buffer, "          lbz   %s_%d                     ; jump if zero",module,l[1]);
    Asm(buffer);
    Asm("          ldn   r7                        ; recover msb");
    Asm("          shl                             ; shift sign bit into df");
    sprintf(buffer, "          lbdf  %s_%d                     ; jump if negative",module,l[0]);
    Asm(buffer);
    sprintf(buffer, "          lbr   %s_%d                     ; jump if positive",module,l[2]);
    Asm(buffer);
    return;
    }
  Asm("          inc   r7                        ; Get expression result");
  Asm("          lda   r7");
  Asm("          str   r2                        ; store for zero check");
  Asm("          lda   r7                        ; next byte");
  Asm("          or                              ; combine with last byte");
  Asm("          str   r2                        ; and keep it");
  Asm("          lda   r7                        ; next byte");
  Asm("          or                              ; combine with last byte");
  Asm("          str   r2                        ; and keep it");
  Asm("          ldn   r7                        ; next byte");
  Asm("          or                              ; combine with last byte");
  falseLabel = nextLabel++;
  sprintf(buffer, "          lbz   lbl_%d                     ; jump if zero",falseLabel);
  Asm(buffer);

  if (strncasecmp(line, "assign", 6) == 0)         cassign(line+6);
  else if (strncasecmp(line, "call", 4) == 0)      ccall(line+4);
  else if (strncasecmp(line, "continue", 8) == 0)  ccontinue(line+8);
  else if (strncasecmp(line, "end", 3) == 0)       cend(line+3);
  else if (strncasecmp(line, "goto", 4) == 0)      cgoto(line+4);
  else if (strncasecmp(line, "if", 2) == 0)        cif(line+2);
  else if (strncasecmp(line, "pause", 5) == 0)     cpause(line+5);
  else if (strncasecmp(line, "stop", 4) == 0)      cstop(line+4);
  else if (strncasecmp(line, "write", 5) == 0)     cwrite(line+5);
  else clet(line);


  sprintf(buffer, "lbl_%d:", falseLabel);
  Asm(buffer);
  }

