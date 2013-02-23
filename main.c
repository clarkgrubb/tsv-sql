#include <stdio.h>

extern char *top;

int
main(int argc, char **argv) {

  char *awk;

  if ( 0 == yyparse() ) {
    printf("awk: %s\n", top);
  } else {
    fprintf(stderr, "parse failed\n");
  }
}
