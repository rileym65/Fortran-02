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

void cassign(char* line) {
  char token[32];
  int  pos;
  int  lnum;
  int  v;
  checkMain();
  if (*line < '0' || *line > '9') {
    showError("Syntax error");
    return;
    }
  lnum = 0;
  while (*line >= '0' && *line <= '9') {
    lnum = (lnum * 10) + (*line - '0');
    line++;
    }
  if (lnum == 0 || lnum > 65535) {
    showError("Invalid line label");
    return;
    }
  if (strncasecmp(line, "to", 2) != 0) {
    showError("Syntax error");
    return;
    }
  line += 2;
  pos = 0;
  while ((*line >= 'a' && *line <= 'z') ||
         (*line >= 'A' && *line <= 'Z') ||
         (*line >= '0' && *line <= '9') ||
         *line == '_') token[pos++] = *line++;
  token[pos] = 0;
  if (pos == 0) {
    showError("Syntax error");
    return;
    }
  if (*line != 0) {
    showError("Syntax error");
    return;
    }
  v = getVariable(token, module);
  if (v < 0) return;
  if (variables[v].type == V_BYTE ||
      variables[v].type == V_LOGICAL ||
      variables[v].type == V_REAL) {
    showError("Invalid variable type");
    return;
    }
  if (variables[v].type == V_DEFAULT) {
    if ((variables[v].name[0] >= 'a' && variables[v].name[0] <= 'h') ||
        (variables[v].name[0] >= 'o' && variables[v].name[0] <= 'z') ||
        (variables[v].name[0] >= 'A' && variables[v].name[0] <= 'H') ||
        (variables[v].name[0] >= 'O' && variables[v].name[0] <= 'Z')) {
      showError("Invalid variable type");
      return;
      }
    }
  if (strlen(variables[v].common) > 0) {
    sprintf(buffer,"            ldi  (c_%s+%d).1          ; Point to variable",
            variables[v].common,
            variables[v].offset);
    Asm(buffer);
    Asm("            phi  rf");
    sprintf(buffer,"            ldi  (c_%s+%d).0",
            variables[v].common,
            variables[v].offset);
    Asm(buffer);
    Asm("            plo  rf");
    }
  else {
    sprintf(buffer,"            ldi  %s_%s.1          ; Point to variable",
            variables[v].module,
            variables[v].name);
    Asm(buffer);
    Asm("            phi  rf");
    sprintf(buffer,"            ldi  %s_%s.0",
            variables[v].module,
            variables[v].name);
    Asm(buffer);
    Asm("            plo  rf");
    }
  if (variables[v].type == V_INTEGER ||
      variables[v].type == V_DEFAULT) {
    Asm("            ldi  0          ; clear high word");
    Asm("            str  rf");
    Asm("            inc  rf");
    Asm("            str  rf");
    Asm("            inc  rf");
    }
  sprintf(buffer,"            ldi  %s_%d.1          ; Point to line",
          variables[v].module,
          lnum);
  Asm(buffer);
  Asm("            str  rf");
  Asm("            inc  rf");
  sprintf(buffer,"            ldi  %s_%d.0          ; Point to line",
          variables[v].module,
          lnum);
  Asm(buffer);
  Asm("            str  rf");
return;

  if (*line != 0) {
    showError("Invalid character encountered in END statement");
    return;
    }
  if (useStg) {
    Asm("            ldi  STG_.1");
    Asm("            phi  rf");
    Asm("            ldi  STG_.0");
    Asm("            plo  rf");
    Asm("            lda  rf");
    Asm("            phi  r2");
    Asm("            ldn  rf");
    Asm("            plo  r2");
    Asm("            sex  r2");
    Asm("            irx");
    Asm("            ldxa");
    Asm("            plo  r6");
    Asm("            ldx");
    Asm("            phi  r6");
    Asm("            sep  sret");
    }
  else if (exitAddress != 0xffff) {
    sprintf(buffer,"          lbr   0%xh                    ; Jump to exit address",exitAddress); Asm(buffer);
    }
  else if (useElfos) {
    Asm("          lbr   0303h                   ; Jump to exit address");
    }
  else {
    Asm("          idl                           ; Idle the CPU");
    Asm("          lbr   $-1");
    }
  }

