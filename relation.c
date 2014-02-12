#include <stdio.h>

#include "def.h"
#include "relation.h"
#include "table.h"

void
relation_print(relation *rel) {

}

query *
new_query() {

}

/* returns SUCCESS
 */
int
query_add_table(table *tbl) {

  return SUCCESS;
}

/* returns SUCCESS
 */
int
query_set_from_relation(table *tbl) {

  return SUCCESS;
}

/* returns FAILURE if no FROM relation.
 */
int
query_add_join_relation() {
  return SUCCESS;
}
