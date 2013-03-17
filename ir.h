#ifndef ENGINE_H_
#define ENGINE_H_

/* should column.type be an enum */

typedef struct column {
  char *name;
  char *type;
  struct column *next;
} column;

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

typedef enum join_type { INNER, LEFT, RIGHT, FULL } join_type;

/* give joins all the attributes of tables? name, columns?
 *
 * embed table struct in join struct?
 */
typedef struct join {
  char *left_table_name; /* this could also be a join? */
  char *left_column_name;
  char *right_table_name;
  char *right_column_name;
  char *filename;
  enum join_type join_type;
  struct join *next;
} join;

typedef struct query {
  struct table *tables;
  struct join *joins;
  struct target *targets;
  struct expression where;
  char **where_table_names; /* so that we know how soon we can apply where filter */
  int *order_by_target_ids; /* store array size? */
  int *group_by_targets_ids; /* store array size? */
  expression *having;
  int limit; /* actually an expr... */
  int offset; /* actually an expr... */
} query;


/*  Columns are numbered 1, 2, ...
 *
 *  Returns 0 if no column by that name.
 */
int
column_name_to_number(table *tbl, char *column_name);

/* Returns NULL if table not found.
 */
table *
table_name_to_table(table *tables, char *table_name);


#endif
