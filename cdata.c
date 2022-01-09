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
  int  i;
  int  pos;
  int  v;
  int  var;
  int  count;
  int  isFloat;
  int  flag;
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
      if (*line == '-') {
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
        for (i=0; i<count; i++) {
          if (isFloat) ir.real = atof(token);
            else ir.integer = atoi(token);
          if (var >= numVars) {
            showError("Too many values specified");
            return;
            }
          variables[vars[var++]].value = ir.integer;
          }
        pos = 0;
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

