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

int pass(char* filename) {
  int   i;
  char  tmp[16];
  source = fopen(filename,"r");
  if (source == NULL) {
    printf("Could not open %s\n",filename);
    exit(1);
    }
  address = programStart;
  if (useElfos) address -= 6;
  outAddress = address;
  lastLineNumber = 0;
  lineCount = -1;
  nextLabel = 1;
  numDoLoops = 0;
  Asm("BEGIN__:  equ  $");
  startup();
  Asm("PROGRAM__:  equ  $");
  inUnit = 0;
  strcpy(module,"main");
  while (nextStatement() != 0) {
    if (currentLine[0] == '.') strcpy(currentLine,"");
    if (strlen(statement) > 0) {
      if (passNumber == 2 && showList) printf("%s\n",initialLine);
      compileLine(statement);
      }
    }
  fclose(source);
  Asm("RUNTIME__:  equ  $");
  if (showCompiler && passNumber == 2) printf("%04x:",address);
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
  if (passNumber == 2 && showCompiler) printf("\n");
  library();
  Asm("ENDRUNTIME__:  equ  $");
  if (useData) {
    if (passNumber == 1) { 
      dataAddress = address;
      }
    if (showCompiler && passNumber == 2) printf("%04x:",address);
    strcpy(buffer, "data:     dw    ");
    for (i=0; i<numData; i++) {
      if (use32Bits) {
        sprintf(tmp,"%d,%d",(data[i] & 0xffff0000) >> 16, data[i] & 0xffff);
        }
      else {
        sprintf(tmp,"%d",data[i]);
        }
      if (strlen(buffer) > 16) strcat(buffer,",");
      strcat(buffer,tmp);
      if (strlen(buffer) > 60) {
        Asm(buffer);
        strcpy(buffer, "          dw    ");
        }
      }
    if (strlen(buffer) > 16) Asm(buffer);
    if (passNumber == 2 && showCompiler) printf("\n");
    }

  Asm("DATA__:  equ  $");
  for (i=0; i<numVariables; i++) {
    if (strlen(variables[i].common) == 0) {
      switch (variables[i].type) {
        case V_BYTE:
             sprintf(buffer, "%s_%s:    db    %d", 
                     variables[i].module,
                     variables[i].name,
                     variables[i].value);
             Asm(buffer);
             break;
        case V_LOGICAL:
             sprintf(buffer, "%s_%s:    db    %d", 
                     variables[i].module,
                     variables[i].name,
                     variables[i].value);
             Asm(buffer);
             break;
        case V_SHORT:
             sprintf(buffer, "%s_%s:    dw    %d", 
                     variables[i].module,
                     variables[i].name,
                     variables[i].value);
             Asm(buffer);
             break;
        case V_INTEGER:
             sprintf(buffer, "%s_%s:    dw    %d,%d", 
                     variables[i].module,
                     variables[i].name,
                     variables[i].value / 65536,
                     variables[i].value % 65536);
             Asm(buffer);
             break;
        case V_REAL:
             sprintf(buffer, "%s_%s:    dw    %d,%d", 
                     variables[i].module,
                     variables[i].name,
                     variables[i].value / 65536,
                     variables[i].value % 65536);
             Asm(buffer);
             break;
        case V_DEFAULT:
             if ((variables[i].name[0] >= 'i' && variables[i].name[0] <= 'n') ||
                 (variables[i].name[0] >= 'I' && variables[i].name[0] <= 'N')) {
               sprintf(buffer, "%s_%s:    dw    %d,%d", 
                       variables[i].module,
                       variables[i].name,
                       variables[i].value / 65536,
                       variables[i].value % 65536);
               Asm(buffer);
               }
             else {
               sprintf(buffer, "%s_%s:    dw    %d,%d", 
                       variables[i].module,
                       variables[i].name,
                       variables[i].value / 65536,
                       variables[i].value % 65536);
               Asm(buffer);
               }

        }
      }
    }

  sprintf(buffer,"iobuffer:  ds    %d",iBufferSize); Asm(buffer);
  if (getDefine("FMTWRITE")) {
    Asm("SCRATCH1_: ds    32");
    Asm("SCRATCH2_: ds    32");
    }
  if (getDefine("LFSR")) {
    Asm("LFSR_:     dw    0");
    Asm("LFSR__:    dw    0");
    }
  if (useData) {
    Asm("DATA_:     dw    0");
    }
  if (getDefine("HEAP")) {
    Asm("HEAP_:     dw    0");
    }
  if (useStg) {
    Asm("STG_:      dw    0");
    }
  if (getDefine ("FILES")) {
    Asm("file1_:    dw    0");
    Asm("file2_:    dw    0");
    Asm("file3_:    dw    0");
    Asm("file4_:    dw    0");
    Asm("file5_:    dw    0");
    Asm("file6_:    dw    0");
    Asm("file7_:    dw    0");
    Asm("file8_:    dw    0");
    }
  Asm("FREE_:    dw    0");
  Asm("end__:    equ   $");
  return(0);
  }

