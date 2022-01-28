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
  int  i;
  char token[32];
  char subName[32];
  int  pos;
  int  v;
  char *temp;
  char argType;
  numCallArgs = 0;
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  if (strncasecmp(line, "open(", 5) == 0) {
    copen(line+5);
    return;
    }

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
  
  if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
            *line == '_') subName[pos++] = *line++;
    subName[pos] = 0;
    if (pos == 0) {
      showError("NULL name not allowed");
      return;
      }

    if (*line == '(') {
      line++;
      argType = ' ';
      while (*line != 0 && *line != ')') {
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
printf("Variable arg: %s\n",token);
            argType = 'V';
            v = getVariable(token,module);
            if (v < 0) return;
            callArgs[numCallArgs] = v;
            callArgTypes[numCallArgs++] = 'V';
            argType = '*';
            line = temp;
            }
          }
        if (argType == ' ') {
printf("Expression arg: %s\n",token);
          line = cexpr(line, 2);
          if (exprErrors > 0) return;
          callArgs[numCallArgs] = 0;
          callArgTypes[numCallArgs++] = 'E';
          }
        if (*line == ',') line++;
        }
      if (*line != ')') {
        showError("Syntax error");
        return;
        }
      line++;
      if (*line != 0) {
        showError("Syntax error");
        return;
        }
      }

    if (numCallArgs > 0) {
      i = numCallArgs-1;
      while (i >= 0) {
        if (callArgTypes[i] == 'V') {
          sprintf(buffer,"          ldi   (%s_%s).0               ; Push variable address",
                  variables[callArgs[i]].module, variables[callArgs[i]].name);
          Asm(buffer);
          Asm("          stxd");
          sprintf(buffer,"          ldi   (%s_%s).1",
                  variables[callArgs[i]].module, variables[callArgs[i]].name);
          Asm(buffer);
          Asm("          stxd");
          }
        i--;
        }
      }

    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    sprintf(buffer,"          ldi   %d                      ; Set no of arguments",numCallArgs);
    Asm(buffer);
    Asm("          stxd                          ; Place on stack");
    Asm("          sep   scall                   ; Call subroutine");
    sprintf(buffer, "          dw    %s",subName); Asm(buffer);
    return;
    }

  }

