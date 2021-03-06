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
  int i;
  int count;
  int size1;
  int size2;
  int repeats;
  char string[256];
  char *restart;
  int pos;
  char ftype;
  if (inBlockData) {
    showError("Not allowed in BLOCK DATA");
    return;
    }
  checkMain();
  if (*line != '(') {
    showError("Syntax error");
    return;
    }
  line++;
  sprintf(buffer,"           lbr     lbl_%d",nextLabel);
  Asm(buffer);
  repeats = 0;
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
      if (*line == ',') line++;
      }
    else if (*line == '/') {
        while (*line == '/') {
          Asm("           db      1,'/',0,0");
          line++;
          }
        if (*line == ',') line++;
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
      if (ftype == '(') {
        restart = line;
        repeats = count;
        }
      else {
        if (ftype >= 'a' && ftype <= 'z') ftype -= 32;
        if (ftype != 'I' && ftype != 'X' && ftype != 'A' && ftype != 'L' &&
            ftype != 'F' && ftype != 'H') {
          showError("Invalid format specifier");
          return;
          }
        if (ftype == 'H') {
          pos = 0;
          for (i=0; i<count; i++) string[pos++] = *line++;
          string[pos] = 0;
          sprintf(buffer,"           db      %d,'H%s'",count,string);
          Asm(buffer);
          }
        else {
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


        if (*line != ',' && *line != ')' && *line != '/') {
          showError("Syntax error");
          return;
          }
        if (*line == ')' && repeats > 0) {
          repeats--;
          if (repeats > 0) line = restart;
          else {
            line++;
            if (*line == ',') line++;
            }
          }
        if (*line == ',') line++;
  
        }

      }
    }
  Asm("           db      0");
  sprintf(buffer,"lbl_%d:",nextLabel++);
  Asm(buffer);
  }

