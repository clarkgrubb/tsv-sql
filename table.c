#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "engine.h"

const char *TABLE_SUFFIX = ".tsv";

char FIELD_SEPARATOR = '\t';

void
trim(char *s) {

  char *first = s;
  size_t i;

  while (isspace(*first)) {
    ++first;
  }

  int lede = first - s;

  if (lede) {

    for (i = 0; s + lede + i; ++i) {
      s[i] = s[lede + i];
    }

    s[i] = s[lede + i];

  }

  i = strlen(s) - 1;

  /* could i be -1 here? */

  while(isspace(s[i])) {
    --i;
  }

  s[i] = '\0';
}

char *
path_to_name(char *path) {

  char *base = basename(path);

  size_t suffix_index = strlen(base) - strlen(TABLE_SUFFIX);

  if (suffix_index < 1) {
    fprintf(stderr, "Name too short: %s", path);
    return NULL;
  }

  if (strcasecmp(base + suffix_index, TABLE_SUFFIX)) {
    fprintf(stderr, "Suffix not .tsv: %s", path);
    return NULL;
  }

  char *name = strdup(base);
  name[suffix_index] = '\0';

  return name;
}

column *
read_column(char **rest_of_line) {

  char *loc = index(*rest_of_line, FIELD_SEPARATOR);

  if (!loc) {

  }

  /* implement me */

  return NULL;
}

column *
read_columns(char *path) {

  char *line = NULL;
  size_t cap = 0;
  ssize_t len;

  len = getline(&line, &cap, stdin);

  if (len == -1) {
    if (ferror(stdin)) {
      perror("getline err: %s\n");
    }
    else if (feof(stdin)) {
      fprintf(stderr, "end of file\n");
    }

    return NULL;
  }

  char *rest = line;

  column *columns = NULL;
  column *last_col, *col;

  while (col = read_column(&rest)) {

    if (!columns) {
      columns = col;
    }

    last_col->next = col;
    last_col = col;
  }

  free(line);

  return columns;
}

table *
read_table(char *path) {

  table *tab = malloc(sizeof(table));

  tab->filename = strdup(path);

  tab->name = path_to_name(path);

  if (!tab->name) {
    return NULL;
  }

  tab->columns = read_columns(path);

  return tab;
}
