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

void cexternal(char* line) {
  int i;
  char token[32];
  int  pos;
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  while (*line != 0) {
    if ((*line >= 'a' && *line <= 'z') ||
        (*line >= 'A' && *line <= 'Z')) {
      pos = 0;
      while ((*line >= 'a' && *line <= 'z') ||
             (*line >= 'A' && *line <= 'Z') ||
             (*line >= '0' && *line <= '9') ||
              *line == '_') token[pos++] = *line++;
      token[pos] = 0;
      for (i=0; i<numExternals; i++) {
        if (strcasecmp(externals[i].name, token) == 0) {
          showError("Duplicate external");
          return;
          }
        }
      strcpy(externals[numExternals].name, token);
      externals[numExternals].type = V_DEFAULT;
      numExternals++;
      if (*line != 0 && *line != ',') {
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
  }

