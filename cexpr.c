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

#define OP_POS      0xa4
#define OP_IOSTATUS 0xa3
#define OP_IOFLAG   0xa2
#define OP_INP   0xa1
#define OP_EXP   0xa0
#define OP_LN    0x9f
#define OP_FLOAT 0x9e
#define OP_INT   0x9d
#define OP_FLG   0x9c
#define OP_PEEK  0x9b
#define OP_RND   0x9a
#define OP_SQRT  0x99
#define OP_ATAN  0x98
#define OP_ACOS  0x97
#define OP_ASIN  0x96
#define OP_TAN   0x95
#define OP_COS   0x94
#define OP_SIN   0x93
#define OP_SGN   0x92
#define OP_ABS   0x91
#define OP_POW   0x80
#define OP_MUL   0x70
#define OP_DIV   0x71
#define OP_MOD   0x72
#define OP_ADD   0x60
#define OP_SUB   0x61
#define OP_EQ    0x50
#define OP_NE    0x51
#define OP_LT    0x52
#define OP_GT    0x53
#define OP_LTE   0x54
#define OP_GTE   0x55
#define OP_NOT   0x40
#define OP_AND   0x30
#define OP_OR    0x21
#define OP_XOR   0x22
#define OP_CP    0x10
#define OP_OP    0x02
#define OP_END   0x01
#define OP_NUM   0x00

char* convertNumber(char* buffer, dword* value, byte* success, int* error, char* ntype) {
  char token[32];
  int  pos;
  INTREAL ir;
  *error = 0;
  *value = 0;
  *success = 0;
  *ntype = 'I';
  if (*buffer == '\'') {
    buffer++;
    while (*buffer != 0 && *buffer != '\'') {
      *value = (*value << 8) | *buffer;
      buffer++;
      }
    if (*buffer == 0) {
      showError("Unexpected end of line");
      *error = -1;
      return buffer;
      }
    buffer++;
    *success = 0xff;
    return buffer;
    }
  if ((*buffer == 'z' || *buffer == 'Z' || *buffer == 'x' || *buffer == 'X') &&
      *(buffer+1) == '\'') {
    buffer += 2;
    while ((*buffer >= 'a' && *buffer <= 'f') ||
           (*buffer >= 'A' && *buffer <= 'F') ||
           (*buffer >= '0' && *buffer <= '9')) {
      if (*buffer >= '0' && *buffer <= '9') *value = (*value << 4) + (*buffer - '0');
      if (*buffer >= 'a' && *buffer <= 'f') *value = (*value << 4) + (*buffer - 87);
      if (*buffer >= 'A' && *buffer <= 'F') *value = (*value << 4) + (*buffer - 55);
      buffer++;
      }
    if (*buffer != '\'') {
      showError("Invalid hex constant");
      *error = -1;
      return buffer;
      }
    buffer++;
    *success = 0xff;
    return buffer;
    }
  if ((*buffer >= '0' && *buffer <= '9') ||
      *buffer == '-' || *buffer == '.') {
    pos = 0;
    if (*buffer == '-') token[pos++] = *buffer++;
    while ((*buffer >= '0' && *buffer <= '9') || *buffer == '.') {
      if (*buffer == '.') *ntype = 'R';
      token[pos++] = *buffer++;
      }
    token[pos] = 0;
    if (*ntype == 'I') *value = atoi(token);
    else {
      ir.real = atof(token);
      *value = ir.integer;
      }
    *success = 0xff;
    return buffer;
    }
  return buffer;
  }

