#include <assert.h>
#include <ctype.h>
#include <libgen.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "util.h"

const char *TABLE_SUFFIX = ".tsv";
const int NO_SUCH_COLUMN = 0;

/* FIXME: Should types be enums?  We would still need a string to int map. */
const char *DEFAULT_TYPE = "text";

char FIELD_SEPARATOR = '\t';

/* Should match IDENT in tokens.l.
   Note that max identifier and type lengths are 100 */

#define IDENT_BUFSIZE 101

const char *HEADER_REGEX =
  "^([a-zA-Z][a-zA-Z0-9_]{1,99})(\\{[a-zA-Z]{1,100}\\})?$";

char *
path_to_name(char *path) {

  char *base = basename(path);

  size_t suffix_index = strlen(base) - strlen(TABLE_SUFFIX);

  if (suffix_index < 1) {
    fprintf(stderr, "[ERROR] Name too short: %s\n", path);
    return NULL;
  }

  if (strcasecmp(base + suffix_index, TABLE_SUFFIX)) {
    fprintf(stderr, "[ERROR] Suffix not .tsv: %s\n", path);
    return NULL;
  }

  char *name = strdup(base);
  name[suffix_index] = '\0';

  return name;
}

column *
read_column(char *header) {

  regex_t rx;
  int retval;

  if (retval = regcomp(&rx, HEADER_REGEX, REG_EXTENDED)) {

    char buf[200];

    regerror(retval, &rx, buf, 200);
    fprintf(stderr, "Regex error: %s\n", buf);

    return NULL;
  }
  else {

    regmatch_t matches[3];

    matches[2].rm_so = -1;

    if (regexec(&rx, header, 3, matches, 0) == 0) {

      char name[IDENT_BUFSIZE];
      char type[IDENT_BUFSIZE];
      regmatch_t rm = matches[1];

      strncpy(name, header + rm.rm_so, rm.rm_eo - rm.rm_so);
      name[rm.rm_eo - rm.rm_so + 1] = '\0';

      rm = matches[2];

      if (matches[2].rm_so != -1) {

        /* there was a type in curly braces { } */

        strncpy(type, header + rm.rm_so + 1, rm.rm_eo - rm.rm_so - 2);
        type[rm.rm_eo - rm.rm_so - 2] = '\0';
      }
      else {
        strcpy(type, "text");
      }

      column *col = malloc(sizeof(column));

      col->name = strdup(name);
      col->type = strdup(type);
      col->next = NULL;

      return col;
    }

    regfree(&rx);
  }

  return NULL;
}

column *
read_columns(char *path) {

  char *line = NULL;
  size_t cap = 0;
  ssize_t len;

  FILE *f = fopen(path, "r");

  if (!f) {
    fprintf(stderr, "[ERROR] opening file %s", path);
    perror("[ERROR] could not open file");
    return NULL;
  }

  len = getline(&line, &cap, f);

  if (len == -1) {
    if (ferror(stdin)) {
      perror("[ERROR] getline: %s\n");
    }
    else if (feof(stdin)) {
      fprintf(stderr, "[ERROR] end of file\n");
    }

    return NULL;
  }

  trim(line);

  char **headers = split(line, FIELD_SEPARATOR);

  column *columns = NULL;
  column *col = NULL;

  int i;

  for (i = 0; headers[i]; ++i) {

    if (!col) {
      col = read_column(headers[i]);
    }
    else {
      col->next = read_column(headers[i]);
      col = col->next;
    }

    if (!col) {
      return NULL;
    }

    if (!columns) {
      columns = col;
    }
  }

  free(line);
  free_split_array(headers);

  return columns;
}

table *
new_table_from_path(char *path) {

  table *tab = malloc(sizeof(table));

  tab->filename = strdup(path);

  tab->name = path_to_name(path);

  if (!tab->name) {
    return NULL;
  }

  tab->columns = read_columns(path);

  tab->next = NULL;

  return tab;
}

table *
new_table_with_tmp_path() {
  table *tab = (table *)malloc(sizeof(table));
  tab->filename = make_tmpfile();
  tab->name = NULL;
  tab->next = NULL;
  tab->columns = NULL;
  return tab;
}

void
table_print(table *tbl, FILE *fout) {

  assert(tbl);

  fprintf(fout, "table: %s (%s)\n", tbl->name, tbl->filename);

  column *col;

  for (col = tbl->columns; col; col = col->next) {
    fprintf(fout, "  %s{%s}\n", col->name, col->type);
  }

}

/*  Columns are numbered 1, 2, ...
 *
 *  Returns NO_SUCH_COLUMN if appropriate.
 */
int
table_column_number(table *tbl, char *column_name) {

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
