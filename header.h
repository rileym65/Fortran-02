#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define localtime_r(X,Y) (localtime_s(Y,X))
#else
#include <unistd.h>
#include <sys/time.h>
#define O_BINARY 0
#endif


#ifdef MAIN
#define LINK
#else
#define LINK extern
#endif

#define VERSION  "0.1.0"

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int   dword;

#define V_DEFAULT 0
#define V_INTEGER 1
#define V_REAL    2
#define V_LOGICAL 3
#define V_BYTE    4
#define V_SHORT   5

typedef struct {
  char  name[32];
  char  module[32];
  byte  commonVar;
  char  common[32];
  word  offset;
  byte  type;
  byte  dimensions;
  word  sizes[3];
  dword value;
  byte  isArg;
  } VARREC;

typedef struct {
  char   name[32];
  int    numVariables;
  int    size;
  int    maxSize;
  byte  *data;
  } COMMON;

typedef struct {
  char   name[32];
  int    size;
  word   offset;
  } COMMONBLOCK;

typedef union {
  int integer;
  float real;
  } INTREAL;

typedef struct {
  int line;
  int loopStart;
  int variable;
  char varStep;
  dword step;
  dword loops;
  } DOREC;

#define IDL      0x00
#define LDN      0x00
#define INC      0x10
#define DEC      0x20
#define BR       0x30
#define BQ       0x31
#define BZ       0x32
#define BDF      0x33
#define B1       0x34
#define B2       0x35
#define B3       0x36
#define B4       0x37
#define NBR      0x38
#define BNQ      0x39
#define BNZ      0x3a
#define BNF      0x3b
#define BN1      0x3c
#define BN2      0x3d
#define BN3      0x3e
#define BN4      0x3f
#define LDA      0x40
#define STR      0x50
#define IRX      0x60
#define OUT      0x61
#define INP      0x69
#define RET      0x70
#define DIS      0x71
#define LDXA     0x72
#define STXD     0x73
#define ADC      0x74
#define SDB      0x75
#define SHRC     0x76
#define SMB      0x77
#define SAV      0x78
#define MARK     0x79
#define REQ      0x7a
#define SEQ      0x7b
#define ADCI     0x7c
#define SDBI     0x7d
#define SHLC     0x7e
#define SMBI     0x7f
#define GLO      0x80
#define GHI      0x90
#define PLO      0xa0
#define PHI      0xb0
#define LBR      0xc0
#define LBQ      0xc1
#define LBZ      0xc2
#define LBDF     0xc3
#define NOP      0xc4
#define LSNQ     0xc5
#define LSNZ     0xc6
#define LSNF     0xc7
#define NLBR     0xc8
#define LSKP     0xc8
#define LBNQ     0xc9
#define LBNZ     0xca
#define LBNF     0xcb
#define LSIE     0xcc
#define LSQ      0xcd
#define LSZ      0xce
#define LSDF     0xcf
#define SEP      0xd0
#define SEX      0xe0
#define LDX      0xf0
#define OR       0xf1
#define AND      0xf2
#define XOR      0xf3
#define ADD      0xf4
#define SD       0xf5
#define SHR      0xf6
#define SM       0xf7
#define LDI      0xf8
#define ORI      0xf9
#define ANI      0xfa
#define XRI      0xfb
#define ADI      0xfc
#define SDI      0xfd
#define SHL      0xfe
#define SMI      0xff
#define R0       0x00
#define R1       0x01
#define R2       0x02
#define R3       0x03
#define R4       0x04
#define R5       0x05
#define R6       0x06
#define R7       0x07
#define R8       0x08
#define R9       0x09
#define RA       0x0a
#define RB       0x0b
#define RC       0x0c
#define RD       0x0d
#define RE       0x0e
#define RF       0x0f

#define OP_NUMFP  0x01
#define OP_NUM    0x00

LINK DOREC   doLoops[32];
LINK int     numDoLoops;
LINK int     inUnit;
LINK int     inSub;
LINK int     inBlockData;
LINK char    module[32];
LINK int     nextLabel;
LINK char    nextLine[1024];
LINK int     numVariables;
LINK int     passNumber;
LINK FILE   *source;
LINK char    sourceFile[1024];
LINK char    statement[4096];
LINK int     statementLabel;
LINK VARREC *variables;
LINK COMMON *common;
LINK int     numCommon;
LINK COMMONBLOCK *commonBlocks;
LINK int          numCommonBlocks;
LINK char       **externals;
LINK int          numExternals;



