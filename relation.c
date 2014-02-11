#include <stdio.h>

#include "relation.h"
#include "table.h"

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
