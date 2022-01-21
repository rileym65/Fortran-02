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

void cgoto(char* line) {
  char token[32];
  char buffer[256];
  int  lines[1024];
  int  numLines;
  int  pos;
  int  i;
  int  v;
  int  value;
  int  finalLabel;
  int  goLabel;
  int  noLabel;

  /* ************************************** */
  /* ***** Handle jump to line number ***** */
  /* ************************************** */
  if (*line >= '0' && *line <= '9') {
    pos = 0;
    while (*line >= '0' && *line <= '9') {
      pos = (pos * 10) + (*line - '0');
      line++;
      }
    if (*line != 0) {
      showError("Invalid character encountered in GO TO");
      return;
      }
    sprintf(buffer,"            lbr  %s_%d",module,pos); Asm(buffer);
    return;
    }

  /* ********************************* */
  /* ***** Handle assigned GO TO ***** */
  /* ********************************* */
  if ((*line >= 'a' && *line <= 'z') ||
      (*line >= 'A' && *line <= 'Z')) {
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    token[pos] = 0;
    if (pos == 0) {
      showError("Syntax error");
      return;
      }
    v = findVariable(token, module);
    if (v < 0) {
      showError("Variable not defined");
      return;
      }
    if (variables[v].type == V_BYTE ||
        variables[v].type == V_LOGICAL ||
        variables[v].type == V_REAL) {
      showError("Invalid variable type");
      return;
      }
    if (variables[v].type == V_DEFAULT) {
      if ((variables[v].name[0] >= 'a' && variables[v].name[0] <= 'h') ||
          (variables[v].name[0] >= 'o' && variables[v].name[0] <= 'z') ||
          (variables[v].name[0] >= 'A' && variables[v].name[0] <= 'H') ||
          (variables[v].name[0] >= 'O' && variables[v].name[0] <= 'Z')) {
        showError("Invalid variable type");
        return;
        }
      }

    /* ************************************************ */
    /* ***** Assigned GO TO without qualification ***** */
    /* ************************************************ */
    if (*line == 0) {
      sprintf(buffer,"          ldi   lbl_%d.1                ; Point to jump point", nextLabel);
      Asm(buffer);
      Asm("            phi  ra");
      sprintf(buffer,"          ldi   lbl_%d.0", nextLabel);
      Asm(buffer);
      Asm("            plo  ra");
      Asm("            sep  ra");
      sprintf(buffer,"lbl_%d:   ldi   %s_%s.1                ; Point to variable",
              nextLabel++, variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("            phi  rf");
      sprintf(buffer,"          ldi   %s_%s.0",
              variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("            plo  rf");
      if (variables[v].type == V_INTEGER ||
          variables[v].type == V_DEFAULT) {
        Asm("            inc  rf");
        Asm("            inc  rf");
        }
      Asm("            lda  rf");
      Asm("            phi  r3");
      Asm("            ldn  rf");
      Asm("            plo  r3");
      Asm("            sep  r3");
      return;
      }

    /* ********************************************** */
    /* ***** Assigned GO TO witht qualification ***** */
    /* ********************************************** */
    if (*line == ',') {
      line++;
      if (*line != '(') {
        showError("Syntax error");
        return;
        }
      line++;
      sprintf(buffer,"          ldi   %s_%s.1                ; Point to variable",
              variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("            phi  ra");
      sprintf(buffer,"          ldi   %s_%s.0",
              variables[v].module, variables[v].name);
      Asm(buffer);
      Asm("            plo  ra");
      Asm("            inc  ra");
      Asm("            inc  ra");
      Asm("            lda  ra");
      Asm("            phi  rf");
      Asm("            ldn  ra");
      Asm("            plo  rf");
      finalLabel = nextLabel++;
      goLabel = nextLabel++;
      while (*line != 0 && *line != ')') {
        if (*line >= '0' && *line <= '9') {
          value = 0;
          while (*line >= '0' && *line <= '9') {
            value = (value * 10) + (*line - '0');
            line++;
            }
          if (value == 0) {
            showError("Invalid line number");
            return;
            }
          noLabel = (*line == ',') ? nextLabel++ : finalLabel;
          Asm("          ghi   rf                      ; Check line number");
          sprintf(buffer,"          smi   %s_%d.1", module, value); Asm(buffer);
          sprintf(buffer,"          lbnz  lbl_%d", noLabel); Asm(buffer);
          Asm("          glo   rf");
          sprintf(buffer,"          smi   %s_%d.0", module, value); Asm(buffer);
          sprintf(buffer,"          lbz   lbl_%d", goLabel); Asm(buffer);
          if (*line == ',') {
            line++;
            sprintf(buffer,"lbl_%d:",noLabel); Asm(buffer);
            }
          else {
            sprintf(buffer,"          lbr   lbl_%d", finalLabel); Asm(buffer);
            }
          }
        }
      if (*line != ')') {
        showError("Syntax error");
        return;
        }
      sprintf(buffer,"lbl_%d:",goLabel); Asm(buffer);
      sprintf(buffer,"          ldi   lbl_%d.1", nextLabel); Asm(buffer);
      Asm("            phi  ra");
      sprintf(buffer,"          ldi   lbl_%d.0", nextLabel); Asm(buffer);
      Asm("            plo  ra");
      Asm("            sep  ra");
      sprintf(buffer,"lbl_%d:     ghi  rf", nextLabel++); Asm(buffer);
      Asm("            phi  r3");
      Asm("            glo  rf");
      Asm("            plo  r3");
      Asm("            sep  r3");


      sprintf(buffer,"lbl_%d:",finalLabel); Asm(buffer);
      return;
      }
    showError("Syntax error");
    return;
    }

  /* ********************************* */
  /* ***** Handle computed GO TO ***** */
  /* ********************************* */
  if (*line == '(') {
    line++;
    numLines = 0;
    lines[0] = 0;
    while (*line != ')' && *line != 0) {
      if (*line >= '0' && *line <= '9') {
        lines[numLines] = (lines[numLines] * 10) + (*line - '0');
        line++;
        }
      else if (*line == ',') {
        numLines++;
        lines[numLines] = 0;
        line++;
        }
      }
    numLines++;
    if (*line != ')') {
      showError("Syntax error");
      return;
      }
    line++;
    if (*line != ',') {
      showError("Syntax error");
      return;
      }
    line++;
    pos = 0;
    while ((*line >= 'a' && *line <= 'z') ||
           (*line >= 'A' && *line <= 'Z') ||
           (*line >= '0' && *line <= '9') ||
           *line == '_') token[pos++] = *line++;
    token[pos] = 0;
    if (pos == 0) {
      showError("Syntax error");
      return;
      }
    if (*line != 0) {
      showError("Syntax error");
      return;
      }
    v = findVariable(token, module);
    if (v < 0) {
      showError("Variable not defined");
      return;
      }
    if (variables[v].type == V_REAL) {
      showError("Invalid variable type");
      return;
      }
    if (variables[v].type == V_DEFAULT) {
      if ((variables[v].name[0] >= 'a' && variables[v].name[0] <= 'h') ||
          (variables[v].name[0] >= 'o' && variables[v].name[0] <= 'z') ||
          (variables[v].name[0] >= 'A' && variables[v].name[0] <= 'H') ||
          (variables[v].name[0] >= 'O' && variables[v].name[0] <= 'Z')) {
        showError("Invalid variable type");
        return;
        }
      }
    sprintf(buffer,"          ldi   %s_%s.1                ; Point to variable",
            variables[v].module, variables[v].name);
    Asm(buffer);
    Asm("            phi  rf");
    sprintf(buffer,"          ldi   %s_%s.0",
            variables[v].module, variables[v].name);
    Asm(buffer);
    Asm("            plo  rf");
    finalLabel = nextLabel++;
    if (variables[v].type == V_BYTE || variables[v].type == V_LOGICAL) {
      Asm("            lda  rf");
      }
    else if (variables[v].type == V_SHORT) {
      Asm("            lda  rf");
      sprintf(buffer, "            lbnz lbl_%d",finalLabel); Asm(buffer);
      Asm("            lda  rf");
      }
    else {
      Asm("            lda  rf");
      sprintf(buffer, "            lbnz lbl_%d",finalLabel); Asm(buffer);
      Asm("            lda  rf");
      sprintf(buffer, "            lbnz lbl_%d",finalLabel); Asm(buffer);
      Asm("            lda  rf");
      sprintf(buffer, "            lbnz lbl_%d",finalLabel); Asm(buffer);
      Asm("            lda  rf");
      }
    for (i=0; i<numLines; i++) {
      Asm("            smi  1");
      sprintf(buffer, "            lbz  %s_%d",module, lines[i]); Asm(buffer);
      }
    sprintf(buffer,"lbl_%d:", finalLabel); Asm(buffer);

return;
    }
  showError("Syntax error");
  }

