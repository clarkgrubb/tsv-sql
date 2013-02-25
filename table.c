#include <ctype.h>
#include <libgen.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "util.h"

const char *TABLE_SUFFIX = ".tsv";

/* Should types be enums?  We would still need a string to int map. */
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

  len = getline(&line, &cap, stdin);

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
  column *last_col, *col;

  int i;

  for (i = 0; headers[i]; ++i) {

    col = read_column(headers[i]);

    if (!col) {
      return NULL;
    }

    if (!columns) {
      columns = col;
    }

    last_col->next = col;
    last_col = col;
  }

  free(line);
  free_split_array(headers);

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
