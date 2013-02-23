#ifndef ENGINE_H_
#define ENGINE_H_

typedef int column_type;

typedef struct column {
  char *name;
  column_type type;
  struct column *next;
} column;

enum expression_type { ADD, SUBTRACT };

enum aggregration_type { COUNT, SUM, MIN, MAX, AVG, STDDEV };

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
  struct expression expr;
  struct target *next;
} target;

/* table alias to table map */

typedef struct table {
  char *filename;
  char *name;
  struct column *columns;
  struct table *next;
} table;

typedef struct query {
  struct table *tables;
  struct target *targets;
  struct expression where;
  char **where_table_names; /* so that we know how soon we can apply where filter */
  int *order_by_target_ids; /* store array size? */
  int *group_by_targets_ids; /* store array size? */
  expression *having;
  int limit; /* actually an expr... */
  int offset; /* actually an expr... */
} query;

#endif
