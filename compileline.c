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

int compileLine(char* line) {
  listCount = 0;
  if (showCompiler && passNumber == 2) printf("%04x:",address);
  if (passNumber == 2 && createLst)
    fprintf(lstFile,"                  ; %s\n",line);
  if (passNumber == 2 && useAsm) {
    sprintf(buffer,"; %s",line); writeAsm(buffer,"");
    }
  if (statementLabel > 0) {
    sprintf(buffer,"%s_%d:", module, statementLabel); Asm(buffer);
    }
  line = trim(line);
  if (strncasecmp(line,"asm",3) == 0) casm(line+3);
  else if (strncasecmp(line, "assign", 6) == 0)     cassign(line+6);
  else if (strncasecmp(line, "blockdata", 9) ==0)   cblockdata(line+9);
  else if (strncasecmp(line, "byte", 4) == 0)       ctype(line+4,V_BYTE);
  else if (strncasecmp(line, "call", 4) == 0)       ccall(line+4);
  else if (strncasecmp(line, "common", 6) == 0)     ccommon(line+6);
  else if (strncasecmp(line, "continue", 8) == 0)   ccontinue(line+8);
  else if (strncasecmp(line, "data", 4) == 0)       cdata(line+4);
  else if (strncasecmp(line, "dimension", 9) == 0)  cdimension(line+9);
  else if (strncasecmp(line, "do", 2) == 0)         cdo(line+2);
  else if (strncasecmp(line, "endfile", 7) == 0)    cendfile(line+7);
  else if (strncasecmp(line, "end", 3) == 0)        cend(line+3);
  else if (strncasecmp(line, "external", 8) == 0)   cexternal(line+8);
  else if (strncasecmp(line, "format", 6) == 0)     cformat(line+6);
  else if (strncasecmp(line, "goto", 4) == 0)       cgoto(line+4);
  else if (strncasecmp(line, "if", 2) == 0)         cif(line+2);
  else if (strncasecmp(line, "integer", 7) == 0)    ctype(line+7,V_INTEGER);
  else if (strncasecmp(line, "logical", 7) == 0)    ctype(line+7,V_LOGICAL);
  else if (strncasecmp(line, "pause", 5) == 0)      cpause(line+5);
  else if (strncasecmp(line, "program", 7) == 0)    cprogram(line+7);
  else if (strncasecmp(line, "read", 4) == 0)       cread(line+4);
  else if (strncasecmp(line, "real", 4) == 0)       ctype(line+4,V_REAL);
  else if (strncasecmp(line, "return", 6) == 0)     creturn(line+6);
  else if (strncasecmp(line, "rewind", 6) == 0)     crewind(line+6);
  else if (strncasecmp(line, "short", 5) == 0)      ctype(line+5,V_SHORT);
  else if (strncasecmp(line, "stop", 4) == 0)       cstop(line+4);
  else if (strncasecmp(line, "subroutine",10) == 0) csubroutine(line+10, 'S', ' ');
  else if (strncasecmp(line, "write", 5) == 0)      cwrite(line+5);
  else clet(line);
  while (numDoLoops > 0 && doLoops[numDoLoops-1].line == statementLabel) {
    cDoEnd();
    numDoLoops--;
    }
  if (passNumber == 2 && showCompiler) printf("\n");
  return 0;
  }

