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
  int  isArray;
  char varname[256];
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  line = trim(line);
  if (!(*line >= 'a' && *line <= 'z') && !(*line >= 'A' && *line <= 'Z')) {
    showError("Invalid variable name");
    *line = 0;
    }
  pos = 0;
  fp = 0;
  st = 0;
  isArray = 0;
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
/* *********************************** */
/* ***** Process array reference ***** */
/* *********************************** */
  if (*line == '(') {
    v = getVariable(varname, module);
     if (v < 0) return;
    line = arrayRef(line, v);
    if (line == NULL) return;
    Asm("           ghi     rf                        ; Save offset for later");
    Asm("           stxd");
    Asm("           glo     rf");
    Asm("           stxd");
    isArray = -1;
    }
/* ********************************** */
/* ***** Now process expression ***** */
/* ********************************** */
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
    if (fp) line = cexpr(line, 1, module);
      else line = cexpr(line, 0, module);
    if (exprErrors > 0) return;
    line = trim(line);
    if (*line != 0) {
      showError("Syntax error");
      *line = 0;
      }

    if (variables[v].isArg) {
      sprintf(buffer,"          ldi   (%s).1              ; Get destination variable address",
              varname); Asm(buffer);
      Asm("          phi   rf");
      sprintf(buffer,"          ldi   (%s).0", varname); Asm(buffer);
      Asm("          plo   rf");
      Asm("          lda   rf                        ; retrieve pointed to address");
      Asm("          plo   re");
      Asm("          lda   rf");
      Asm("          plo   rf");
      Asm("          glo   re");
      Asm("          phi   rf");
      if (varType(v) == 'I' || varType(v) == 'R') {
        Asm("          inc   rf                        ; point to LSB");
        Asm("          inc   rf");
        Asm("          inc   rf");
        }
      if (varType(v) == 'S') {
        Asm("          inc   rf                        ; point to LSB");
        }
      }
    else if (strlen(variables[v].common) > 0) {
      if (varType(v) == 'I' || varType(v) == 'R') {
        sprintf(buffer,"          ldi   (c_%s+%d+3).1              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (c_%s+%d+3).0              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          plo   rf");
        }
      if (varType(v) == 'S') {
        sprintf(buffer,"          ldi   (c_%s+%d+1).1              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (c_%s+%d+1).0              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          plo   rf");
        }
      if (varType(v) == 'B' || varType(v) == 'L') {
        sprintf(buffer,"          ldi   (c_%s+%d).1              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (c_%s+%d).0              ; Get destination variable address", variables[v].common,variables[v].offset); Asm(buffer);
        Asm("          plo   rf");
        }
      }
    else {
      if (varType(v) == 'I' || varType(v) == 'R') {
        sprintf(buffer,"          ldi   (%s+3).1              ; Get destination variable address", varname); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (%s+3).0", varname); Asm(buffer);
        Asm("          plo   rf");
        }
      if (varType(v) == 'S') {
        sprintf(buffer,"          ldi   (%s+1).1              ; Get destination variable address", varname); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (%s+1).0", varname); Asm(buffer);
        Asm("          plo   rf");
        }
      if (varType(v) == 'B' || varType(v) == 'L') {
        sprintf(buffer,"          ldi   (%s).1              ; Get destination variable address", varname); Asm(buffer);
        Asm("          phi   rf");
        sprintf(buffer,"          ldi   (%s).0", varname); Asm(buffer);
        Asm("          plo   rf");
        }
      }

    if (isArray) {
      Asm("          irx                             ; Add offset");
      Asm("          glo   rf");
      Asm("          add");
      Asm("          plo   rf");
      Asm("          irx");
      Asm("          ghi   rf");
      Asm("          adc");
      Asm("          phi   rf");
      }


    if (varType(v) == 'I' || varType(v) == 'R') {
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
    if (varType(v) == 'S') {
      Asm("          inc   r7                        ; Write expresison result to variable");
      Asm("          lda   r7");
      Asm("          str   rf");
      Asm("          dec   rf");
      Asm("          lda   r7");
      Asm("          str   rf");
      Asm("          inc   r7");
      Asm("          inc   r7");
      }
    if (varType(v) == 'B' || varType(v) == 'L') {
      Asm("          inc   r7                        ; Write expresison result to variable");
      Asm("          lda   r7");
      Asm("          str   rf");
      Asm("          inc   r7");
      Asm("          inc   r7");
      Asm("          inc   r7");
      }
    }
  }

