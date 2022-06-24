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

char* buildCall(char* subName, char* line) {
  char  token[32];
  int   pos;
  char  usedExpr;
  char *temp;
  char  argType;
  int   i;
  int   v;
  usedExpr = 0;
  numCallArgs = 0;
    if (*line == '(') {
      line++;
      while (*line != 0 && *line != ')') {
        argType = ' ';
        if ((*line >= 'a' && *line <= 'z') ||
            (*line >= 'A' && *line <= 'Z')) {
          pos = 0;
          temp = line;
          while ((*temp >= 'a' && *temp <= 'z') ||
                 (*temp >= 'A' && *temp <= 'Z') ||
                 (*temp >= '0' && *temp <= '9') ||
                 *temp == '_') token[pos++] = *temp++;
          token[pos] = 0;
          if (*temp == ')' || *temp == ',') {
            argType = 'V';
            v = getVariable(token,module);
            if (v < 0) return NULL;
            callArgs[numCallArgs] = v;
            callArgTypes[numCallArgs++] = 'V';
            argType = '*';
            line = temp;
            }
          }
        if (argType == ' ') {
          line = cexpr(line, 2, module);
          if (exprErrors > 0) return NULL;
          callArgs[numCallArgs] = 0;
          callArgTypes[numCallArgs++] = 'E';
          Asm("           inc   r7                      ; Transfer value to stack");
          Asm("           lda   r7");
          Asm("           stxd");
          Asm("           lda   r7");
          Asm("           stxd");
          Asm("           lda   r7");
          Asm("           stxd");
          Asm("           ldn   r7");
          Asm("           stxd");
          usedExpr = 0xff;
          }
        else {
          }
        if (*line == ',') line++;
        }
      if (*line != ')') {
        showError("Syntax error");
        return NULL;
        }
      line++;
      }
    if (numCallArgs > 0) {
      if (usedExpr) {
        Asm("           glo   r2                      ; Get address of value on stack");
        Asm("           plo   ra");
        Asm("           ghi   r2");
        Asm("           phi   ra");
        Asm("           inc   ra");
        usedExpr = 0;
        }
      i = numCallArgs-1;
      while (i >= 0) {
        if (callArgTypes[i] == 'V') {
          sprintf(buffer,"          ldi   (%s).0               ; Push variable address",
                  variables[callArgs[i]].name);
          Asm(buffer);
          Asm("          stxd");
          sprintf(buffer,"          ldi   (%s).1",
                  variables[callArgs[i]].name);
          Asm(buffer);
          Asm("          stxd");
          }
        else {
          if (usedExpr) {
            Asm("           inc   ra                      ; Move to next value");
            Asm("           inc   ra");
            Asm("           inc   ra");
            Asm("           inc   ra");
            }
          Asm("           glo   ra                      ; Push address of value on stack");
          Asm("           stxd");
          Asm("           ghi   ra");
          Asm("           stxd");
          usedExpr = 0xff;
          }
        i--;
        }
      }
    sprintf(buffer,"          ldi   %d                      ; Set no of arguments",numCallArgs);
    Asm(buffer);
    Asm("          stxd                          ; Place on stack");
    Asm("          sep   scall                   ; Call subroutine");
    sprintf(buffer, "          dw    %s",subName); Asm(buffer);
    for (i=0; i<numCallArgs; i++) {
      if (callArgTypes[i] == 'E') {
        Asm("           inc   r2                      ; Remove temp value from stack");
        Asm("           inc   r2");
        Asm("           inc   r2");
        Asm("           inc   r2");
        }
      }
    return line;
  }

