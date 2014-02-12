#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>

/* TODO: add support for numeric types?
 */
typedef enum column_type { STRING } column_type;

typedef struct column {
  char *name;
  char *type; /* change to column_type and fix read_column() */
  struct column *next;
} column;

/* A table is a TSV file.  These can be inputs to the
 * executable and temporary files created during the course
 * of execution.
 */
typedef struct table {
  char *filename;
  char *name;
  struct table *next;
  struct column *columns;
} table;

table *
new_table_from_path(char *path);

table *
new_table_with_tmp_path();

void table_print(table *tbl, FILE *fout);

/*  Columns are numbered 1, 2, ...
 *  Returns NO_SUCH_COLUMN if no column by that name.
 */
int
table_column_number(table *tbl, char *column_name);

/* Walk thru linked list of tables.  Returns NULL if table not found.
 */
table *
get_table_by_name(table *tables, char *table_name);

/* Internal routines which are declared
 * for unit tests.
 */
char *path_to_name(char *path);

column *read_column(char *header);

column *read_columns(char *path);


#endif
