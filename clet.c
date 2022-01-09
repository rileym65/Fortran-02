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

void clet(char* line) {
  int  pos;
  int  fp;
  int  st;
  int  v;
  char varname[256];
  line = trim(line);
  if (!(*line >= 'a' && *line <= 'z') && !(*line >= 'A' && *line <= 'Z')) {
    showError("Invalid variable name");
    *line = 0;
    }
  pos = 0;
  fp = 0;
  st = 0;
  while ((*line >= 'a' && *line <= 'z') ||
         (*line >= 'A' && *line <= 'Z') ||
         (*line >= '0' && *line <= '9') ||
         *line == '_') {
    varname[pos++] = *line++;
    if (*line == '$') {
      varname[pos++] = *line++;
      st = -1;
      if ((*line >= 'a' && *line <= 'z') ||
          (*line >= 'A' && *line <= 'Z') ||
          (*line >= '0' && *line <= '9') ||
          *line == '_') {
          showError("Invalid variable name");
          *line = 0;
        }
      }
    if (useFp) {
      if (*line == '!') {
        varname[pos++] = *line++;
        fp = -1;
        if ((*line >= 'a' && *line <= 'z') ||
            (*line >= 'A' && *line <= 'Z') ||
            (*line >= '0' && *line <= '9') ||
            *line == '_') {
            showError("Invalid variable name");
            *line = 0;
          }
        }
      }
    }
  varname[pos] = 0;
  line = trim(line);
  if (*line != '=') {
    showError("Syntax error");
    *line = 0;
    }
  line++;
  line = trim(line);
  if (st) {
    }
  else {
    v = getVariable(varname, module);
    if (v < 0) return;
    if (variables[v].type == V_REAL) fp = -1;
    if (variables[v].type == V_DEFAULT) {
      fp = -1;
      if (variables[v].name[0] >= 'i' && variables[v].name[0] <= 'n') fp = 0;
      if (variables[v].name[0] >= 'I' && variables[v].name[0] <= 'N') fp = 0;
      }
    if (fp) line = cexpr(line, 1);
      else line = cexpr(line, 0);
    if (exprErrors > 0) return;
    line = trim(line);
    if (*line != 0) {
      showError("Syntax error");
      *line = 0;
      }
    sprintf(buffer,"          ldi   (%s_%s+3).1              ; Get destination variable address", module,varname); Asm(buffer);
    Asm("          phi   rf");
    sprintf(buffer,"          ldi   (%s_%s+3).0", module,varname); Asm(buffer);
    Asm("          plo   rf");
    Asm("          inc   r7                        ; Write expresison result to variable");
    Asm("          lda   r7");
    Asm("          str   rf");
    Asm("          dec   rf");
    Asm("          lda   r7");
    Asm("          str   rf");
    Asm("          dec   rf");
    Asm("          lda   r7");
    Asm("          str   rf");
    Asm("          dec   rf");
    Asm("          ldn   r7");
    Asm("          str   rf");
    }
  }