char* evaluate(char *pos, int *err, char* rtype, char *module) {
  char numbers[256];
  byte ops[256];
  int  nstack;
  int  ostack;
  int  op;
  int  flag;
  int  p;
  int  v;
  int  error;
  int  isArray;
  char ntype;
  char term;
  byte success;
  dword number;
  char token[64];
  char abuffer[256];
  nstack = 0;
  ostack = 0;
  op = 0;
  *err = 0;
  if (*pos == '-' && (*(pos+1) < '0' || *(pos+1) > '9')) {
    Asm("           sep     scall                     ; push constant to expr stack");
    Asm("           dw      epush");
    Asm("           db      0,0,0,0");
    ops[ostack++] = OP_SUB;
    pos++;
    }
  while (*pos  != 0 && op != OP_END) {
    flag = -1;
    isArray = 0;
    while (flag) {
      flag = 0;
      if (*pos == '(') {
        ops[ostack++] = OP_OP;
        flag = -1;
        }
      else if (strncasecmp(pos, "abs(", 4) == 0) {
        ops[ostack++] = OP_ABS;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "sgn(", 4) == 0) {
        ops[ostack++] = OP_SGN;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "cos(", 4) == 0) {
        ops[ostack++] = OP_COS;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "sin(", 4) == 0) {
        ops[ostack++] = OP_SIN;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "tan(", 4) == 0) {
        ops[ostack++] = OP_TAN;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "acos(", 5) == 0) {
        ops[ostack++] = OP_ACOS;
        ops[ostack++] = OP_OP;
        pos += 4;
        flag = -1;
        }
      else if (strncasecmp(pos, "asin(", 5) == 0) {
        ops[ostack++] = OP_ASIN;
        ops[ostack++] = OP_OP;
        pos += 4;
        flag = -1;
        }
      else if (strncasecmp(pos, "atan(", 5) == 0) {
        ops[ostack++] = OP_ATAN;
        ops[ostack++] = OP_OP;
        pos += 4;
        flag = -1;
        }
      else if (strncasecmp(pos, "sqrt(", 5) == 0) {
        ops[ostack++] = OP_SQRT;
        ops[ostack++] = OP_OP;
        pos += 4;
        flag = -1;
        }
      else if (strncasecmp(pos, "rnd(", 4) == 0) {
        ops[ostack++] = OP_RND;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "peek(", 5) == 0) {
        ops[ostack++] = OP_PEEK;
        ops[ostack++] = OP_OP;
        pos += 4;
        flag = -1;
        }
      else if (strncasecmp(pos, "flg(", 4) == 0) {
        ops[ostack++] = OP_FLG;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "int(", 4) == 0) {
        ops[ostack++] = OP_INT;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "float(", 6) == 0) {
        ops[ostack++] = OP_FLOAT;
        ops[ostack++] = OP_OP;
        pos += 5;
        flag = -1;
        }
      else if (strncasecmp(pos, "ln(", 3) == 0) {
        ops[ostack++] = OP_LN;
        ops[ostack++] = OP_OP;
        pos += 2;
        flag = -1;
        }
      else if (strncasecmp(pos, "exp(", 4) == 0) {
        ops[ostack++] = OP_EXP;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "inp(", 4) == 0) {
        ops[ostack++] = OP_INP;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "pos(", 4) == 0) {
        ops[ostack++] = OP_POS;
        ops[ostack++] = OP_OP;
        pos += 3;
        flag = -1;
        }
      else if (strncasecmp(pos, "ioflag(", 7) == 0) {
        if (!useElfos) {
          showError("Elf/OS mode not enabled");
          *err = -1;
          return pos;
          }
        ops[ostack++] = OP_IOFLAG;
        ops[ostack++] = OP_OP;
        pos += 6;
        flag = -1;
        }
      else if (strncasecmp(pos, "iostatus(", 9) == 0) {
        if (!useElfos) {
          showError("Elf/OS mode not enabled");
          *err = -1;
          return pos;
          }
        ops[ostack++] = OP_IOSTATUS;
        ops[ostack++] = OP_OP;
        pos += 8;
        flag = -1;
        }
      

      else {
        term = 0;
        pos = convertNumber(pos, &number, &success, &error, &ntype);
        if (error != 0) {
          *err = -1;
          return pos;
          }
        if (success != 0) {
          numbers[nstack++] = ntype;
          term = -1;
          pos--;
            Asm("           sep     scall                     ; push constant to expr stack");
            Asm("           dw      epush");
            sprintf(abuffer,"           db      %d,%d,%d,%d",
              (number >> 24) & 0xff,
              (number >> 16) & 0xff,
              (number >> 8) & 0xff,
              number & 0xff); Asm(abuffer);
          }

        if (term == 0) {
          if ((*pos >= 'a' && *pos <= 'z') ||
              (*pos >= 'A' && *pos <= 'Z')) {
            p = 0;
            while ((*pos >= 'a' && *pos <= 'z') ||
                   (*pos >= 'A' && *pos <= 'Z') ||
                   (*pos >= '0' && *pos <= '9') ||
                    *pos == '_') {
              token[p++] = *pos++;
              }
            token[p] = 0;

            if (*pos == '(') {
              v = findFunction(token);
              if (v >= 0) {
                strcpy(token, externals[v].name);
                pos = buildCall(token, pos);
                if (pos == NULL) {
                  *err = -1;
                  return pos;
                  }
                pos--;
                term = -1;
                switch (externals[v].type) {
                  case V_BYTE:
                  case V_SHORT:
                  case V_LOGICAL:
                  case V_INTEGER:
                       numbers[nstack++] = 'I';
                       break;
                  case V_REAL:
                       numbers[nstack++] = 'R';
                       break;
                  case V_DEFAULT:
                       if (externals[v].name[0] >= 'i' &&
                           externals[v].name[0] <= 'n')
                         numbers[nstack++] = 'I';
                       else if (externals[v].name[0] >= 'I' &&
                                externals[v].name[0] <= 'N')
                         numbers[nstack++] = 'I';
                       else
                         numbers[nstack++] = 'R';
                       break;
                  }
                }
              }
            if (term == 0) {
              v = getVariable(token, module);
              if (v < 0) {
                *err = -1;
                return pos;
                }
              if (*pos == '(') {
                if (variables[v].dimensions == 0) {
                  showError("Attempt to subscript a non-array variable");
                  *err = -1;
                  return pos;
                  }
                pos = arrayRef(pos, v);
                if (pos == NULL) {
                  *err = -1;
                  return pos;
                  }
                isArray = -1;
                Asm("           ghi     rf                        ; Save offset for later");
                Asm("           stxd");
                Asm("           glo     rf");
                Asm("           stxd");
                }
  
              if (variables[v].isArg && isArray == 0) {
                Asm("           sep     scall                     ; push variable to expr stack");
                if ((varType(v) == 'I' || varType(v) == 'R') && isArray == 0) {
                  Asm("           dw      vpush32p");
                  }
                if (varType(v) == 'S') {
                  Asm("           dw      vpush16p");
                  addDefine("VPUSH16P",1,1);
                  }
                if ((varType(v) == 'B' || varType(v) == 'L') && isArray == 0) {
                  addDefine("VPUSH8P",1,1);
                  Asm("           dw      vpush8p");
                  }
                sprintf(abuffer,"           dw      %s_%s",module,token);
                Asm(abuffer);
                numbers[nstack++] = (varType(v) == 'R') ? 'R' : 'I';
                }
  
              else if ((varType(v) == 'I' || varType(v) == 'R') && isArray == 0) {
                Asm("           sep     scall                     ; push variable to expr stack");
                Asm("           dw      vpush32");
                if (strlen(variables[v].common) > 0) {
                  sprintf(abuffer,"           dw      c_%s+%d",variables[v].common,variables[v].offset);
                  }
                else {
                  sprintf(abuffer,"           dw      %s_%s",module,token);
                  }
                Asm(abuffer);
                if (varType(v) == 'I') numbers[nstack++] = 'I';
                if (varType(v) == 'R') numbers[nstack++] = 'R';
                }
              else {
                if (strlen(variables[v].common) > 0) {
                  sprintf(abuffer,"           ldi     (c_%s+%d).1             ; Point to variable",
                          variables[v].common, variables[v].offset);
                  Asm(abuffer);
                  Asm("           phi     rf");
                  sprintf(abuffer,"           ldi     (c_%s+%d).0             ; Point to variable",
                          variables[v].common, variables[v].offset);
                  Asm(abuffer);
                  }
                else {
                  sprintf(abuffer,"           ldi     %s_%s.1             ; Point to variable",module,token);
                  Asm(abuffer);
                  Asm("           phi     rf");
                  sprintf(abuffer,"           ldi     %s_%s.0             ; Point to variable",module,token);
                  Asm(abuffer);
                  }
                Asm("           plo     rf");
                if (variables[v].isArg) {
                  Asm("           lda     rf                        ; Retrieve pointed to address");
                  Asm("           plo     re");
                  Asm("           lda     rf");
                  Asm("           plo     rf");
                  Asm("           glo     re");
                  Asm("           phi     rf");
                  }
                if (isArray) {
                  Asm("           irx                               ; Add array cell offset");
                  Asm("           glo     rf");
                  Asm("           add");
                  Asm("           plo     rf");
                  Asm("           irx");
                  Asm("           ghi     rf");
                  Asm("           adc");
                  Asm("           phi     rf");
                  isArray = 0;
                  }
                Asm("           sex     r7");
                if (variables[v].type == V_BYTE || variables[v].type == V_LOGICAL) {
                  Asm("           ldi     0                   ; place value onto expr stack");
                  Asm("           stxd");
                  Asm("           stxd");
                  Asm("           stxd");
                  Asm("           ldn     rf");
                  Asm("           stxd");
                  numbers[nstack++] = 'I';
                  }
                else if (variables[v].type == V_SHORT) {
                  Asm("           ldi     0                   ; place value onto expr stack");
                  Asm("           stxd");
                  Asm("           stxd");
                  Asm("           lda     rf");
                  Asm("           stxd");
                  Asm("           ldn     rf");
                  Asm("           stxd");
                  numbers[nstack++] = 'I';
                  }
                else if (variables[v].type == V_INTEGER ||
                         variables[v].type == V_REAL ||
                         variables[v].type == V_DEFAULT) {
                  Asm("           lda     rf                  ; place value onto expr stack");
                  Asm("           stxd");
                  Asm("           lda     rf");
                  Asm("           stxd");
                  Asm("           lda     rf");
                  Asm("           stxd");
                  Asm("           ldn     rf");
                  Asm("           stxd");
                  if (variables[v].type == V_INTEGER) numbers[nstack++] = 'I';
                  if (variables[v].type == V_REAL) numbers[nstack++] = 'R';
                  if (variables[v].type == V_DEFAULT) {
                    if (variables[v].name[0] >= 'i' && variables[v].name[0] <= 'n') numbers[nstack++] = 'I';
                    else if (variables[v].name[0] >= 'I' && variables[v].name[0] <= 'N') numbers[nstack++] = 'I';
                    else numbers[nstack++] = 'R';
                    }
                  }
                Asm("           sex     r2");
                }
              term = -1;
              pos--;
              }
            }
          }
        if (term == 0) { printf("Non-number found\n"); return 0; }
        }

      if (*pos != 0) pos++;
      pos = trim(pos);
      }
    flag = -1;
    while (flag) {
      flag = 0;
      op = 0;
      switch (*pos) {
        case 0  : op = OP_END; break;
        case ',': op = OP_END; break;
        case '*':
             if (*(pos+1) == '*') { op = OP_POW; pos++; }
               else op = OP_MUL;
             break;
        case '/': op = OP_DIV; break;
        case '%': op = OP_MOD; break;
        case '+': op = OP_ADD; break;
        case '-': op = OP_SUB; break;
        case '^': op = OP_XOR; break;
        case ')': op = OP_CP ; break;
        case '&': op = OP_AND; break;
        case '|': op = OP_OR;  break;
        case '=': op = OP_EQ;  break;
        case '<':
             if (*(pos+1) == '>') { op = OP_NE; pos++; }
             else if (*(pos+1) == '=') { op = OP_LTE; pos++; }
             else op = OP_LT;
             break;
        case '>':
             if (*(pos+1) == '=') { op = OP_GTE; pos++; }
             else op = OP_GT;
             break;
        }
      if (op == 0) {
        sprintf(abuffer,"Invalid OP %c (%02x)",*pos, *pos);
        showError(abuffer);
        return 0;
        }
      while (ostack > 0 && (ops[ostack-1] & 0xf0) >= (op & 0xf0)) {
        nstack--;
        ostack--;
        if (ops[ostack] >= 0x50 && ops[ostack] < 0x80 &&
            numbers[nstack-1] != numbers[nstack]) {
          if (numbers[nstack-1] == 'I') {
            Asm("           inc     r7                  ; Convert first number to floating point");
            Asm("           inc     r7");
            Asm("           inc     r7");
            Asm("           inc     r7");
            Asm("           sep     scall");
            Asm("           dw      itof");
            Asm("           dec     r7");
            Asm("           dec     r7");
            Asm("           dec     r7");
            Asm("           dec     r7");
            numbers[nstack-1] = 'R';
            addDefine("USEFP",1,1);
            }
          if (numbers[nstack] == 'I') {
            Asm("           sep     scall");
            Asm("           dw      itof");
            addDefine("USEFP",1,1);
            numbers[nstack] = 'R';
            }
          }
        if (ops[ostack] >= 0x20 && ops[ostack] < 0x40) {
          if (numbers[nstack-1] == 'R') {
            Asm("           inc     r7                  ; Convert first number to integer");
            Asm("           inc     r7");
            Asm("           inc     r7");
            Asm("           inc     r7");
            Asm("           sep     scall");
            Asm("           dw      ftoi");
            Asm("           dec     r7");
            Asm("           dec     r7");
            Asm("           dec     r7");
            Asm("           dec     r7");
            numbers[nstack-1] = 'I';
            addDefine("USEFP",1,1);
            }
          if (numbers[nstack] == 'R') {
            Asm("           sep     scall");
            Asm("           dw      ftoi");
            addDefine("USEFP",1,1);
            numbers[nstack] = 'I';
            }
          }
        switch (ops[ostack]) {
          case OP_MUL :
               Asm("           sep     scall               ; Perform multiplication");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      mul32");
                 addDefine("MUL32",1,1);
                 }
               else {
                 Asm("           dw      mulfp");
                 addDefine("MULFP",1,1);
                 }
               break;
          case OP_DIV :
               Asm("           sep     scall               ; Perform division");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      div32");
                 addDefine("DIV32",1,1);
                 }
               else {
                 Asm("           dw      divfp");
                 addDefine("DIVFP",1,1);
                 }
               break;
          case OP_MOD :
               Asm("           sep     scall               ; Perform modulo");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      mod32");
                 addDefine("MOD32",1,1);
                 }
               break;
          case OP_ADD :
               Asm("           sep     scall               ; Perform addition");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      add32");
                 addDefine("ADD32",1,1);
                 }
               else {
                 Asm("           dw      addfp");
                 addDefine("ADDFP",1,1);
                 }
               break;
          case OP_SUB :
               Asm("           sep     scall               ; Perform subtraction");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      sub32");
                 addDefine("SUB32",1,1);
                 }
               else {
                 Asm("           dw      subfp");
                 addDefine("SUBFP",1,1);
                 }
               break;
          case OP_AND :
               Asm("           sep     scall               ; Perform bitwise and");
               Asm("           dw      and32");
               addDefine("AND32",1,1);
               break;
          case OP_OR  :
               Asm("           sep     scall               ; Perform bitwise or");
               Asm("           dw      or32");
               addDefine("OR32",1,1);
               break;
          case OP_XOR :
               Asm("           sep     scall               ; Perform bitwise xor");
               Asm("           dw      xor32");
               addDefine("XOR32",1,1);
               break;
          case OP_EQ  :
               Asm("           sep     scall               ; Perform equals");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      eq32");
                 addDefine("EQ32",1,1);
                 }
               else {
                 Asm("           dw      eqfp");
                 addDefine("EQFP",1,1);
                 }
               break;
          case OP_NE  :
               Asm("           sep     scall               ; Perform not equals");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      ne32");
                 addDefine("NE32",1,1);
                 }
               else {
                 Asm("           dw      nefp");
                 addDefine("NEFP",1,1);
                 }
               break;
          case OP_LT  :
               Asm("           sep     scall               ; Perform less than");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      lt32");
                 addDefine("LT32",1,1);
                 }
               else {
                 Asm("           dw      ltfp");
                 addDefine("LTFP",1,1);
                 }
               break;
          case OP_GT  :
               Asm("           sep     scall               ; Perform greater than");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      gt32");
                 addDefine("GT32",1,1);
                 }
               else {
                 Asm("           dw      gtfp");
                 addDefine("GTFP",1,1);
                 }
               break;
          case OP_LTE :
               Asm("           sep     scall               ; Perform less or equal");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      lte32");
                 addDefine("LTE32",1,1);
                 }
               else {
                 Asm("           dw      ltefp");
                 addDefine("LTEFP",1,1);
                 }
               break;
          case OP_GTE :
               Asm("           sep     scall               ; Perform greater or equal");
               if (numbers[nstack-1] == 'I' && numbers[nstack] == 'I') {
                 Asm("           dw      gte32");
                 addDefine("GTE32",1,1);
                 }
               else {
                 Asm("           dw      gtefp");
                 addDefine("GTEFP",1,1);
                 }
               break;
          case OP_POW :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               if (numbers[nstack-1] == 'I') {
                 Asm("           inc     r7                  ; Convert first number to integer");
                 Asm("           inc     r7");
                 Asm("           inc     r7");
                 Asm("           inc     r7");
                 Asm("           sep     scall");
                 Asm("           dw      itof");
                 Asm("           dec     r7");
                 Asm("           dec     r7");
                 Asm("           dec     r7");
                 Asm("           dec     r7");
                 numbers[nstack-1] = 'R';
                 addDefine("USEFP",1,1);
                 }
               Asm("           sep     scall               ; Perform power");
               Asm("           dw      fppow");
               addDefine("POWFP",1,1);
               break;
          case OP_ABS :
               Asm("           sep     scall               ; Perform greater or equal");
               if (numbers[nstack] == 'I') {
                 Asm("           dw      abs32");
                 addDefine("ABS32",1,1);
                 }
               else {
                 Asm("           dw      absfp");
                 addDefine("ABSFP",1,1);
                 }
               break;
          case OP_COS :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform cos");
               Asm("           dw      fpcos");
               addDefine("COSFP",1,1);
               break;
          case OP_SIN :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform sin");
               Asm("           dw      fpsin");
               addDefine("SINFP",1,1);
               break;
          case OP_TAN :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform tan");
               Asm("           dw      fptan");
               addDefine("TANFP",1,1);
               break;
          case OP_ACOS :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform acos");
               Asm("           dw      fpacos");
               addDefine("ACOSFP",1,1);
               break;
          case OP_ASIN :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform asin");
               Asm("           dw      fpasin");
               addDefine("ASINFP",1,1);
               break;
          case OP_ATAN :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform atan");
               Asm("           dw      fpatan");
               addDefine("ATANFP",1,1);
               break;
          case OP_SQRT :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform square root");
               Asm("           dw      fpsqrt");
               addDefine("SQRTFP",1,1);
               break;
          case OP_SGN :
               Asm("           sep     scall               ; Perform sgn");
               if (numbers[nstack] == 'I') {
                 Asm("           dw      sgn32");
                 addDefine("SGN32",1,1);
                 }
               else {
                 Asm("           dw      sgnfp");
                 addDefine("SGNFP",1,1);
                 }
               break;
          case OP_RND :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           sep     scall               ; Perform random");
               Asm("           dw      rnd32");
               addDefine("RND32",1,1);
               break;
          case OP_POS :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           sep     scall               ; get file position");
               Asm("           dw      fpos");
               addDefine("FPOS",1,1);
               break;
          case OP_PEEK :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           inc     r7                  ; Retrieve address for peek");
               Asm("           lda     r7");
               Asm("           plo     rf");
               Asm("           ldn     r7");
               Asm("           phi     rf");
               Asm("           inc     r7                  ; Remove high word");
               Asm("           inc     r7");
               Asm("           ldi     0                   ; Result is a byte, so MSB=0");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           ldn     rf");
               Asm("           str     r7");
               Asm("           dec     r7");
               break;
          case OP_FLG :
               numbers[nstack] = 'I';
               Asm("           inc     r7                  ; FLG needs no args, so remove dummy");
               Asm("           inc     r7");
               Asm("           inc     r7");
               Asm("           inc     r7");
               Asm("           ldi     0                   ; Result is a byte, so MSB=0");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           sep     scall               ; Read EF flags");
               Asm("           dw      sampleef");
               Asm("           str     r7");
               Asm("           dec     r7");
               addDefine("USEEF",1,1);
               break;
          case OP_INT :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to integer");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               break;
          case OP_FLOAT :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'F';
                 }
               break;
          case OP_LN :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform ln");
               Asm("           dw      fpln");
               addDefine("LNFP",1,1);
               break;
          case OP_EXP :
               if (numbers[nstack] == 'I') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      itof");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'R';
                 }
               Asm("           sep     scall               ; Perform exp");
               Asm("           dw      fpexp");
               addDefine("EXPFP",1,1);
               break;
          case OP_INP :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           ldi     0d3h                ; Push SEP R3 onto stack");
               Asm("           stxd");
               Asm("           inc     r7                  ; Retrieve port");
               Asm("           lda     r7");
               Asm("           inc     r7                  ; Remove high word");
               Asm("           inc     r7");
               Asm("           ani     7                   ; Keep only bottom 3 bits");
               Asm("           adi     068h                ; convert to INP instruction");
               Asm("           stxd");
               Asm("           glo     r2                  ; Copy stack address to RF");
               Asm("           plo     rf");
               Asm("           ghi     r2");
               Asm("           phi     rf");
               Asm("           inc     rf                  ; RF now points to INP command");
               Asm("           sep     rf                  ; Execute code on stack");
               Asm("           ldi     0                   ; High byte is zero");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           str     r7");
               Asm("           dec     r7");
               Asm("           ldx                         ; Retrieve value from INP");
               Asm("           str     r7");
               Asm("           dec     r7");
               break;
          case OP_IOFLAG :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           sep     scall               ; Perform ioflag()");
               Asm("           dw      ioflag");
               addDefine("IOFLAG",1,1);
               break;
          case OP_IOSTATUS :
               if (numbers[nstack] == 'R') {
                 Asm("           sep     scall               ; Convert to floating point");
                 Asm("           dw      ftoi");
                 addDefine("USEFP",1,1);
                 numbers[nstack] = 'I';
                 }
               Asm("           sep     scall               ; Perform iostatus()");
               Asm("           dw      iostatus");
               addDefine("IOSTATUS",1,1);
               break;
          }
        if (ops[ostack] >= 0x90) nstack++;
        }
      if (op != OP_END) {
        if (op == OP_CP) {
          if (ops[ostack-1] != OP_OP) {
            op = OP_END;
            }
          else {
            ostack--;
            flag = -1;
            }
          }
        else
          ops[ostack++] = op;
        if (op != OP_END) {
          pos++;
          while (*pos == ' ') pos++;
          pos = trim(pos);
         }
        }
      }
    }
  if (nstack == 0) { printf("nstack empty\n"); return pos; }
  if (nstack != 1) {
     sprintf(abuffer,"Did not reduce to 1 term: %d\n",nstack);
     showError(abuffer);
     return pos;
     }
  *rtype = numbers[0];
  return pos;
  }


char* cexpr(char* line, int etype, char* prefix) {
  int error;
  char rtype;
  line = evaluate(line, &error, &rtype, prefix);
  exprErrors = error;
  if (line == 0) {
    exprErrors++;
    return 0;
    }
  if (error != 0) return line;
  if (etype == 0 && rtype == 'R') {
    Asm("           sep     scall");
    Asm("           dw      ftoi");
    }
  if (etype == 1 && rtype == 'I') {
    Asm("           sep     scall");
    Asm("           dw      itof");
    addDefine("USEFP",1,1);
    }
  return line;
  }

