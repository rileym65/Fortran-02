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

int validVar(char *line) {
  if (*line >= 'a' && *line <= 'z') return -1;
  if (*line >= 'A' && *line <= 'Z') return -1;
  return 0;
  }

