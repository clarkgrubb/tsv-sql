#include <stdio.h>

#include "ir.h"

const int NO_SUCH_COLUMN = 0;

/*  Columns are numbered 1, 2, ...
 *
 *  Returns NO_SUCH_COLUMN if appropriate.
 */
int
column_name_to_number(table *tbl, char *column_name) {

  column *col;
  int col_num;

  for (col = tbl->columns, col_num = 1; col; col = col->next, ++col_num) {

    if (strcmp(column_name, col->name) == 0) {
      return col_num;
    }
  }

  return NO_SUCH_COLUMN;
}

/* Returns NULL if table not found.
 */
table *
table_name_to_table(table *tables, char *table_name) {

  table *tbl;

  for (tbl = tables; tbl; tbl = tbl->next) {
    if (strcmp(table_name, tbl->name) == 0) {
      return tbl;
    }
  }

  return NULL;
}
