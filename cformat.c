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

void cformat(char* line) {
  int count;
  int size1;
  int size2;
  char string[256];
  int pos;
  char ftype;
  if (*line != '(') {
    showError("Syntax error");
    return;
    }
  line++;
  sprintf(buffer,"           lbr     lbl_%d",nextLabel);
  Asm(buffer);
  while (*line != ')' && *line != 0) {
    if (*line == '\'') {
      pos = 0;
      line++;
      while (*line != 0 && *line != '\'') string[pos++] = *line++;
      string[pos] = 0;
      count = strlen(string);
      sprintf(buffer,"           db      %d,'H%s'",count,string);
      Asm(buffer);
      if (*line == '\'') line++;
      }
    else {
      if (*line >= '0' && *line <= '9') {
        count = 0;
        while (*line >= '0' && *line <= '9') {
          count = (count * 10) + (*line - '0');
          line++;
          }
        }
      else count = 1;
      ftype = *line++;
      if (ftype >= 'a' && ftype <= 'z') ftype -= 32;
      if (ftype != 'I' && ftype != 'X' && ftype != 'A' && ftype != 'L' &&
          ftype != 'F') {
        showError("Invalid format specifier");
        return;
        }
      size2 = -1;
      if (*line >= '0' && *line <= '9') {
        size1 = 0;
        while (*line >= '0' && *line <= '9') {
          size1 = (size1 * 10) + (*line - '0');
          line++;
          }
        }
      else {
        size1 = 1;
        }
      if (*line == '.') {
        if (ftype != 'F') {
          showError("Invalid specifier");
          return;
          }
        size2 = 0;
        line++;
        while (*line >= '0' && *line <= '9') {
          size2 = (size2 * 10) + (*line - '0');
          line++;
          }
        }
      else size2 = 0xff;
      if (ftype == 'F' && size2 == 0xff) size2 = 0;
      sprintf(buffer,"           db      %d,%d,%d,%d",count,ftype,size1,size2);
      Asm(buffer);
      }
    if (*line != ',' && *line != ')') {
      showError("Syntax error");
      return;
      }
    if (*line == ',') line++;
    }
  Asm("           db      0");
  sprintf(buffer,"lbl_%d:",nextLabel++);
  Asm(buffer);
  }

