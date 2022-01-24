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

void cread(char* line) {
  char token[32];
  int  pos;
  int  v;
  int  size;
  int  varLabel;
  int  eolLabel;
  int  fline;
  int  rec;
  int  eof;
  int  err;
  rec = -1;
  eof = -1;
  err = -1;
  fline = -1;
  if (*line != '(') {
    showError("Syntax error");
    return;
    }
  line++;
  if (*line == '*') {
    Asm("           ldi     0                   ; Push LUN 5 to expr stack");
    Asm("           sex     r7");
    Asm("           stxd");
    Asm("           stxd");
    Asm("           stxd");
    Asm("           ldi     5");
    Asm("           stxd");
    Asm("           sex     r2");
    line++;
    }
  else {
    line = cexpr(line,0);
    if (exprErrors > 0) return;
    }
  while (*line != 0 && *line != ')') {
    if (*line != ',') {
      showError("Syntax error");
      return;
      }
    line++;

    if (strncasecmp(line, "rec=",4) == 0) {
      if (rec >= 0) {
        showError("Duplicate REC=");
        return;
        }
      line += 4;
      rec = 0;
      if (*line < '0' || *line > '9') {
        showError("Syntax error");
        return;
        }
      while (*line >= '0' && *line <= '9') {
        rec = (rec * 10) + (*line - '0');
        line++;
        }
      }

    else if (strncasecmp(line, "err=",4) == 0) {
      if (err >= 0) {
        showError("Duplicate ERR=");
        return;
        }
      line += 4;
      err = 0;
      if (*line < '0' || *line > '9') {
        showError("Syntax error");
        return;
        }
      while (*line >= '0' && *line <= '9') {
        err = (err * 10) + (*line - '0');
        line++;
        }
      if (err == 0) {
        showError("Syntax error");
        return;
        }
      }

    else if (strncasecmp(line, "end=",4) == 0) {
      if (eof >= 0) {
        showError("Duplicate END=");
        return;
        }
      line += 4;
      eof = 0;
      if (*line < '0' || *line > '9') {
        showError("Syntax error");
        return;
        }
      while (*line >= '0' && *line <= '9') {
        eof = (eof * 10) + (*line - '0');
        line++;
        }
      if (eof == 0) {
        showError("Syntax error");
        return;
        }
      }

    else if (*line == '*') {
      if (fline != -1) {
        showError("Duplicate FORMAT line");
        return;
        }
      fline = -999;
      line++;
      }

    else if (*line >= '0' && *line <= '9') {
      if (fline != -1) {
        showError("Duplicate FORMAT line");
        return;
        }
      fline = 0;
      if (*line < '0' || *line > '9') {
        showError("Syntax error");
        return;
        }
      while (*line >= '0' && *line <= '9') {
        fline = (fline * 10) + (*line - '0');
        line++;
        }
      if (fline == 0) {
        showError("Syntax error");
        return;
        }
      }

    }

  if (*line != ',' && *line != ')') {
    showError("Syntax error");
    return;
    }
  line++;

  varLabel = nextLabel++;
  eolLabel = nextLabel++;
  size = 0;
  sprintf(buffer,"           lbr     lbl_%d",eolLabel);
  Asm(buffer);
  sprintf(buffer,"lbl_%d:",varLabel);
  Asm(buffer);
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
    switch (varType(v)) {
      case 'B':
      case 'L': size += 1; break;
      case 'S': size += 2; break;
      case 'I':
      case 'R': size += 4;  break;
      }
    sprintf(buffer,"           db      %d",varType(v));
    Asm(buffer);
    sprintf(buffer,"           dw      %s_%s",variables[v].module, variables[v].name);
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
  if (fline > 0) {
    sprintf(buffer,"           ldi     (%s_%d+3).1                 ; point to format data",
      module, fline);
    Asm(buffer);
    Asm("           phi     r8");
    sprintf(buffer,"           ldi     (%s_%d+3).0", module, fline);
    Asm(buffer);
    Asm("           plo     r8");
    }
  else {
    sprintf(buffer,"           ldi     %d                       ; Number of bytes to read",size/256);
    Asm(buffer);
    Asm("           phi     rc");
    sprintf(buffer,"           ldi     %d",size%256);
    Asm(buffer);
    Asm("           plo     rc");
    }
  sprintf(buffer,"           ldi     lbl_%d.1                 ; point variable data", varLabel);
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
  if (fline > 0) {
    Asm("           dw      fmtread");
    addDefine("FMTREAD",1,1);
    }
  else if (fline == -999) {
    Asm("           dw      ffread");
    addDefine("FFREAD",1,1);
    }
  else {
    Asm("           dw      uread");
    addDefine("UREAD",1,1);
    }
  if (err > 0) {
    sprintf(buffer,"           lbdf    %s_%d                 ; Jump on error",module, err);
    Asm(buffer);
    }
  if (eof > 0) {
    sprintf(buffer,"           lbz     %s_%d                 ; Jump on eof",module, eof);
    Asm(buffer);
    }
  }

