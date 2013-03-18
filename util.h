#ifndef UTIL_H_
#define UTIL_H_

char *make_tmpfile();

void trim(char *s);

char **split(char *s, char delim);

void free_split_array(char **split_array);

#endif
