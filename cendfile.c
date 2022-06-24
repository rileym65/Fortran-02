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

void cendfile(char* line) {
  char token[32];
  int  pos;
  int  lun;
  int  v;
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  lun = 0;
  if (*line >= '0' && *line <= '9') {
    while (*line >= '0' && *line <= '9') {
      lun = (lun * 10) + (*line - '0');
      line++;
      }
    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    if (lun < 1 || lun > 4) {
      showError("Invalid LUN specified");
      return;
      }
    sprintf(buffer,"              ldi   %d                 ; Point to LUN'",lun);
    Asm(buffer);

    }
  else if ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    if (pos == 0 || *line != 0) {
      showError("Syntax error");
      return;
      }
    token[pos] = 0;
    v = getVariable(token, module);
    if (v < 0) return;
    if (varType(v) == 'R') {
      showError("Invalid variable type");
      return;
      }
    if (varType(v) == 'I') {
      sprintf(buffer,"              ldi   (%s+3).1        ; Point variable",
        variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s+3).0",
        variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    else if (varType(v) == 'S') {
      sprintf(buffer,"              ldi   (%s+1).1        ; Point variable",
        variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s+1).0",
        variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    else {
      sprintf(buffer,"              ldi   (%s).1          ; Point variable",
        variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s).0",
        variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    Asm("              lda   rf                 ; Retrieve lsb");
    }
  else {
    showError("Syntax error");
    return;
    }
  Asm("              sep   scall              ; Close the file");
  Asm("              dw    fclose");
  addExtrn("fclose");

  }

