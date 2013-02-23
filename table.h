#ifndef TABLE_H_
#define TABLE_H_

#include "engine.h"


void trim(char *s);

char *path_to_name(char *path);

column *path_to_column(char **rest_of_line);

column *read_columns(char *path);

table *read_table(char *path);


#endif
