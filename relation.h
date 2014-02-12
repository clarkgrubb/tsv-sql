#ifndef RELATION_H_
#define RELATION_H_

#include "table.h"

typedef enum { TABLE, JOIN, QUERY } relation_type;
typedef enum join_type { INNER, LEFT, RIGHT, FULL } join_type;
typedef enum expression_type { ADD, SUBTRACT } expression_type;
typedef enum aggregration_type { COUNT, SUM, MIN, MAX, AVG, STDDEV } aggregation_type;

/* must be able to convert this to awk;
 column names must be replaced by column numbers
*/
typedef struct expression {
  enum expression_type head;
  struct expression *arguments;
  int is_aggregate;
} expression;

typedef struct target {
  int id;
  char *alias;
  struct expression expr;
  struct target *next;
} target;

/* A relation is the base object for
 * tables, joins, and queries.
 *
 * From the perspective of the execution engine,
 * joins are a tree.
 *
 * However, the parser always created joins which are
 * a leftward expanding tree with the from table
 * at the far left.
 */
typedef struct relation {

  relation_type type;
  struct table *table;


  /* For a table, the columns are the same as table.columns.
   * If there is an alias (in the FROM claus) it is appended.
   *
   * For a join, the alias (if any) is appended to the right
   * relation.
   *
   * Aliases are required for subqueries.  The top level query
   * has a NULL alias.
   */
  char *alias;
  struct column *columns;

  /* type: JOIN */
  char *left_relation;
  char *left_column_name;
  char *right_relation;
  char *right_column_name;
  enum join_type join_type;

  /* type: QUERY */
  table *tables;             /* how do we handle subqueries? */
  struct relation *joins;    /* NULL if no FROMclause; table relation if FROM but no JOIN */
  struct target *targets;
  struct expression where;
  char **where_table_names;  /* so that we know how soon we can apply where filter */
  int *order_by_target_ids;  /* store array size? */
  int *group_by_targets_ids; /* store array size? */
  expression *having;
  int limit;                 /* actually an expr... */
  int offset;                /* actually an expr... */
} relation;

/* TODO: do we need these? */
typedef struct relation query;
typedef struct relation join;

void
relation_print(relation *rel);

query *
new_query();

/* returns SUCCESS
 */
int
query_add_table(table *tbl);

/* returns SUCCESS
 */
int
query_set_from_relation(table *tbl);

/* returns FAILURE if no FROM relation.
 */
int
query_add_join_relation();

int
query_set_where();

int
query_set_order_by();

int
query_set_group_by();

int
query_set_having();

int
query_set_limit();

int
query_set_offset();

#endif
