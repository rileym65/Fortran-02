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

void csubroutine(char* line) {
  char token[32];
  int  pos;
  if (inUnit) {
    showError("Not allowed inside program unit");
    return;
    }
  inUnit = -1;
  inSub = -1;
  pos = 0;
  while ((*line >= 'a' && *line <= 'z') ||
         (*line >= 'A' && *line <= 'Z') ||
         (*line >= '0' && *line <= '9') ||
          *line == '_') token[pos++] = *line++;
  if (*line != 0) {
    showError("Syntax error");
    return;
    }
  if (pos == 0) {
    showError("NULL name not allowed");
    return;
    }
  token[pos] = 0;
  strcpy(module, token);
  sprintf(buffer,"%s:", token); Asm(buffer);
  }

