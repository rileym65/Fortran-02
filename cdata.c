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
  int  counts[256];
  int  totalSlots;
  int  numVars;
  int  i,j;
  int  pos;
  int  v;
  int  c;
  int  var;
  int  ofs;
  int  count;
  int  isFloat;
  int  flag;
  int  ct;
  int  ap;
  INTREAL ir;
  checkMain();
//  Asm("          sep   scall                    ; Call data initializer");
//  Asm("          dw    fdata");
  while (*line != 0) {
    numVars = 0;
    totalSlots = 0;
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
        if (variables[v].dimensions == 0) {
          counts[numVars-1] = 1;
          totalSlots += 1;
          }
        if (variables[v].dimensions == 1) {
          ct = variables[v].sizes[0];
          counts[numVars-1] = ct;
          totalSlots += ct;
          }
        if (variables[v].dimensions == 2) {
          ct = variables[v].sizes[0] * variables[v].sizes[1];
          counts[numVars-1] = ct;
          totalSlots += ct;
          }
        if (variables[v].dimensions == 3) {
          ct = variables[v].sizes[0] * variables[v].sizes[1] * variables[v].sizes[2];
          counts[numVars-1] = ct;
          totalSlots += ct;
          }
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
    ap = 0;
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
          for (j=1; j<strlen(token); j++) {
            ir.integer = (ir.integer << 8) | token[j];
            }
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
        isFloat = 0;
        for (i=0; i<count; i++) {
          if (var >= totalSlots) {
            showError("Too many values specified");
            return;
            }
          variables[vars[var]].value = ir.integer;
          if (variables[vars[var]].dimensions != 0) {
            if (variables[vars[var]].values == NULL) {
              if (variables[vars[var]].dimensions == 1)
                variables[vars[var]].values =
                 (dword*)malloc(variables[vars[var]].sizes[0] * sizeof(dword));
              if (variables[vars[var]].dimensions == 2)
                variables[vars[var]].values =
                 (dword*)malloc(variables[vars[var]].sizes[0] *
                                variables[vars[var]].sizes[1] * sizeof(dword));
              if (variables[vars[var]].dimensions == 3)
                variables[vars[var]].values =
                 (dword*)malloc(variables[vars[var]].sizes[0] *
                                variables[vars[var]].sizes[1] *
                                variables[vars[var]].sizes[2] * sizeof(dword));
              ap = 0;
              }
            variables[vars[var]].values[ap++] = ir.integer;
            }
          if (strlen(variables[vars[var]].common) > 0) {
            ofs = variables[vars[var]].offset;
            c = getCommon(variables[vars[var]].common,module);
            if (c < 0) {
              showError("COMMON block not found");
              return;
              }
            switch (varType(vars[var])) {
              case 'B':
              case 'L':
                   common[c].data[ofs] = ir.integer & 0xff;
                   break;
              case 'S':
                   common[c].data[ofs++] = (ir.integer >> 8) & 0xff;
                   common[c].data[ofs]   = ir.integer & 0xff;
                   break;
              case 'I':
              case 'R':
                   common[c].data[ofs++] = (ir.integer >> 24) & 0xff;
                   common[c].data[ofs++] = (ir.integer >> 16) & 0xff;
                   common[c].data[ofs++] = (ir.integer >> 8) & 0xff;
                   common[c].data[ofs]   = ir.integer & 0xff;
                   break;
              }
            }
          counts[var]--;
          if (counts[var] == 0) {
            var++;
            ap = 0;
            }
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
//  Asm("          dw    0                        ; End the list");
//  addDefine("FDATA",1,1);
  }

