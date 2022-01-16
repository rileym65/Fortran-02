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

char varType(int varNum) {
  if (variables[varNum].type == V_BYTE) return 'B';
  if (variables[varNum].type == V_LOGICAL) return 'L';
  if (variables[varNum].type == V_SHORT) return 'S';
  if (variables[varNum].type == V_INTEGER) return 'I';
  if (variables[varNum].type == V_REAL) return 'R';
  if (variables[varNum].name[0] >= 'i' && variables[varNum].name[0] <= 'n') return 'I';
  if (variables[varNum].name[0] >= 'I' && variables[varNum].name[0] <= 'N') return 'I';
  return 'R';
  }

