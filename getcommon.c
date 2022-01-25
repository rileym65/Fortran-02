/*
 *******************************************************************
 *** This software is copyright 2021 by Michael H Riley          ***
 *** You have permission to use, modify, copy, and distribute    ***
 *** this software so long as this copyright notice is retained. ***
 *** This software may not be used in commercial applications    ***
 *** without express written permission from the author.         ***
 *******************************************************************
*/

#include <stdlib.h>
#include "header.h"

word getCommon(char* name, char* module) {
  int i;
  for (i=0; i<numCommon; i++) {
    if (strcasecmp(name, common[i].name) == 0) return i;
    }
  numCommon++;
  if (numCommon == 1)
    common = (COMMON*)malloc(sizeof(COMMON));
  else
    common = (COMMON*)realloc(common, sizeof(COMMON) * numCommon);
  strcpy(common[numCommon-1].name, name);
  common[numCommon-1].numVariables = 0;
  common[numCommon-1].size = 0;
  common[numCommon-1].maxSize = 0;
  return numCommon-1;
  }

