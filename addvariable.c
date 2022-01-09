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

int addVariable(char* name,char* module) {
  int i;
  i = findVariable(name, module);
  if (i >= 0) {
    showError("Variable already defined");
    return -1;
    }
  numVariables++;
  if (numVariables == 1)
    variables = (VARREC*)malloc(sizeof(VARREC));
  else
    variables = (VARREC*)realloc(variables, sizeof(VARREC) * numVariables);
  strcpy(variables[numVariables-1].name, name);
  strcpy(variables[numVariables-1].module, module);
  strcpy(variables[numVariables-1].common, "");
  variables[numVariables-1].commonVar = 0;
  variables[numVariables-1].offset = 0;
  variables[numVariables-1].type = V_DEFAULT;
  variables[numVariables-1].dimensions = 0;
  variables[numVariables-1].value = 0;
  return numVariables-1;
  }

