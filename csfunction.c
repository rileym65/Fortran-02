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

int csfunction(char* line) {
  int  i;
  char token[32];
  char subname[64];
  int  fp;
  int  pos;
  int  skip;
  pos = 0;
  while ((*line >= 'a' && *line <= 'z') ||
         (*line >= 'A' && *line <= 'Z') ||
         (*line >= '0' && *line <= '9') ||
          *line == '_') token[pos++] = *line++;
  if (*line != '(') {
    return 0;
    }
  line++;
  if (pos == 0) {
    return 0;
    }
  token[pos] = 0;
  i = findVariable(token, module);
  if (i >= 0) return 0;
  checkMain();
  strcpy(subname, module);
  strcat(subname, "_");
  strcat(subname, token);
  if ((subname[0] >= 'i' && subname[0] <= 'n') ||
      (subname[0] >= 'I' && subname[0] <= 'N')) fp = 0;
    else fp = -1;
  skip = nextLabel++;
  strcpy(externals[numExternals].name, subname);
  externals[numExternals].type = V_DEFAULT;
  numExternals++;
  sprintf(buffer,"              lbr   lbl_%d             ; jump past subroutine",skip);
  Asm(buffer);
  sprintf(buffer,"%s:",subname); Asm(buffer);
  Asm("              sep   scall              ; Bind parameters");
  Asm("              dw    fenter");
  addExtrn("fenter");
  while (*line != 0 && *line != ')') {
    if ((*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')) {
      pos = 0;
      while ((*line >= 'a' && *line <= 'z') ||
             (*line >= 'A' && *line <= 'Z') ||
             (*line >= '0' && *line <= '9') ||
              *line == '_') token[pos++] = *line++;
      token[pos] = 0;
      if (passNumber == 1)
        i = addVariable(token, subname);
      else
        i = getVariable(token, subname);
      if (i < 0) return -1;
      variables[i].isArg = 0xff;
      sprintf(buffer,"              dw    %s_%s",
        variables[i].module, variables[i].name);
      Asm(buffer);
      if (*line != ',' && *line != 0 && *line != ')') {
        showError("Syntax error");
        return -1;
        }
      if (*line == ',') line++;
      }
    else {
      showError("Syntax error");
      return -1;
      }
    }
  Asm("              dw    0");
  if (*line != ')') {
    showError("Syntax error");
    return -1;
    }
  line++;
  if (*line != '=') {
    showError("Syntax error");
    return -1;
    }
  line++;
  if (fp) line = cexpr(line, 1, subname);
    else line = cexpr(line, 0, subname);
  if (exprErrors > 0) return -1;
  Asm("              sep   sret");
  sprintf(buffer,"lbl_%d:",skip); Asm(buffer);
  return -1;
  }





/*
  line++;
  while (*line != 0 && *line != ')') {
    if ((*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')) {
      pos = 0;
      while ((*line >= 'a' && *line <= 'z') ||
             (*line >= 'A' && *line <= 'Z') ||
             (*line >= '0' && *line <= '9') ||
              *line == '_') token[pos++] = *line++;
      token[pos] = 0;
      if (passNumber == 1)
        i = addVariable(token, module);
      else
        i = getVariable(token, module);
      if (i < 0) return;
      variables[i].isArg = 0xff;
      sprintf(buffer,"              dw    %s_%s",
        variables[i].module, variables[i].name);
      Asm(buffer);
      if (*line != ',' && *line != 0 && *line != ')') {
        showError("Syntax error");
        return;
        }
      if (*line == ',') line++;
      }
    else {
      showError("Syntax error");
      return;
      }
    }
  Asm("              dw    0");
  addExtrn("fenter");
  }
*/
