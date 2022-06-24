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

void cend(char* line) {
  int i;
  inUnit = 0;
  inBlockData = 0;
  functionVar = -1;
  if (*line != 0) {
    showError("Invalid character encountered in END statement");
    return;
    }
  if (inSub == 0) {
    if (useStg) {
      Asm("            ldi  STG_.1");
      Asm("            phi  rf");
      Asm("            ldi  STG_.0");
      Asm("            plo  rf");
      Asm("            lda  rf");
      Asm("            phi  r2");
      Asm("            ldn  rf");
      Asm("            plo  r2");
      Asm("            sex  r2");
      Asm("            irx");
      Asm("            ldxa");
      Asm("            plo  r6");
      Asm("            ldx");
      Asm("            phi  r6");
      Asm("            sep  sret");
      }
    else if (exitAddress != 0xffff) {
      sprintf(buffer,"          lbr   0%xh                    ; Jump to exit address",exitAddress); Asm(buffer);
      }
    else if (useElfos) {
      Asm("          lbr   0303h                   ; Jump to exit address");
      }
    else {
      Asm("          idl                           ; Idle the CPU");
      Asm("          lbr   $-1");
      }
    }
  for (i=0; i<numCommon; i++) {
    common[i].numVariables = 0;
    common[i].size = 0;
    }
  moduleEnd();
  if (inSub) {
    Asm("           endp");
    }
  inSub = 0;
  }

