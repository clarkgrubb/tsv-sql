#include <assert.h>
#include <stdio.h>

#include "table.h"
#include "relation.h"

extern char *top;

/* TODO: declare these elsewhere...
 */
int yyparse();
void yy_scan_string(char *);

int
main(int argc, char **argv) {

  char *awk;

  if (argc == 1) {
    printf("USAGE: tsv-sql SELECT_STMT [FILE.tsv ...]\n");
    return 1;
  }

  char *select_stmt = argv[1];

  int i;

  query qry;
  table *tables = NULL;

  table *tbl = NULL;

  for (i = 2; i < argc; ++i) {
    if (!tbl) {
      tbl = new_table_from_path(argv[i]);
      assert(tbl);
      qry.tables = tbl;
    }
    else {
      tbl->next = new_table_from_path(argv[i]);
      assert(tbl->next);
      tbl = tbl->next;
    }
  }

  yy_scan_string(select_stmt);

  if ( 0 == yyparse() ) {
    printf("[DEBUG] parses as: %s\n", top);

    for (tbl = qry.tables; tbl; tbl = tbl->next) {
      table_print(tbl, stdout);
    }

  } else {
    fprintf(stderr, "[ERROR] parse failed\n");
  }
}
