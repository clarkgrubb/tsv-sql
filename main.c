#include <stdio.h>

extern char *top;

int
main(int argc, char **argv) {

  char *awk;

  if (argc == 1) {
    printf("USAGE: tsv-sql SELECT_STMT [FILE.tsv ...]\n");
    return -1;
  }

  char *select_stmt = argv[1];

  yy_scan_string(select_stmt);

  if ( 0 == yyparse() ) {
    printf("[DEBUG] parses as: %s\n", top);
  } else {
    fprintf(stderr, "[ERROR] parse failed\n");
  }
}
