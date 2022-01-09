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

char* getVarName(char *line, char* token) {
  int pos;
  token[0] = 0;
  if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z') ||
      *line == '_') {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    token[pos] = 0;
    }
  return line;
  }

