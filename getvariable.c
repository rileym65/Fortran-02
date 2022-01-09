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

int getVariable(char* name, char* module) {
  int i;
  i = findVariable(name, module);
  if (i >= 0) return i;
  i = addVariable(name, module);
  return i;
  }

