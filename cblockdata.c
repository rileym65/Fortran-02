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

void cblockdata(char* line) {
  char token[32];
  int  pos;
  if (inUnit) {
    showError("Not allowed inside program unit");
    return;
    }
  inUnit = -1;
  inBlockData = -1;
  pos = 0;
  while ((*line >= 'a' && *line <= 'z') ||
         (*line >= 'A' && *line <= 'Z') ||
         (*line >= '0' && *line <= '9') ||
          *line == '_') token[pos++] = *line++;
  if (*line != 0) {
    showError("Syntax error");
    return;
    }
  token[pos] = 0;
  if (strlen(token) == 0) strcpy(token,"BLOCKDATA");
  strcpy(module, token);
printf("module: %s\n",module);
  }

