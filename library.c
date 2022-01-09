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
#include "library.h"

// R7 - data stack

void library() {
  int  i;
  char ctmp;
  FILE* file;
  ctmp = showCompiler;
  showCompiler = 0;

  file = fopen("library.asm","r");
  if (file == NULL) {
    i = 0;
    while (strstr(librarysrc[i],"__END__") == NULL) {
      Asm(librarysrc[i]);
      i++;
      }
    }
  else {
    fclose(file);
    Asm("#include library.asm");
    }

  showCompiler = ctmp;
  }

