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

void addLabel(char* label, word value) {
  int i;
  if (passNumber == 2) return;
  for (i=0; i<numLabels; i++)
    if (strcasecmp(label, labels[i]) == 0) {
      printf("<ASM>Duplicate label: %s\n",label);
      exit(1);
      }
  numLabels++;
  if (numLabels == 1) {
    labels = (char**)malloc(sizeof(char*));
    labelValues = (word*)malloc(sizeof(word));
    }
  else {
    labels = (char**)realloc(labels, sizeof(char*) * numLabels);
    labelValues = (word*)realloc(labelValues, sizeof(word) * numLabels);
    }
  labels[numLabels-1] = (char*)malloc(strlen(label) + 1);
  strcpy(labels[numLabels-1], label);
  labelValues[numLabels-1] = value;
  }

word getLabel(char* label) {
  int i;
  if (passNumber == 1) return 0;
  for (i=0; i<numLabels; i++)
    if (strcasecmp(label, labels[i]) == 0) {
      return labelValues[i];
      }
  printf("<ASM>Label not found: %s\n",label);
  exit(1);
  }

void addDefine(char* define, int value, int redefine) {
  int i;
  if (passNumber == 2) return;
  for (i=0; i<numDefines; i++)
    if (strcasecmp(define, defines[i]) == 0) {
      if (redefine) {
        defineValues[i] = value;
        return;
        }
      printf("<ASM>Duplicate define: %s\n",define);
      exit(1);
      }
  numDefines++;
  if (numDefines == 1) {
    defines = (char**)malloc(sizeof(char*));
    defineValues = (int*)malloc(sizeof(int));
    }
  else {
    defines = (char**)realloc(defines, sizeof(char*) * numDefines);
    defineValues = (int*)realloc(defineValues, sizeof(int) * numDefines);
    }
  defines[numDefines-1] = (char*)malloc(strlen(define) + 1);
  strcpy(defines[numDefines-1], define);
  defineValues[numDefines-1] = value;
  }

int getDefine(char* define) {
  int i;
  for (i=0; i<numDefines; i++)
    if (strcasecmp(define, defines[i]) == 0) {
      return defineValues[i];
      }
  return 0;
  }

void addExtrn(char* e) {
  int i;
  char name[256];
  strcpy(name, module);
  strcat(name,"|");
  strcat(name,e);
  for (i=0; i<numExtrns; i++)
    if (strcasecmp(extrns[i], name) == 0) return;
  numExtrns++;
  if (numExtrns == 1)
    extrns = (char**)malloc(sizeof(char*));
  else
    extrns = (char**)realloc(extrns, sizeof(char*) * numExtrns);
  extrns[numExtrns-1] = (char*)malloc(strlen(name) + 1);
  strcpy(extrns[numExtrns-1], name);
  }

void Asm(char* line) {
  if (passNumber == 2) {
    if (showAsm) printf("%s\n",line);
    write(asmFile, line, strlen(line));
    write(asmFile, lineEnding, strlen(lineEnding));
    }
  }