LINK char   createLst;
LINK word   address;
LINK int    asmFile;
LINK char   asmName[1024];
LINK char   baseName[1024];
LINK char   buffer[2048];
LINK int    callArgs[100];
LINK char   callArgTypes[100];
LINK int    numCallArgs;
LINK char   compMode;
LINK word   codeGenerated;
LINK dword  *data;
LINK word   dataAddress;
LINK word  *dataLines;
LINK word  *dataPositions;
LINK int    errorCount;
LINK int    exprErrors;
LINK word   numData;
LINK word   numDataLines;
LINK word   iBufferSize;
LINK word   stack;
LINK word   estack;
LINK word   heap;
LINK word   exitAddress;
LINK word   highest;
LINK word   lastLineNumber;
LINK word   lblF_delay;
LINK word   lblF_inmsg;
LINK word   lblF_msg;
LINK word   lblF_setbd;
LINK word   lblF_type;
LINK word   lblF_read;
LINK word   lblF_input;
LINK word   lblStart;
LINK char   currentLine[2048];
LINK word   keyBuffer;
LINK char   initialLine[256];
LINK int    lineCount;
LINK char   lineEnding[3];
LINK word   lineNumber;
LINK word   lineNumbers[16384];
LINK word   lineAddresses[16384];
LINK word   listCount;
LINK char   lstName[1024];
LINK char   matches[10][64];
LINK int    matchCount;
LINK word   numberOfLines;
LINK word   numberOfVariables;
LINK word   outAddress;
LINK byte   outBuffer[16];
LINK byte   outCount;
LINK int    outFile;
LINK char   outMode;
LINK char   outName[1024];
LINK word   programStart;
LINK word   ramEnd;
LINK word   ramStart;
LINK word   romEnd;
LINK word   romStart;
LINK word   runtime;
LINK byte   SERSEQ;
LINK byte   SERREQ;
LINK byte   SERP;
LINK byte   SERN;
LINK char   showAsm;
LINK char   showCompiler;
LINK char   showList;
LINK char   showOptions;
LINK char   showRuntime;
LINK char   showSymbols;
LINK char   showVariables;
LINK int    tokens[64];
LINK int    numTokens;
LINK char   use1805;
LINK byte   useData;
LINK byte   useSelfTerm;
LINK byte   useStep;
LINK byte   useStg;
LINK byte   useTrig;
LINK byte   useElfos;
LINK byte   useAsm;
LINK byte   use32Bits;
LINK byte   useFp;
LINK byte   useStep32;
LINK byte   useStrings;
LINK byte   debug;
LINK byte   image[65536];
LINK int    highestAddress;
LINK int    lowestAddress;

LINK char **variableNames;
LINK word  *variableAddresses;
LINK char  *variableTypes;
LINK word   variableNextAddress;
LINK word   variableRAM;
LINK word   variableStart;
LINK FILE  *lstFile;

LINK char** defines;
LINK int*   defineValues;
LINK int    numDefines;
LINK char** labels;
LINK word*  labelValues;
LINK int    numLabels;
LINK word   asmAddress;
LINK char   nests[10];
LINK int    numNests;

extern char* arrayRef(char* line, int v);
extern void cassign(char* line);
extern void cblockdata(char* line);
extern void ccall(char* line);
extern void ccommon(char* line);
extern void ccontinue(char* line);
extern void cdata(char* line);
extern void cdimension(char* line);
extern void cdo(char* line);
extern void cDoEnd();
extern void cend(char* line);
extern void cendfile(char* line);
extern void cexternal(char* line);
extern void cformat(char* line);
extern void cgoto(char* line);
extern void checkMain();
extern void cif(char* line);
extern void clet(char* line);
extern void copen(char* line);
extern void cpause(char* line);
extern void cprogram(char* line);
extern void cread(char* line);
extern void creturn(char* line);
extern void crewind(char* line);
extern void cstop(char* line);
extern void csubroutine(char* line);
extern void cwrite(char* line);
extern void ctype(char* line,int vtype);
extern char* getArg(char*line, char dest, char* rem);
extern word getCommon(char* name, char* module);
extern int  getVariable(char* name, char* module);
extern char* getVarName(char *line, char* token);
extern int  nextStatement();
extern void startup();
extern int  validVar(char *line);
extern char varType(int varNum);
extern void processRAM(char* buffer);
extern void processROM(char* buffer);

extern void  addLabel(char* label, word value);
int addVariable(char* name,char* module);
extern void  Asm(char* line);
extern void  casm(char* line);
extern char* cexpr(char* line, int etype);
extern int   compileLine(char* line);
extern int   findLine(word lineNumber, word* address);
extern int   findNextLine(word lineNumber, word* address);
extern int   findNextLineNumber(word lineNumber, word* address);
extern int   findVariable(char* name,char* module);
extern word  getHex(char* line);
extern char* getNumber(char* line, word* number);
extern void  library();
extern int   match(char* line, char* pattern);
extern void  optionFile(char* filename);
extern void  output(byte value);
extern int   pass(char* filename);
extern int   prepass(char* filename);
extern void  processOption(char* option);
extern void  showError(char* msg);
extern char* trim(char* line);

extern void writeAsm(char* line,char* rem);
extern word getLabel(char* label);
extern void addDefine(char* define, int value, int redefine);
extern int  getDefine(char* define);
extern void writeOutput();

#endif
