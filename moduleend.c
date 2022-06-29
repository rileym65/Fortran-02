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

void moduleEnd() {
  int i;
  int j;
  int count;
  int size;
  char vt[3];
  for (i=0; i<numVariables; i++) {
    if (strcasecmp(module, variables[i].module) == 0) {
      size = 1;
      switch (varType(i)) {
        case 'I': size *= 4; strcpy(vt, "dw"); break;
        case 'R': size *= 4; strcpy(vt, "dw"); break;
        case 'S': size *= 2; strcpy(vt, "dw"); break;
        case 'L': size *= 1; strcpy(vt, "db"); break;
        case 'B': size *= 1; strcpy(vt, "db"); break;
        default : size *= 4; strcpy(vt, "dw"); break;
        }
      if (strlen(variables[i].common) == 0) {
        if (variables[i].dimensions > 0) {
          size *= variables[i].sizes[0];
          count = variables[i].sizes[0];
          if (variables[i].dimensions > 1) {
            size *= variables[i].sizes[1];
            count *= variables[i].sizes[1];
            }
          if (variables[i].dimensions > 2) {
            size *= variables[i].sizes[2];
            count *= variables[i].sizes[2];
            }
          if (variables[i].values == NULL) {
            sprintf(buffer, "v_%s:    ds    %d", 
                    variables[i].name,
                    size);
            Asm(buffer);
           }
          else {
            for (j=0; j<count; j++) {
              if (j == 0) {
                sprintf(buffer, "v_%s:    dw    %d,%d", 
                        variables[i].name,
                        variables[i].values[j] / 256,
                        variables[i].values[j] % 256);
                }
              else {
                sprintf(buffer, "          dw    %d,%d", 
                        variables[i].values[j] / 256,
                        variables[i].values[j] % 256);
                }
              Asm(buffer);
              }
            }
          }
        else if (variables[i].isArg) {
          sprintf(buffer, "v_%s:    dw    0", 
                         variables[i].name);
          Asm(buffer);
          }
        else {
          switch (variables[i].type) {
            case V_BYTE:
                 sprintf(buffer, "v_%s:    db    %d", 
                         variables[i].name,
                         variables[i].value);
                 Asm(buffer);
                 break;
            case V_LOGICAL:
                 sprintf(buffer, "v_%s:    db    %d", 
                         variables[i].name,
                         variables[i].value);
                 Asm(buffer);
                 break;
            case V_SHORT:
                 sprintf(buffer, "v_%s:    dw    %d", 
                         variables[i].name,
                         variables[i].value);
                 Asm(buffer);
                 break;
            case V_INTEGER:
                 sprintf(buffer, "v_%s:    dw    %d,%d", 
                         variables[i].name,
                         variables[i].value / 65536,
                         variables[i].value % 65536);
                 Asm(buffer);
                 break;
            case V_REAL:
                 sprintf(buffer, "v_%s:    dw    %d,%d", 
                         variables[i].name,
                         variables[i].value / 65536,
                         variables[i].value % 65536);
                 Asm(buffer);
                 break;
            case V_DEFAULT:
                 if ((variables[i].name[0] >= 'i' && variables[i].name[0] <= 'n') ||
                     (variables[i].name[0] >= 'I' && variables[i].name[0] <= 'N')) {
                   sprintf(buffer, "v_%s:    dw    %d,%d", 
                           variables[i].name,
                           variables[i].value / 65536,
                           variables[i].value % 65536);
                   Asm(buffer);
                   }
                 else {
                   sprintf(buffer, "v_%s:    dw    %d,%d", 
                           variables[i].name,
                           variables[i].value / 65536,
                           variables[i].value % 65536);
                   Asm(buffer);
                   }
            }
  
          }
        }
      }
    }

  if (inSub == 0) {
    if (useElfos) {
      Asm("file1_:    db    0,0");
      Asm("           db    0,0");
      Asm("           dw    0");
      Asm("fildes1_:  db    0,0,0,0");
      Asm("           dw    dta1_");
      Asm("           db    0,0,0,0,0,0,0,0,0,0,0");
      Asm("file2_:    db    0,0");
      Asm("           db    0,0");
      Asm("           dw    0");
      Asm("fildes2_:  db    0,0,0,0");
      Asm("           dw    dta2_");
      Asm("           db    0,0,0,0,0,0,0,0,0,0,0");
      Asm("file3_:    db    0,0");
      Asm("           db    0,0");
      Asm("           dw    0");
      Asm("fildes3_:  db    0,0,0,0");
      Asm("           dw    dta3_");
      Asm("           db    0,0,0,0,0,0,0,0,0,0,0");
      Asm("file4_:    db    0,0");
      Asm("           db    0,0");
      Asm("           dw    0");
      Asm("fildes4_:  db    0,0,0,0");
      Asm("           dw    dta4_");
      Asm("           db    0,0,0,0,0,0,0,0,0,0,0");
      Asm("dta1_:     dw    512");
      Asm("dta2_:     dw    512");
      Asm("dta3_:     dw    512");
      Asm("dta4_:     dw    512");
      }
    else {
      Asm("file1_:    equ   $");
      Asm("file2_:    equ   $");
      Asm("file3_:    equ   $");
      Asm("file4_:    equ   $");
      }
    sprintf(buffer,"iobuffer:  ds    %d",iBufferSize); Asm(buffer);
    Asm("            public file1_");
    Asm("            public file2_");
    Asm("            public file3_");
    Asm("            public file4_");
    Asm("            public iobuffer");
    Asm("            end    init");
    }

  }

