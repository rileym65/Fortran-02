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

void appendLine(char* dest, char* src) {
  int qt;
  src += 6;
  qt = 0;
  while (*dest != 0) dest++;
  while (*src != 0) {
    if (strncasecmp(src, ".lt.", 4) == 0) {
      *dest++ = '<';
      src += 3;
      }
    else if (strncasecmp(src, ".gt.", 4) == 0) {
      *dest++ = '>';
      src += 3;
      }
    else if (strncasecmp(src, ".le.", 4) == 0) {
      *dest++ = '<';
      *dest++ = '=';
      src += 3;
      }
    else if (strncasecmp(src, ".ge.", 4) == 0) {
      *dest++ = '>';
      *dest++ = '=';
      src += 3;
      }
    else if (strncasecmp(src, ".eq.", 4) == 0) {
      *dest++ = '=';
      src += 3;
      }
    else if (strncasecmp(src, ".ne.", 4) == 0) {
      *dest++ = '<';
      *dest++ = '>';
      src += 3;
      }
    else if (strncasecmp(src, ".or.", 4) == 0) {
      *dest++ = '|';
      src += 3;
      }
    else if (strncasecmp(src, ".and.", 5) == 0) {
      *dest++ = '&';
      src += 4;
      }
    else if (strncasecmp(src, ".xor.", 5) == 0) {
      *dest++ = '^';
      src += 4;
      }
    else if (strncasecmp(src, ".not.", 5) == 0) {
      *dest++ = '!';
      src += 4;
      }
    else if (strncasecmp(src, ".true.", 6) == 0) {
      *dest++ = '-';
      *dest++ = '1';
      src += 5;
      }
    else if (strncasecmp(src, ".false.", 7) == 0) {
      *dest++ = '0';
      src += 6;
      }
    else if (*src == ' ' && qt) {
      *dest++ = *src;
      }
    else if (*src == '\'') {
      qt = (qt) ? 0 : 1;
      *dest++ = *src;
      }
    else if (*src != 0 && *src <= ' ') ;
    else *dest++ = *src;
    src++;
    }
  *dest = 0;
  }

int nextStatement() {
  int i;
  int valid;
  int first;
  int flag;
  int pos;
  char label[10];
  strcpy(statement, "");
  statementLabel = 0;
  first = -1;
  flag = -1;
  while (flag) {
    while (strlen(nextLine) < 7) {
      if (feof(source)) {
        if (strlen(statement) > 0) return -1;
        return 0;
        }
      fgets(nextLine, 1023, source);
      lineCount++;
      if (strlen(nextLine) > 0) {
        if (nextLine[0] == 'c' || nextLine[0] == 'C')
          strcpy(nextLine, "");
        valid = 0;
        for (i=0; i<strlen(nextLine); i++)
          if (nextLine[i] > ' ') valid = -1;
        if (valid == 0)
          strcpy(nextLine, "");
        }
      }
    if (first) {
      strcpy(initialLine, nextLine);
      }
    if (first != 0 && nextLine[5] != ' ' && nextLine[5] != '0') {
      showError("Unexpectadly encountered continuation line");
      return 0;
      }
    if (first != 0 || (nextLine[5] != ' ' && nextLine[5] != '0')) {
      pos = 0;
      for (i=0; i<5; i++) {
        if (nextLine[i] >= '0' && nextLine[i] <= '9')
          label[pos++] = nextLine[i];
        else if (nextLine[i] != ' ') {
          showError("Unexpected character in label field");
          return 0;
          }
        }
      label[pos] = 0;
      if (pos > 0) statementLabel = atoi(label);
      appendLine(statement, nextLine);
      strcpy(nextLine, "");
      }
    else flag = 0;
    first = 0;
    }
  return -1;
  }

