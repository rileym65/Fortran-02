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

void cdo(char* line) {
  char token[32];
  char start[32];
  char end[32];
  char step[32];
  int  pos;
  int  v;
  int  vstep;
  int  lnum;
  char *argret;
  dword value;
  checkMain();
  lnum = 0;
  if (*line < '0' || *line > '9') {
    showError("Line number expected");
    return;
    }
  while (*line >= '0' && *line <= '9') {
    lnum = (lnum * 10) + (*line - '0');
    line++;
    }
  if (lnum < 1 || lnum > 65535) {
    showError("Line number out of range");
    return;
    }
  if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    token[pos] = 0;
    v = getVariable(token, module);
    if (v < 0) return;
    if (variables[v].type == V_REAL) {
      showError("Invalid variable type");
      return;
      }
    }
  else {
    showError("Variable name expected");
    return;
    }
  doLoops[numDoLoops].line = lnum;
  doLoops[numDoLoops].variable = v;
  doLoops[numDoLoops].loopStart = nextLabel++;
  if (*line != '=') {
    showError("Syntax error");
    return;
    }
  line++;
  strcpy(step,"1");
  pos = 0;
  if ((*line >= '0' && *line <= '9') || *line == '-') {
    if (*line == '-') start[pos++] = *line++;
    while (*line >= '0' && *line <= '9') start[pos++] = *line++;
    start[pos] = 0;
    }
  else if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') start[pos++] = *line++;
    start[pos] = 0;
    }
  else {
    showError("Syntax error");
    return;
    }
  if (*line != ',') {
    showError("Syntax error");
    return;
    }
  line++;
  pos = 0;
  if ((*line >= '0' && *line <= '9') || *line == '-') {
    if (*line == '-') end[pos++] = *line++;
    while (*line >= '0' && *line <= '9') end[pos++] = *line++;
    end[pos] = 0;
    }
  else if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') end[pos++] = *line++;
    end[pos] = 0;
    }
  else {
    showError("Syntax error");
    return;
    }
  if (*line == ',') {
    line++;
    pos = 0;
    if (*line >= '0' && *line <= '9') {
      while (*line >= '0' && *line <= '9') step[pos++] = *line++;
      step[pos] = 0;
      }
    else if ((*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')) {
      pos = 0;
      while ((*line >= 'a' && *line <= 'z') ||
             (*line >= 'A' && *line <= 'Z') ||
             (*line >= '0' && *line <= '9') ||
             *line == '_') step[pos++] = *line++;
      step[pos] = 0;
      }
    else {
      showError("Syntax error");
      return;
      }
    }
  if (*line != 0) {
    showError("End of line expected");
    return;
    }

  /* *************************************************** */
  /* ***** Case where all parameters are constants ***** */
  /* *************************************************** */
  if ( (start[0] == '-' || (start[0] >= '0' && start[0] <= '9')) &&
       (end[0] == '-' || (end[0] >= '0' && end[0] <= '9')) &&
       (step[0] >= '0' && step[0] <= '9') ) {
    doLoops[numDoLoops].varStep = 'N';
    doLoops[numDoLoops].loops = ((atoi(end) - atoi(start)) / atoi(step)) + 1;
    doLoops[numDoLoops].step = atoi(step);
    value = atoi(start);
    if (strlen(variables[v].common) > 0) {
      sprintf(buffer,"          ldi   (c_%s+%d).1                 ; point to variable",variables[v].common, variables[v].offset);
      Asm(buffer);
      Asm("          phi   rf");
      sprintf(buffer,"          ldi   (c_%s+%d).0",variables[v].common, variables[v].offset);
      Asm(buffer);
      Asm("          plo   rf");
      }
    else {
      sprintf(buffer,"          ldi   (%s_%s).1                 ; point to variable",variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("          phi   rf");
      sprintf(buffer,"          ldi   (%s_%s).0",variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("          plo   rf");
      }
    if (varType(v) == 'I') {
      sprintf(buffer,"          ldi   %d                 ; Write start value", (value >> 24) & 0xff);
      Asm(buffer);
      Asm("          str   rf");
      Asm("          inc   rf");
      sprintf(buffer,"          ldi   %d", (value >> 16) & 0xff);
      Asm(buffer);
      Asm("          str   rf");
      Asm("          inc   rf");
      }
    if (varType(v) == 'I' || varType(v) == 'S') {
      sprintf(buffer,"          ldi   %d                 ; Write start value", (value >> 8) & 0xff);
      Asm(buffer);
      Asm("          str   rf");
      Asm("          inc   rf");
      }
    sprintf(buffer,"          ldi   %d                 ; Write start value", value & 0xff);
    Asm(buffer);
    Asm("          str   rf");
    if (varType(v) == 'I') {
      sprintf(buffer,"          ldi   %d                 ; Write loops to stack", (doLoops[numDoLoops].loops >> 24) & 0xff);
      Asm(buffer);
      Asm("          stxd");
      sprintf(buffer,"          ldi   %d", (doLoops[numDoLoops].loops >> 16) & 0xff);
      Asm(buffer);
      Asm("          stxd");
      }
    if (varType(v) == 'I' || varType(v) == 'S') {
      sprintf(buffer,"          ldi   %d                 ; Write loops to stack", (doLoops[numDoLoops].loops >> 8) & 0xff);
      Asm(buffer);
      Asm("          stxd");
      }
    sprintf(buffer,"          ldi   %d                 ; Write loops to stack", doLoops[numDoLoops].loops & 0xff);
    Asm(buffer);
    Asm("          stxd");
    }
  /* ************************************************** */
  /* ***** Case where any parameter is a variable ***** */
  /* ************************************************** */
  else {
    if (step[0] >= '0' && step[0] <= '9') {
      doLoops[numDoLoops].varStep = 'N';
      doLoops[numDoLoops].step = atoi(step);
      }
    else {
      doLoops[numDoLoops].varStep = 'V';
      vstep = getVariable(step, module);
      if (vstep < 0) return;
      doLoops[numDoLoops].step = vstep;
      }
    argret = getArg(end, 'e', "Push end to expr stack");
    if (argret == NULL) return;
    argret = getArg(start, 'e', "Push start to expr stack");
    if (argret == NULL) return;
    if (strlen(variables[v].common) > 0) {
      sprintf(buffer,"          ldi   (c_%s+%d).1                 ; point to variable",variables[v].common, variables[v].offset);
      Asm(buffer);
      Asm("          phi   rf");
      sprintf(buffer,"          ldi   (c_%s+%d).0",variables[v].common, variables[v].offset);
      Asm(buffer);
      Asm("          plo   rf");
      }
    else {
      sprintf(buffer,"          ldi   (%s_%s).1                 ; point to variable",variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("          phi   rf");
      sprintf(buffer,"          ldi   (%s_%s).0",variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("          plo   rf");
      }
    switch (varType(v)) {
      case 'B':
      case 'L':
           Asm("              inc   r7                 ; point to start");
           Asm("              ldn   r7                 ; Retrieve LSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              dec   r7                 ; restore R7");
           break;
      case 'S':
           Asm("              inc   r7                 ; point to start");
           Asm("              inc   r7");
           Asm("              ldn   r7                 ; Retrieve MSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              inc   rf");
           Asm("              dec   r7");
           Asm("              ldn   r7                 ; Retrieve LSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              dec   r7                 ; restore R7");
           break;
      case 'I':
           Asm("              inc   r7                 ; point to start");
           Asm("              inc   r7");
           Asm("              inc   r7");
           Asm("              inc   r7");
           Asm("              ldn   r7                 ; Retrieve MSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              inc   rf");
           Asm("              dec   r7");
           Asm("              ldn   r7                 ; Retrieve MSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              inc   rf");
           Asm("              dec   r7");
           Asm("              ldn   r7                 ; Retrieve MSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              inc   rf");
           Asm("              dec   r7");
           Asm("              ldn   r7                 ; Retrieve LSB");
           Asm("              str   rf                 ; store into variable");
           Asm("              dec   r7                 ; restore R7");
           break;
      }
    Asm("              sep   scall              ; subtract");
    Asm("              dw    sub32");
    addDefine("SUB32",1,1);
    argret = getArg(step, 'e', "Push step to expr stack");
    if (argret == NULL) return;
    Asm("              sep   scall              ; subtract");
    Asm("              dw    div32");
    addDefine("DIV32",1,1);
    Asm("              sep   scall              ; Add 1");
    Asm("              dw    epush");
    Asm("              db    0,0,0,1");
    Asm("              sep   scall              ; and add");
    Asm("              dw    add32");
    addDefine("ADD32",1,1);
    Asm("              inc   r7                 ; retrieve loops");
    Asm("              lda   r7");
    Asm("              plo   rb");
    Asm("              lda   r7");
    Asm("              phi   rb");
    Asm("              lda   r7");
    Asm("              plo   ra");
    Asm("              lda   r7");
    Asm("              phi   ra");
    switch(varType(v)) {
      case 'B':
      case 'L':
           Asm("              glo   rb                 ; push to stack");
           Asm("              stxd");
           break;
      case 'S':
           Asm("              ghi   rb                 ; push to stack");
           Asm("              stxd");
           Asm("              glo   rb");
           Asm("              stxd");
           break;
      case 'I':
           Asm("              ghi   ra                 ; push to stack");
           Asm("              stxd");
           Asm("              glo   ra");
           Asm("              stxd");
           Asm("              ghi   rb");
           Asm("              stxd");
           Asm("              glo   rb");
           Asm("              stxd");
           break;
      }
    }
  sprintf(buffer, "lbl_%d:",doLoops[numDoLoops].loopStart); Asm(buffer);
  numDoLoops++;
  }

void cDoEnd() {
  int v,v2;
  int s1,s2;
  int i;
  v = doLoops[numDoLoops-1].variable;
  switch (varType(v)) {
    case 'B': 
    case 'L': i = 0; s1 = 1; break;
    case 'S': i = 1; s1 = 2; break;
    default : i = 3; s1 = 4; break;
    }
  if (strlen(variables[v].common) > 0) {
    sprintf(buffer,"          ldi   (c_%s+%d+%d).1                 ; point to variable",
            variables[v].common, variables[v].offset, i);
    Asm(buffer);
    Asm("          phi   rf");
    sprintf(buffer,"          ldi   (c_%s+%d+%d).0                 ; point to variable",
            variables[v].common, variables[v].offset, i);
    Asm(buffer);
    Asm("          plo   rf");
    }
  else {
    sprintf(buffer,"          ldi   (%s_%s+%d).1                 ; point to variable",
            variables[v].module, variables[v].name, i);
    Asm(buffer);
    Asm("          phi   rf");
    sprintf(buffer,"          ldi   (%s_%s+%d).0                 ; point to variable",
            variables[v].module, variables[v].name, i);
    Asm(buffer);
    Asm("          plo   rf");
    }

  if (doLoops[numDoLoops-1].varStep == 'N') {
    Asm("          ldn   rf                       ; get byte from variable");
    sprintf(buffer,"          adi   %d                 ; add step",doLoops[numDoLoops-1].step & 0xff);
    Asm(buffer);
    Asm("          str   rf                       ; put it back");
    if (varType(v) == 'S' || varType(v) == 'I') {
      Asm("          dec   rf                       ; point to next byte");
      Asm("          ldn   rf                       ; get byte from variable");
      sprintf(buffer,"          adci  %d                 ; add step",(doLoops[numDoLoops-1].step >> 8) & 0xff);
      Asm(buffer);
      Asm("          str   rf                       ; put it back");
      }
    if (varType(v) == 'I') {
      Asm("          dec   rf                       ; point to next byte");
      Asm("          ldn   rf                       ; get byte from variable");
      sprintf(buffer,"          adci  %d                 ; add step",(doLoops[numDoLoops-1].step >> 16) & 0xff);
      Asm(buffer);
      Asm("          str   rf                       ; put it back");
      Asm("          dec   rf                       ; point to next byte");
      Asm("          ldn   rf                       ; get byte from variable");
      sprintf(buffer,"          adci  %d                 ; add step",(doLoops[numDoLoops-1].step >> 24) & 0xff);
      Asm(buffer);
      Asm("          str   rf                       ; put it back");
      }
    }
  else {
    v2 = doLoops[numDoLoops-1].step;
    if (v2 < 0) return;
    switch (varType(v2)) {
      case 'B': 
      case 'L': i = 0; s2 = 1; break;
      case 'S': i = 1; s2 = 2; break;
      default : i = 3; s2 = 4; break;
      }
    if (strlen(variables[v2].common) > 0) {
      sprintf(buffer,"          ldi   (c_%s+%d+%d).1                 ; point to variable",
              variables[v2].common, variables[v2].offset, i);
      Asm(buffer);
      Asm("          phi   rd");
      sprintf(buffer,"          ldi   (c_%s+%d+%d).0                 ; point to variable",
              variables[v2].common, variables[v2].offset, i);
      Asm(buffer);
      Asm("          plo   rd");
      }
    else {
      sprintf(buffer,"          ldi   (%s_%s+%d).1                 ; point to variable",
              variables[v2].module, variables[v2].name, i);
      Asm(buffer);
      Asm("          phi   rd");
      sprintf(buffer,"          ldi   (%s_%s+%d).0                 ; point to variable",
              variables[v2].module, variables[v2].name, i);
      Asm(buffer);
      Asm("          plo   rd");
      }
    for (i=0; i<s1; i++) {
      if (s2 > 0) {
        Asm("          ldn   rd");
        Asm("          str   r2");
        Asm("          ldn   rf");
        if (i == 0)
          Asm("          add");
        else
          Asm("          adc");
        Asm("          str   rf");
        if (i < s2-1) {
          Asm("          dec   rf");
          Asm("          dec   rd");
          }
        }
      else {
        Asm("          ldn   rf");
        Asm("          adci  0");
        Asm("          str   rf");
        if (i < s2-1) 
          Asm("          dec   rf");
        }
      }
    
    }
  Asm("          ghi   r2                       ; copy stack pointer to rf");
  Asm("          phi   rf");
  Asm("          glo   r2");
  Asm("          plo   rf");
  Asm("          inc   rf                       ; point to lsb of loops count");
  Asm("          ldn   rf                       ; get lsb of loops count");
  Asm("          smi   1                        ; and decrement");
  Asm("          str   rf                       ; put it back");
  if (varType(v) == 'S' || varType(v) == 'I') {
    Asm("          inc   rf                       ; point to next byte");
    Asm("          str   r2                       ; store value for zero test");
    Asm("          ldn   rf                       ; get next byte of loops count");
    Asm("          smbi  0                        ; propagate carry");
    Asm("          str   rf                       ; put it back");
    Asm("          or                             ; combine with zero test");
    }
  if (varType(v) == 'I') {
    Asm("          inc   rf                       ; point to next byte");
    Asm("          str   r2                       ; store value for zero test");
    Asm("          ldn   rf                       ; get next byte of loops count");
    Asm("          smbi  0                        ; propagate carry");
    Asm("          str   rf                       ; put it back");
    Asm("          or                             ; combine with zero test");
    Asm("          inc   rf                       ; point to next byte");
    Asm("          str   r2                       ; store value for zero test");
    Asm("          ldn   rf                       ; get next byte of loops count");
    Asm("          smbi  0                        ; propagate carry");
    Asm("          str   rf                       ; put it back");
    Asm("          or                             ; combine with zero test");
    }
  sprintf(buffer,"          lbnz  lbl_%d                   ; Loop back if not done", doLoops[numDoLoops-1].loopStart);
  Asm(buffer);
  Asm("          inc   r2                       ; remove loops from stack");
  if (varType(v) == 'S' || varType(v) == 'I') {
    Asm("          inc   r2");
    }
  if (varType(v) == 'I') {
    Asm("          inc   r2");
    Asm("          inc   r2");
    }

  }

