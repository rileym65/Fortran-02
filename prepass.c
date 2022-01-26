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
      if (strcasecmp(currentLine,".binary") == 0) {
        outMode = 'B';
        strcpy(outName,baseName);
        strcat(outName, ".bin");
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




//     if (strlen(currentLine) > 0) {
//       qt = 0;
//       for (i=0; i<strlen(currentLine); i++) {
//         if (currentLine[i] == '"') qt = 1-qt;
//         if (qt == 0) {
//           if (useFp) {
//             if (strncasecmp(currentLine+i,"cos(",4) == 0) { addDefine("COSFP",1,1); addDefine("SINFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"exp(",4) == 0) { addDefine("EXPFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"ln(",3) == 0) { addDefine("LNFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"sin(",4) == 0) { addDefine("SINFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"sqrt(",5) == 0) { addDefine("SQRTFP",1,1); addDefine("POWFP",1,1); addDefine("LNFP",1,1); addDefine("EXPFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"tan(",4) == 0) { addDefine("TANFP",1,1); addDefine("COSFP",1,1); addDefine("SINFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"atan(",5) == 0) { addDefine("ATANFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"asin(",5) == 0) { addDefine("ASINFP",1,1); useTrig = -1; }
//             if (strncasecmp(currentLine+i,"acos(",5) == 0) { addDefine("ACOSFP",1,1); useTrig = -1; }
//             }
//           if (use32Bits) {
//             if (currentLine[i] == '+') addDefine("ADD32",1,1);
//             if (currentLine[i] == '-') addDefine("SUB32",1,1);
//             if (currentLine[i] == '*') addDefine("MUL32",1,1);
//             if (currentLine[i] == '/') addDefine("DIV32",1,1);
//             if (currentLine[i] == '%') addDefine("MOD32",1,1);
//             if (currentLine[i] == '&') addDefine("AND32",1,1);
//             if (currentLine[i] == '|') addDefine("OR32",1,1);
//             if (currentLine[i] == '^') addDefine("XOR32",1,1);
//             if (currentLine[i] == '=') addDefine("EQ32",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] == '>') addDefine("NE32",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] == '=') addDefine("LTE32",1,1);
//             if (currentLine[i] == '>' && currentLine[i+1] == '=') addDefine("GTE32",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] != '>' && currentLine[i+1] != '=') addDefine("LT32",1,1);
//             if (currentLine[i] == '>' && currentLine[i+1] != '=') addDefine("GT32",1,1);
//             if (strncasecmp(currentLine+i,".and.",5) == 0) addDefine("AND32",1,1);
//             if (strncasecmp(currentLine+i,".or.",4) == 0) addDefine("OR32",1,1);
//             if (strncasecmp(currentLine+i,".xor.",5) == 0) addDefine("XOR32",1,1);
//             if (strncasecmp(currentLine+i,".eq.",4) == 0) addDefine("EQ32",1,1);
//             if (strncasecmp(currentLine+i,".ne.",4) == 0) addDefine("NE32",1,1);
//             if (strncasecmp(currentLine+i,".gt.",4) == 0) addDefine("GT32",1,1);
//             if (strncasecmp(currentLine+i,".lt.",4) == 0) addDefine("LT32",1,1);
//             if (strncasecmp(currentLine+i,".gte.",4) == 0) addDefine("GTE32",1,1);
//             if (strncasecmp(currentLine+i,".lte.",4) == 0) addDefine("LTE32",1,1);
//             if (strncasecmp(currentLine+i,"print",5) == 0) addDefine("ITOA32",1,1);
//             if (strncasecmp(currentLine+i,"input",5) == 0) addDefine("ATOI32",1,1);
//             if (strncasecmp(currentLine+i,"next",4) == 0) addDefine("NEXT32",1,1);
//             if (strncasecmp(currentLine+i,"step",4) == 0) useStep32 = -1;
//             if (strncasecmp(currentLine+i,"fre(",4) == 0) addDefine("SUB32",1,1);
//             if (strncasecmp(currentLine+i,"sgn(",4) == 0) addDefine("SGN32",1,1);
//             if (strncasecmp(currentLine+i,"abs(",4) == 0) addDefine("ABS32",1,1);
//             if (strncasecmp(currentLine+i,"rnd(",4) == 0) addDefine("RND32",1,1);
//             }
//           else if (useFp) {
//             if (strncasecmp(currentLine+i,"print",5) == 0) { addDefine("ITOA32",1,1); addDefine("FTOA",1,1); }
//             }
//           else {
//             if (currentLine[i] == '+') addDefine("ADD16",1,1);
//             if (currentLine[i] == '-') addDefine("SUB16",1,1);
//             if (currentLine[i] == '*') addDefine("MUL16",1,1);
//             if (currentLine[i] == '/') addDefine("DIV16",1,1);
//             if (currentLine[i] == '%') addDefine("MOD16",1,1);
//             if (currentLine[i] == '&') addDefine("AND16",1,1);
//             if (currentLine[i] == '|') addDefine("OR16",1,1);
//             if (currentLine[i] == '^') addDefine("XOR16",1,1);
//             if (currentLine[i] == '=') addDefine("EQ16",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] == '>') addDefine("NE16",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] == '=') addDefine("LTE16",1,1);
//             if (currentLine[i] == '>' && currentLine[i+1] == '=') addDefine("GTE16",1,1);
//             if (currentLine[i] == '<' && currentLine[i+1] != '>' && currentLine[i+1] != '=') addDefine("LT16",1,1);
//             if (currentLine[i] == '>' && currentLine[i+1] != '=') addDefine("GT16",1,1);
//             if (strncasecmp(currentLine+i,"print",5) == 0) addDefine("ITOA16",1,1);
//             if (strncasecmp(currentLine+i,"input",5) == 0) addDefine("ATOI16",1,1);
//             if (strncasecmp(currentLine+i,"next",4) == 0) addDefine("NEXT16",1,1);
//             if (strncasecmp(currentLine+i,"step",4) == 0) useStep = -1;
//             if (strncasecmp(currentLine+i,"fre(",4) == 0) addDefine("SUB16",1,1);
//             if (strncasecmp(currentLine+i,"sgn(",4) == 0) addDefine("SGN16",1,1);
//             if (strncasecmp(currentLine+i,"abs(",4) == 0) addDefine("ABS16",1,1);
//             if (strncasecmp(currentLine+i,"rnd(",4) == 0) addDefine("RND16",1,1);
//             }
//           if (strncasecmp(currentLine+i,"dealloc",7) == 0) addDefine("HEAP",1,1);
//           if (strncasecmp(currentLine+i,"locate",6) == 0) addDefine("ITOA16",1,1);
//           if (strncasecmp(currentLine+i,"read",4) == 0 &&
//               currentLine[i-1] != 'f') useData = -1;
//           if (strncasecmp(currentLine+i,"flg(",4) == 0) addDefine("USEEF",1,1);
//           if (strncasecmp(currentLine+i,"alloc(",6) == 0) addDefine("HEAP",1,1);
// //          if (strncasecmp(currentLine+i,"$",1) == 0) addDefine("STRINGS",1,1);
//           if (strncasecmp(currentLine+i,"open",4) == 0) addDefine("FILES",1,1);
//           if (strncasecmp(currentLine+i,"close",5) == 0) addDefine("FILES",1,1);
//           }
//         }
//       }
     }
   fclose(source);
