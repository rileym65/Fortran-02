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

void creturn(char* line) {
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  if (inSub == 0) {
    showError("Invalid use of RETURN");
    return;
    }
  if (*line != 0) {
    showError("Syntax error");
    return;
    }
  Asm("              sep   sret               ; Return to caller");
  }

