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

/* ******************************************* */
/* **** Dest:                            ***** */
/* **** 'd' - argument to D              ***** */
/* **** 'e' - Argument to expr stack     ***** */
/* **** 'r' - Argument to Register r(uc) ***** */
/* **** Returns position after arg, or   ***** */
/* ****  NULL on error                   ***** */
/* ******************************************* */
char* getArg(char*line, char dest, char* rem) {
  char token[32];
  int  pos;
  int  value;
  int  v;
  int  ofs;
  char argType;

  if (*line >= '0' && *line <= '9') {
    argType = 'N';
    value = 0;
    while (*line >= '0' && *line <= '9') {
      value = (value * 10) + (*line - '0');
      line++;
      }
    }

  else if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    argType = 'V';
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    token[pos] = 0;
    }

  else {
    showError("Syntax error");
    return NULL;
    }

  if (argType == 'V') {
    v = getVariable(token, module);
    if (v < 0) return NULL;
    if (varType(v) == 'R') {
      showError("Invalid variable type");
      return NULL;
      }
    switch (varType(v)) {
      case 'B': ofs = 0; break;
      case 'L': ofs = 0; break;
      case 'S': ofs = 1; break;
      case 'I': ofs = 3; break;
      default : ofs = 3; break;
      }
    if (dest == 'e') ofs = 0;
    sprintf(buffer,"              ldi   (%s_%s+%d).1        ; Point to variable",
      variables[v].module, variables[v].name, ofs);
    Asm(buffer);
    Asm("              phi   rf");
    sprintf(buffer,"              ldi   (%s_%s+%d).0",
      variables[v].module, variables[v].name, ofs);
    Asm(buffer);
    Asm("              plo   rf");
    }

  if (dest == 'd') {
    if (argType == 'N') {
      if (value <0 || value > 255) {
        showError("Value is out of range");
        return NULL;
        }
      sprintf(buffer,"              ldi   %d'",value & 0xff);
      if (rem != NULL && strlen(rem) > 0) {
        strcat(buffer,"                 ; ");
        strcat(buffer,rem);
        }
      Asm(buffer);
      }
    else {
      sprintf(buffer,"              lda   rf");
      if (rem != NULL && strlen(rem) > 0) {
        strcat(buffer,"                 ; ");
        strcat(buffer,rem);
        }
      Asm(buffer);
      }
    return line;
    }

  if (dest == 'e') {
    if (argType == 'N') {
      sprintf(buffer, "              sex   r7");
      if (rem != NULL && strlen(rem) > 0) {
        strcat(buffer,"                 ; ");
        strcat(buffer,rem);
        }
      Asm(buffer);
      sprintf(buffer,"              ldi   %d", (value >> 24) & 0xff);
      Asm(buffer);
      Asm("              stxd");
      sprintf(buffer,"              ldi   %d", (value >> 16) & 0xff);
      Asm(buffer);
      Asm("              stxd");
      sprintf(buffer,"              ldi   %d", (value >> 8) & 0xff);
      Asm(buffer);
      Asm("              stxd");
      sprintf(buffer,"              ldi   %d", value & 0xff);
      Asm(buffer);
      Asm("              stxd");
      Asm("              sex   r2");
      return line;
      }
    else {
      switch (varType(v)) {
        case 'B':
        case 'L':
             Asm("              sex   r7");
             Asm("              ldi   0");
             Asm("              stxd");
             Asm("              stxd");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              sex   r2");
             break;
        case 'S':
             Asm("              sex   r7");
             Asm("              ldi   0");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              sex   r2");
             break;
        default:
             Asm("              sex   r7");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              lda   rf");
             Asm("              stxd");
             Asm("              sex   r2");
             break;
        }
      }
    return line;
    }

  return line;
  }

