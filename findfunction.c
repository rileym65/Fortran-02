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

int findFunction(char* name) {
  int i;
  char local[64];
  for (i=0; i<numExternals; i++) {
    if (strcasecmp(name, externals[i].name) == 0)
      return i;
    }
  strcpy(local,module);
  strcat(local,"_");
  strcat(local,name);
  for (i=0; i<numExternals; i++) {
    if (strcasecmp(local, externals[i].name) == 0)
      return i;
    }
  return -1;
  }