// 
//   if (useFp) addDefine("USEFP",1,1);
//   if (useTrig) addDefine("USETRIG",1,1);
//   if (useSelfTerm) addDefine("SELFTERM",1,1);
// 
//   if (getDefine("FILES")) {
//     addDefine("HEAP",1,1);
//     }
// 
//   if (getDefine("STRINGS")) {
//     addDefine("HEAP",1,1);
//     }
// 
//   if (getDefine("ASINFP")) addDefine("ATANFP",1,1);
//   if (getDefine("ACOSFP")) addDefine("ATANFP",1,1);
//   if (getDefine("ATANFP")) {
//     addDefine("ADDFP",1,1);
//     addDefine("SUBFP",1,1);
//     addDefine("MULFP",1,1);
//     addDefine("DIVFP",1,1);
//     addDefine("SQRTFP",1,1);
//     }
//     
//   if (getDefine("EQ16") || getDefine("NE16") || getDefine("GT16") ||
//       getDefine("LT16") || getDefine("GTE16") || getDefine("LTE16")) addDefine("CMP16",1,1);
//   if (getDefine("CMP16")) addDefine("SUB16",1,1);
//   if (getDefine("RND16")) {
//     addDefine("MOD16",1,1);
//     addDefine("LFSR",1,1);
//     }
//   if (getDefine("RND32")) {
//     addDefine("MOD32",1,1);
//     addDefine("LFSR",1,1);
//     }
//   if (getDefine("MOD16")) addDefine("DIV16",1,1);
//   if (getDefine("NEXT16")) {
//     addDefine("SUB16",1,1);
//     addDefine("ABS16",1,1);
//     }
//   if (useStep) {
//     addDefine("DIV16",1,1);
//     }
//   if (getDefine("MUL16")) addDefine("MULDIV16",1,1);
//   if (getDefine("DIV16")) addDefine("MULDIV16",1,1);
//   if (useStep32) {
//     addDefine("DIV32",1,1);
//     }
//   if (getDefine("MUL32")) {
//     addDefine("ADD32",1,1);
//     addDefine("ZERO32",1,1);
//     addDefine("SHL32",1,1);
//     addDefine("SHR32",1,1);
//     }
//   if (getDefine("MOD32")) addDefine("DIV32",1,1);
//   if (getDefine("DIV32")) {
//     addDefine("SUB32",1,1);
//     addDefine("NEG32",1,1);
//     addDefine("COMP32",1,1);
//     addDefine("ICOMP32",1,1);
//     addDefine("ADD32",1,1);
//     addDefine("ZERO32",1,1);
//     addDefine("SHL32",1,1);
//     addDefine("SHR32",1,1);
//     }
//   if (getDefine("NEXT32")) {
//     addDefine("SUB32",1,1);
//     addDefine("ABS32",1,1);
//     }
//   if (getDefine("EQ32") || getDefine("NE32") || getDefine("GT32") ||
//       getDefine("LT32") || getDefine("GTE32") || getDefine("LTE32")) {
//     addDefine("CMP32",1,1);
//     addDefine("COMP32",1,1);
//     }
//   if (useFp) {
//     if (useTrig) {
//       addDefine("MULFP",1,1);
//       addDefine("DIVFP",1,1);
//       addDefine("ADDFP",1,1);
//       addDefine("SUBFP",1,1);
//       }
//     addDefine("ABSFP", getDefine("ABS32"), 1);
//     addDefine("ADDFP", getDefine("ADD32"), 1);
//     addDefine("EQFP", getDefine("EQ32"), 1);
//     addDefine("GTFP", getDefine("GT32"), 1);
//     addDefine("GTEFP", getDefine("GTE32"), 1);
//     addDefine("LTFP", getDefine("LT32"), 1);
//     addDefine("LTEFP", getDefine("LTE32"), 1);
//     addDefine("NEFP", getDefine("NE32"), 1);
//     addDefine("SUBFP", getDefine("SUB32"), 1);
//     addDefine("MULFP", getDefine("MUL32"), 1);
//     addDefine("DIVFP", getDefine("DIV32"), 1);
//     addDefine("FTOA", getDefine("ITOA32"), 1);
//     addDefine("ATOF", getDefine("ATOI32"), 1);
//     addDefine("SGNFP", getDefine("SGN32"), 1);
//     if (getDefine("SUBFP")) addDefine("ADDFP",1,1);
//     if (getDefine("FTOA")) {
//       addDefine("DIVFP",1,1);
//       }
//     if (getDefine("EQFP") || getDefine("NEFP") || getDefine("GTFP") ||
//         getDefine("LTFP") || getDefine("GTEFP") || getDefine("LTEFP")) {
//       addDefine("SUBFP",1,1);
//       addDefine("ADDFP",1,1);
//       }
//     if (getDefine("FTOA")) addDefine("MULFP",1,1);
//     }
//   if (lblF_inmsg == 0xffff) {
//     addDefine("ATOI16",0,1);
//     addDefine("ITOA16",0,1);
//     }

  if (SERSEQ == REQ) addDefine("INVQ",1,1);
  if (SERP == B1) addDefine("SEREF1",1,1);
  if (SERP == B2) addDefine("SEREF2",1,1);
  if (SERP == B3) addDefine("SEREF3",1,1);
  if (SERP == B4) addDefine("SEREF4",1,1);
  if (SERP == BN1) addDefine("SEREF1I",1,1);
  if (SERP == BN2) addDefine("SEREF2I",1,1);
  if (SERP == BN3) addDefine("SEREF3I",1,1);
  if (SERP == BN4) addDefine("SEREF4I",1,1);

  printf("\n");
  return(0);
  }

