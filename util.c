#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <strings.h>

const char *make_tmpfile_template = "/tmp/tsv-sql.XXXXX.tsv";
const int make_tmpfile_suffix_length = 4;

char *
make_tmpfile() {

  char buf[PATH_MAX];

  strcpy(buf, make_tmpfile_template);

  int fd = mkstemps(buf, make_tmpfile_suffix_length);

  if (fd != -1) {

    if (close(fd) == -1) {
      perror("failed to close temp file");
      return NULL;
    }
    return strdup(buf);

  } else {
    perror("mkstemp failed");
    return NULL;
  }
}

void
trim(char *s) {

  char *first = s;
  size_t i;

  while (isspace(*first)) {
    ++first;
  }

  int lede = first - s;

  if (lede) {

    for (i = 0; *(s + lede + i); ++i) {
      s[i] = s[lede + i];
    }

    s[i] = s[lede + i];

  }

  i = strlen(s) - 1;

  while(i >= 0 && isspace(s[i])) {
    --i;
  }

  s[i + 1] = '\0';
}

char **
split(char *s, char delim) {

  char **parts;
  int i;

  if (strlen(s) == 0) {
    parts = calloc(1, sizeof(char *));
    return parts;
  }

  int delim_cnt = 0;

  for (i = 0; s[i]; ++i) {
    if (s[i] == delim) {
      ++delim_cnt;
    }
  }

  parts = calloc(delim_cnt + 2, sizeof(char *));

  char *lastp = s;
  char *p;

  i = 0;
  while (p = index(lastp, delim)) {

    parts[i] = (char *)calloc(p - lastp + 1, sizeof(char));
    strncpy(parts[i], lastp, p - lastp);

    lastp = p + 1;
    ++i;
  }


  parts[i] = (char *)calloc(strlen(lastp), sizeof(char));
  strcpy(parts[i], lastp);

  return parts;
}

void
free_split_array(char **split_array) {
    /* implement me */
}
