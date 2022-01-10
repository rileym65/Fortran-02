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

void cdata(char* line) {
  char token[32];
  int  vars[256];
  int  numVars;
  int  i,j;
  int  pos;
  int  v;
  int  var;
  int  count;
  int  isFloat;
  int  flag;
  dword value;
  INTREAL ir;
  while (*line != 0) {
    numVars = 0;
    pos = 0;
    flag = -1;
    while (flag) {
      if ((*line >= 'a' && *line <= 'z') ||
          (*line >= 'A' && *line <= 'Z') ||
          (*line >= '0' && *line <= '9') ||
           *line == '_') {
        if (*line >= '0' && *line <= '9' && pos == 0) {
          showError("Syntax error");
          return;
          }
        token[pos++] = *line;
        }
      else if (*line == ',' || *line == '/' || *line == 0) {
        token[pos] = 0;
        if (pos == 0) {
          showError("Syntax error");
          return;
          }
        v = getVariable(token, module);
        if (v < 0) return;
        vars[numVars++] = v;
        pos = 0;
        }
      if (*line == 0 || *line == '/') flag = 0;
        else line++;
      }
    if (*line != '/') {
      showError("Syntax error");
      return;
      }
    line++;
    var = 0;
    pos = 0;
    count = 1;
    isFloat = 0;
    flag = -1;
    while (flag) {
      if (*line == '\'') {
        line++;
        pos = 0;
        token[pos++] = '\'';
        while (*line != '\'' && *line != 0) {
          token[pos++] = *line++;
          }
        }
      else if ((*line == 'x' || *line == 'X' || *line == 'z' || *line == 'Z') &&
               *(line+1) == '\'') {
        pos = 0;
        token[pos++] = 'X';
        line += 2;
        while ((*line >= 'a' && *line <= 'f') ||
               (*line >= 'A' && *line <= 'F') ||
               (*line >= '0' && *line <= '9')) {
          token[pos++] = *line++;
          }
        token[pos] = 0;
        if (*line != '\'') {
          showError("Syntax error");
          return;
          }
        }
      else if (*line == '-') {
        token[pos++] = *line;
        }
      else if (*line >= '0' && *line <= '9') {
        token[pos++] = *line;
        }
      else if (*line == '.') {
        token[pos++] = *line;
        isFloat = -1;
        }
      else if (*line == '*') {
        token[pos] = 0;
        if (isFloat || pos == 0) {
          showError("Syntax error");
          return;
          }
        count = atoi(token);
        pos = 0;
        }
      else if (*line == ',' || *line == '/' || *line == 0) {
        token[pos] = 0;
        if (token[0] == '\'') {
          ir.integer = 0;
          for (j=1; j<strlen(token); j++)
            ir.integer = (ir.integer << 8) | token[j];
          }
        else if (token[0] == 'X') {
          ir.integer = 0;
          for (j=1; j<strlen(token); j++) {
            if (token[j] >= 'a' && token[j] <= 'f') 
              ir.integer = (ir.integer << 4) + (token[j] - 87);
            if (token[j] >= 'A' && token[j] <= 'F') 
              ir.integer = (ir.integer << 4) + (token[j] - 55);
            if (token[j] >= '0' && token[j] <= '9') 
              ir.integer = (ir.integer << 4) + (token[j] - '0');
            }
          }
        else if (isFloat) ir.real = atof(token);
        else ir.integer = atoi(token);
        for (i=0; i<count; i++) {
          if (var >= numVars) {
            showError("Too many values specified");
            return;
            }
          variables[vars[var++]].value = ir.integer;
          }
        pos = 0;
        count = 1;
        if (*line == '/' || *line == 0) {
          flag = 0;
          }
        }
      else {
        showError("Syntax error");
        return;
        }
      if (flag) line++;




      }
    if (*line == '/') line++;
    }
  }

