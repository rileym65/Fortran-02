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
    if (strcasecmp(name, common[i].name) == 0 &&
        strcasecmp(module, common[i].module) == 0) return i;
    }
  numCommon++;
  if (numCommon == 1)
    common = (COMMON*)malloc(sizeof(COMMON));
  else
    common = (COMMON*)realloc(common, sizeof(COMMON) * numCommon);
  strcpy(common[numCommon-1].name, name);
  strcpy(common[numCommon-1].module, module);
  common[numCommon-1].numVariables = 0;
  for (i=0; i<numCommonBlocks; i++)
    if (strcasecmp(name, commonBlocks[i].name) == 0) return numCommon-1;
  numCommonBlocks++;
  if (numCommonBlocks == 1)
    commonBlocks = (COMMONBLOCK*)malloc(sizeof(COMMONBLOCK));
  else
    commonBlocks = (COMMONBLOCK*)realloc(commonBlocks, sizeof(COMMONBLOCK) * numCommonBlocks);
  strcpy(commonBlocks[numCommonBlocks-1].name, name);
  return numCommon-1;
  }

