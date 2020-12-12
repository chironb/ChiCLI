#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* Compiling, deleting object files, and launching VICE in fast prg loading mode: 
cl65 -g -Osr -t c64 --static-locals  echoarg.c -o echoarg.prg  &&  rm *.o  &&  x64sc -autostartprgmode 1 echoarg.prg
*/ 

// https://www.cc65.org/doc/c64-7.html
// RUN:REM ARG1 " ARG2 IS QUOTED" ARG3 "" ARG5

// https://www.cc65.org/snapshot-doc/funcref-144.html#getopt
// getopt int __fastcall__ getopt (int argc, char* const* argv, const char* optstring);

// https://www.cc65.org/snapshot-doc/funcref-135.html#exec
// exec int __fastcall__ exec (const char* progname, const char* cmdline);

int main( int argc, char* argv[] ) {

  if (argc > 1)printf("%s " , argv[ 1]);
  if (argc > 2)printf("%s " , argv[ 2]);
  if (argc > 3)printf("%s " , argv[ 3]);
  if (argc > 4)printf("%s " , argv[ 4]);
  if (argc > 5)printf("%s " , argv[ 5]);
  if (argc > 6)printf("%s " , argv[ 6]);
  if (argc > 7)printf("%s " , argv[ 7]);
  if (argc > 8)printf("%s " , argv[ 8]);
  if (argc > 9)printf("%s " , argv[ 9]);
  if (argc >10)printf("%s " , argv[10]);
  printf("\n");
  exec("chicli", "-st");
  return EXIT_SUCCESS;

};//end_main