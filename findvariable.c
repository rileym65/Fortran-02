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

int findVariable(char* name,char* module) {
  int i;
  for (i=0; i<numVariables; i++) {
    if (strcasecmp(name, variables[i].name) == 0 &&
        strcasecmp(module, variables[i].module) == 0)
      return i;
    }
  return -1;
  }

