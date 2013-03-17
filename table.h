#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>

#include "ir.h"

char *path_to_name(char *path);

column *path_to_column(char **rest_of_line);

column *read_column(char *header);

column *read_columns(char *path);

table *read_table(char *path);

void print_table(FILE *fout, table *tbl);

#endif
