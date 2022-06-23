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

void copen(char* line, byte flags) {
  int  reclen;
  int  labelFName;
  int  labelJump;
  int  pos;
  if (!useElfos) {
    showError("Elf/OS mode not enabled");
    return;
    }

  line = getArg(line, 'd', "set LUN");
  if (line == NULL) return;
  Asm("              stxd                     ; save for now");
  if (*line != ',') {
    showError("Syntax error");
    return;
    }
  line++;
  if (*line != '\'') {
    showError("Syntax error");
    return;
    }
  line++;
  labelFName = nextLabel++;
  labelJump = nextLabel++;
  sprintf(buffer,"              lbr   lbl_%d             ; Jump past filename",labelJump);
  Asm(buffer);
  sprintf(buffer,"lbl_%d:       db    '", labelFName);
  pos = strlen(buffer);
  while (*line != 0 && *line != '\'') buffer[pos++] = *line++;
  buffer[pos] = 0;
  strcat(buffer,"',0");
  Asm(buffer);
  if (*line != '\'') {
    showError("Syntax error");
    return;
    }
  line++;
  sprintf(buffer,"lbl_%d:", labelJump);
  Asm(buffer);
  if (*line == ',') {
    line++;
    line = getArg(line, 'd', "Set record length");
    if (line == NULL) return;
    }
  else {
    reclen = 128;
    sprintf(buffer,"              ldi   %d                 ; Set record length", reclen);
    Asm(buffer);
    }
  Asm("              plo   rc");
  sprintf(buffer,"              ldi   lbl_%d.1           ; Point to filename",labelFName);
  Asm(buffer);
  Asm("              phi   rf");
  sprintf(buffer,"              ldi   lbl_%d.0",labelFName);
  Asm(buffer);
  Asm("              plo   rf");
  sprintf(buffer,"              ldi   %d           ; File flags",flags);
  Asm(buffer);
  Asm("              plo   r8");
  Asm("              irx                      ; Recover LUN");
  Asm("              ldx");
  Asm("              sep   scall              ; Open the file");
  Asm("              dw    fopen");
  addExtrn("fopen");
  }

