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

void cwrite(char* line) {
  char token[32];
  int  pos;
  int  v;
  int  varLabel;
  int  eolLabel;
  if (*line != '(') {
    showError("Syntax error");
    return;
    }
  line++;
  line = cexpr(line,0);
  if (exprErrors > 0) return;
  if (*line != ',') {
    showError("Syntax error");
    return;
    }
  line++;
  if (*line >= '0' && *line <= '9') {
    /* *************************** */
    /* ***** Formatted write ***** */
    /* *************************** */
    pos = 0;
    while (*line >= '0' && *line <= '9') token[pos++] = *line++;
    token[pos] = 0;
    sprintf(buffer,"           ldi     (%s_%s+3).1                 ; point to format data",
      module, token);
    Asm(buffer);
    Asm("           phi     r8");
    sprintf(buffer,"           ldi     (%s_%s+3).0", module, token);
    Asm(buffer);
    Asm("           plo     r8");
    varLabel = nextLabel++;
    eolLabel = nextLabel++;
    sprintf(buffer,"           ldi     lbl_%d.1                 ; point variable data",
      varLabel);
    Asm(buffer);
    Asm("           phi     r9");
    sprintf(buffer,"           ldi     lbl_%d.0", varLabel);
    Asm(buffer);
    Asm("           plo     r9");
    Asm("           inc     r7                  ; Retrieve LUN");
    Asm("           lda     r7");
    Asm("           phi     ra");
    Asm("           inc     r7");
    Asm("           inc     r7");
    Asm("           sep     scall               ; Process write list");
    Asm("           dw      fmtwrite");
    sprintf(buffer,"           lbr     lbl_%d",eolLabel);
    Asm(buffer);

/* ***** Need processing for ERR=, et ***** */

    sprintf(buffer,"lbl_%d:",varLabel);
    Asm(buffer);
    if (*line != ')') {
      showError("Syntax error");
      return;
      }
    line++;
    while (*line != 0) {
      line = getVarName(line, token);
      if (strlen(token) == 0) {
        showError("Invalid variable name");
        return;
        }
      v = getVariable(token, module);
      if (v < 0) {
        showError("Invalid variable name");
        return;
        }
      sprintf(buffer,"           db      %d",varType(v)); 
      Asm(buffer);
      sprintf(buffer,"           dw      %s_%s",
        variables[v].module, variables[v].name);
      Asm(buffer);
      if (*line != ',' && *line != 0) {
        showError("Syntax error");
        return;
        }
      if (*line == ',') line++;
      }
    Asm("           db      0");
    sprintf(buffer,"lbl_%d:",eolLabel);
    Asm(buffer);
    addDefine("FMTWRITE",1,1);
    }
  else {
    /* ***************************** */
    /* ***** Unformatted write ***** */
    /* ***************************** */
    }
  }

