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

void ccommon(char* line) {
  char token[32];
  int  dims[3];
  int  dimCount;
  char area[32];
  int  pos;
  int  i;
  int  c;
  int  flag;
  int  vsize;
  flag = -1;
  pos = 0;
  checkMain();
  if (passNumber != 1) return;
  strcpy(area," ");
  while (flag) {
    if (*line == '/') {
      line++;
      pos = 0;
      while (*line != '/' && *line != 0) {
        area[pos++] = *line++;
        }
      area[pos] = 0;
      if (pos == 0) strcpy(area, "_");
      if (*line == '/') line++;
      pos = 0;
      }
    else if (*line >= 'a' && *line <= 'z') token[pos++] = *line++;
    else if (*line >= 'A' && *line <= 'Z') token[pos++] = *line++;
    else if (*line >= '0' && *line <= '9') token[pos++] = *line++;
    else if (*line == '_') token[pos++] = *line++;
    else if (*line == ',' || *line == 0 || *line == '(') {
      token[pos] = 0;
      if (pos == 0) {
        showError("Expected variable name not given");
        return;
        }
      pos = 0;
      i = findVariable(token, module);
      if (i < 0) i = addVariable(token, module);
      strcpy(variables[i].common, area);
      c = getCommon(area, module);
      switch (varType(i)) {
        case 'B': vsize=1; break;
        case 'L': vsize=1; break;
        case 'S': vsize=2; break;
        default : vsize=4; break;
        }
      if (*line == '(') {
        line++;
        dimCount = 0;
        dims[0] = 0; dims[1] = 0; dims[2] = 0;
        while (*line != ')' && *line != 0) {
          if (*line >= '0' && *line <= '9') {
            dims[dimCount] = (dims[dimCount] * 10) + (*line - '0');
            line++;
            }
          else if (*line == ',') {
            if (dims[dimCount] == 0) {
              showError("Array dimension of 0 is not allowed");
              return;
              }
            if (dimCount == 2) {
              showError("Dimensions beyond 3 are not supported");
              return;
              }
            dimCount++;
            line++;
            }
          else {
            showError("Invalid character in array specification");
            return;
            }
          }
        variables[i].dimensions = dimCount+1;
        variables[i].sizes[0] = dims[0];
        variables[i].sizes[1] = dims[1];
        variables[i].sizes[2] = dims[2];
        if (dimCount == 0) vsize = vsize * dims[0];
        if (dimCount == 1) vsize = vsize * dims[0] * dims[1];
        if (dimCount == 2) vsize = vsize * dims[0] * dims[1] * dims[2];
        }
      variables[i].offset = common[c].size;
      common[c].size += vsize;
      if (common[c].size > common[c].maxSize) common[c].maxSize = common[c].size;
      if (*line == ')') line++;
      if (*line == ',') line++;
      else if (*line == 0) flag = 0;
      }
    else {
      showError("Invalid character found in variable name");
      return;
      }
    }
  }

