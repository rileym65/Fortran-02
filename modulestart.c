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

void moduleStart() {
  char m[256];
  char buffer[256];
  int i;
  char *pchar;
  strcpy(m, module);
  strcat(m, "|");
  for (i=0; i<numExtrns; i++) {
    if (strncmp(m, extrns[i], strlen(m)) == 0) {
      pchar = strchr(extrns[i], '|') + 1;
      sprintf(buffer,"           extrn   %s",pchar);
      Asm(buffer);
      }
    }
  }

