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

int prepass(char* filename) {
  FILE *source;
  source = fopen(filename,"r");
  if (source == NULL) {
    printf("Could not open %s\n",filename);
    exit(1);
    }
  useStep = 0;
  useData = 0;
  useStep32 = 0;
  useTrig = 0;
  useStrings = 0;

  lblStart = 0;

  if (use32Bits) {
    addDefine("use32bits",1,1);
    }

  while (fgets(currentLine, 1023, source) != NULL) {
    while (strlen(currentLine) > 0 && currentLine[strlen(currentLine)-1] <= 32) currentLine[strlen(currentLine)-1] = 0;
    if (currentLine[0] == '.') {
      if (strcasecmp(currentLine,".fp") == 0) { useFp = -1; use32Bits = -1; }
      if (strcasecmp(currentLine,".32") == 0) { use32Bits = -1; }
      if (strcasecmp(currentLine,".elfos") == 0) { useElfos = -1; programStart = 0x2000; }
      if (strcasecmp(currentLine,".noelfos") == 0) { useElfos = 0; }
      if (strcasecmp(currentLine,".list") == 0) { showList = -1; }
      if (strcasecmp(currentLine,".nolist") == 0) { showList = 0; }
      if (strcasecmp(currentLine,".1805") == 0) { use1805 = -1; }
      if (strcasecmp(currentLine,".vars") == 0) { showVariables = -1; }
      if (strcasecmp(currentLine,".novars") == 0) { showVariables = 0; }
      if (strcasecmp(currentLine,".symbols") == 0) { showSymbols = -1; }
      if (strcasecmp(currentLine,".nosymbols") == 0) { showSymbols = 0; }
      if (strcasecmp(currentLine,".runtime") == 0) { showRuntime = -1; }
      if (strcasecmp(currentLine,".noruntime") == 0) { showRuntime = 0; }
      if (strcasecmp(currentLine,".options") == 0) { showOptions = -1; }
      if (strncasecmp(currentLine,".ram=",5) == 0) { processRAM(currentLine+5); }
      if (strncasecmp(currentLine,".rom=",5) == 0) { processROM(currentLine+5); }
      if (strncasecmp(currentLine,".vars=",6) == 0) { variableStart=getHex(currentLine+6); }
      if (strncasecmp(currentLine,".stack=",7) == 0) { stack=getHex(currentLine+7); }
      if (strncasecmp(currentLine,".estack=",8) == 0) { estack=getHex(currentLine+8); }
      if (strncasecmp(currentLine,".iosize=",8) == 0) { iBufferSize=getHex(currentLine+8); }
      if (strncasecmp(currentLine,".exit=",6) == 0) { exitAddress=getHex(currentLine+6); }
      if (strncasecmp(currentLine,".start=",7) == 0) { programStart=getHex(currentLine+7); }
      if (strcasecmp(currentLine,".stg") == 0) {
        useStg = -1;
        exitAddress = 0x8003;
        ramEnd = 0x7eff;
        }
      if (strcasecmp(currentLine,".term=bios") == 0) {
        useSelfTerm = 0;
        lblF_inmsg = 0xff66;
        lblF_type = 0xff03;
        lblF_read = 0xff06;
        lblF_input = 0xff0f;
        lblF_msg = 0xff09;
        lblF_setbd = 0xff2d;
        }
      if (strcasecmp(currentLine,".term=elfos") == 0) {
        useSelfTerm = 0;
        lblF_inmsg = 0x034b;
        lblF_type = 0x0330;
        lblF_read = 0x0309;
        lblF_input = 0x0339;
        lblF_msg = 0x0333;
        lblF_setbd = 0x0360;
        }
      if (strcasecmp(currentLine,".term=self") == 0) {
        useSelfTerm = 0xff;
        lblF_inmsg = 0x0000;
        lblF_type = 0x0000;
        lblF_read = 0x0000;
        lblF_input = 0x0000;
        lblF_msg = 0x0000;
        lblF_setbd = 0x0000;
        }
      if (strcasecmp(currentLine,".term=none") == 0) {
        useSelfTerm = 0;
        lblF_inmsg = 0xffff;
        lblF_type = 0xffff;
        lblF_read = 0xffff;
        lblF_input = 0xffff;
        lblF_msg = 0xffff;
        lblF_setbd = 0xffff;
        }
      if (strcasecmp(currentLine,".arch=pev") == 0) {
        ramStart = 0x0000;
        ramEnd = 0x7fff;
        romStart = 0x8000;
        romEnd = 0xffff;
        SERN = BN2;
        SERP = B2;
        SERSEQ = SEQ;
        SERREQ = REQ;
        }
      if (strcasecmp(currentLine,".arch=pev2") == 0) {
        ramStart = 0x0000;
        ramEnd = 0x7fff;
        romStart = 0x8000;
        romEnd = 0xffff;
        SERN = B2;
        SERP = BN2;
        SERSEQ = REQ;
        SERREQ = SEQ;
        }
      if (strcasecmp(currentLine,".arch=micro") == 0) {
        ramStart = 0x0000;
        ramEnd = 0x7fff;
        romStart = 0x8000;
        romEnd = 0xffff;
        SERN = BN2;
        SERP = B2;
        SERSEQ = SEQ;
        SERREQ = REQ;
        }
      if (strcasecmp(currentLine,".arch=elf2k") == 0) {
        ramStart = 0x0000;
        ramEnd = 0x7fff;
        romStart = 0x8000;
        romEnd = 0xffff;
        SERN = BN3;
        SERP = B3;
        SERSEQ = SEQ;
        SERREQ = REQ;
        }
      if (strcasecmp(currentLine,".arch=mchip") == 0) {
        ramStart = 0x8000;
        ramEnd = 0xffff;
        romStart = 0x0000;
        romEnd = 0x7fff;
        SERN = B3;
        SERP = BN3;
        SERSEQ = REQ;
        SERREQ = SEQ;
        lblF_inmsg = 0x0f66;
        lblF_type = 0x0f03;
        lblF_read = 0x0f06;
        lblF_input = 0x0f0f;
        lblF_msg = 0x0f09;
        lblF_setbd = 0x0f2d;
        }
      strcpy(currentLine,"");
      }




     }
   fclose(source);

  if (SERSEQ == REQ) addDefine("INVQ",1,1);
  if (SERP == B1) addDefine("SEREF1",1,1);
  if (SERP == B2) addDefine("SEREF2",1,1);
  if (SERP == B3) addDefine("SEREF3",1,1);
  if (SERP == B4) addDefine("SEREF4",1,1);
  if (SERP == BN1) addDefine("SEREF1I",1,1);
  if (SERP == BN2) addDefine("SEREF2I",1,1);
  if (SERP == BN3) addDefine("SEREF3I",1,1);
  if (SERP == BN4) addDefine("SEREF4I",1,1);

  return(0);
  }

