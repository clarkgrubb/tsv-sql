#include <assert.h>
#include <stdio.h>

#include "engine.h"
#include "table.h"

extern char *top;

int
main(int argc, char **argv) {

  char *awk;

  if (argc == 1) {
    printf("USAGE: tsv-sql SELECT_STMT [FILE.tsv ...]\n");
    return -1;
  }

  char *select_stmt = argv[1];

  int i;

  query qry;
  qry.tables = NULL;

  table *tbl = NULL;

  for (i = 2; i < argc; ++i) {
    if (!tbl) {
      tbl = read_table(argv[i]);
      assert(tbl);
      qry.tables = tbl;
    }
    else {
      tbl->next = read_table(argv[i]);
      assert(tbl->next);
      tbl = tbl->next;
    }
  }

  yy_scan_string(select_stmt);

  if ( 0 == yyparse() ) {
    printf("[DEBUG] parses as: %s\n", top);

    for (tbl = qry.tables; tbl; tbl = tbl->next) {
      print_table(stdout, tbl);
    }

  } else {
    fprintf(stderr, "[ERROR] parse failed\n");
  }
}
