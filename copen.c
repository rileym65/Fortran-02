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

void copen(char* line) {
  int  i;
  int  v;
  int  reclen;
  int  labelFName;
  int  labelJump;
  char token[32];
  int  pos;
  if (!useElfos) {
    showError("Elf/OS mode not enabled");
    return;
    }
  if (*line >= '0' && *line <= '9') {
    i = 0;
    while (*line >= '0' && *line <= '9') {
      i = (i * 10) + (*line - '0');
      line++;
      }
    sprintf(buffer,"              ldi   %d                 ; Point to LUN'",i);
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
      sprintf(buffer,"              ldi   (%s_%s+3).1        ; Point variable",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s_%s+3).0",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    else if (varType(v) == 'S') {
      sprintf(buffer,"              ldi   (%s_%s+1).1        ; Point variable",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s_%s+1).0",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    else {
      sprintf(buffer,"              ldi   (%s_%s).1          ; Point variable",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              phi   rf");
      sprintf(buffer,"              ldi   (%s_%s).0",
        variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("              plo   rf");
      }
    Asm("              lda   rf                 ; Retrieve lsb");
    }
    else {
      showError("Syntax error");
      return;
      }
  Asm("              stxd                     ; save for now");
  if (*line != ',') {
    showError("Syntax error");
    return;
    }
  line++;
  if (*line != '\'') {
    showError("Syntax error");
    return;
    }
  line++;
  labelFName = nextLabel++;
  labelJump = nextLabel++;
  sprintf(buffer,"              lbr   lbl_%d             ; Jump past filename",labelJump);
  Asm(buffer);
  sprintf(buffer,"lbl_%d:       db    '", labelFName);
  pos = strlen(buffer);
  while (*line != 0 && *line != '\'') buffer[pos++] = *line++;
  buffer[pos] = 0;
  strcat(buffer,"',0");
  Asm(buffer);
  if (*line != '\'') {
    showError("Syntax error");
    return;
    }
  line++;
  sprintf(buffer,"lbl_%d:", labelJump);
  Asm(buffer);
  if (*line == ',') {
    line++;
    reclen = 0;
    while (*line >='0' && *line <= '9') {
      reclen = (reclen * 10) + (*line - '0');
      line++;
      }
    if (reclen == 0 || reclen > 256) {
      showError("Invalid record length");
      return;
      }
    sprintf(buffer,"              ldi   %d                 ; Set record length", reclen);
    Asm(buffer);
    Asm("              plo   rc");
    if (reclen > iBufferSize) iBufferSize = reclen + 1;
    }
  else {
    reclen = 128;
    sprintf(buffer,"              ldi   %d                 ; Set record length", reclen);
    Asm(buffer);
    Asm("              plo   rc");
    if (reclen > iBufferSize) iBufferSize = reclen + 1;
    }
  sprintf(buffer,"              ldi   lbl_%d.1           ; Point to filename",labelFName);
  Asm(buffer);
  Asm("              phi   rf");
  sprintf(buffer,"              ldi   lbl_%d.0",labelFName);
  Asm(buffer);
  Asm("              plo   rf");
  Asm("              irx                      ; Recover LUN");
  Asm("              ldx");
  Asm("              sep   scall              ; Open the file");
  Asm("              dw    fopen");
  addDefine("FOPEN",1,1);
  }

