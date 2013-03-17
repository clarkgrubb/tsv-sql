#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend.h"
#include "def.h"
#include "ir.h"

/* FIXME research what this value should be */
#define MAX_CMD_LEN 20000

char cmd_buf[MAX_CMD_LEN];

/*  'fmt' can contain the %s style format specifier.  The %% style
 *  escape sequence is also honored.
 */
int
substitute_tmpfiles(char *buf, char *fmt, step *prerequisites) {

  /* TODO: buffer offerflow problems? */

  char *bufp = buf;
  char *fmtp = fmt;
  step *prereq = prerequisites;

  while (1) {

    char *p = strchr(fmtp, '%');

    if (p == NULL) {

      if (prereq) {
        fprintf(stderr, "more tmpfiles than %%s specifiers\n");
        return FAILURE;
      }
      else {
        strcpy(bufp, fmtp);
        return SUCCESS;
      }
    }
    else {

      int len = p - fmtp;
      if (len > 0) {
        strncpy(bufp, fmtp, len);
        bufp += len;
      }

      switch (*(p + 1)) {
      case '%':
        *bufp = '%';
        bufp++;
        fmtp = p + 2;
        break;
      case 's':
        if (!prereq) {
          fprintf(stderr, "more %%s specifiers than tmpfiles\n");
          return FAILURE;
        }
        int tmpfile_len = strlen(prereq->tmpfile);
        strncpy(bufp, prereq->tmpfile, tmpfile_len);
        bufp += tmpfile_len;
        prereq = prereq->next;
        fmtp = p + 2;
        break;
      default:
        fprintf(stderr, "unrecognized format specifier: %%%d\n", *(p + 1));
        return FAILURE;
      }
    }
  }
}

char *
join_type_to_join_flag(join_type jt) {

  /* returning a string literal is ok */

  switch(jt) {
  case INNER:
    return "";
  case LEFT:
    return  "-a 1";
  case RIGHT:
    return "-a 2";
  case FULL:
    return "-a 1 -a 2";
  default:
    fprintf(stderr, "[ERROR] unrecognized join_type %d", jt);
    return NULL;
  }
}

int
execute_stdout_step(step *stp) {

  step *tmpfile_step;
  char *tmpfile;

  for (tmpfile_step = stp->tmpfile_prerequisites;
       tmpfile_step;
       tmpfile_step = tmpfile_step->next) {

    /* generate a tempfile; set tmpfile_step->tmpfile */

    execute_tmpfile_step(tmpfile_step);
  }

  if (substitute_tmpfiles(cmd_buf,
                          stp->stdout_cmd_fmt,
                          stp->tmpfile_prerequisites) == SUCCESS) {
    printf("[DEBUG]: about to execute: %s\n", cmd_buf);
    int retval = system(cmd_buf);
    return retval;
  }
  else {
    /* TODO error msg */
    return FAILURE;
  }
}

int
execute_tmpfile_step(step *stp) {

  step *tmpfile_step;
  char *tmpfile;

  for (tmpfile_step = stp->tmpfile_prerequisites;
       tmpfile_step;
       tmpfile_step = tmpfile_step->next) {

    /* generate a tempfile; set tmpfile_step->tmpfile */

    execute_tmpfile_step(tmpfile_step);
  }

  /* TODO: must generate a tmpfile name, set stp->tmpfile, and pass
   *  it to substitute_tmpfiles()
   */

  if (substitute_tmpfiles(cmd_buf, stp->tmpfile_cmd_fmt, stp) == SUCCESS) {
    int retval = system(cmd_buf);
    return retval;
  }
  else {
    /* TODO error msg */
    return FAILURE;
  }
}

/*
int
execute_join(query *qry, join *join) {

  assert(qry);
  assert(join);

  // what about header?
  snprintf(cmd_buf, MAX_CMD_LEN, "sort %s -t $'\\t' -k %d,%d %s > %s",
           left_numeric ? "-n" : "",
           left_column_number,
           left_column_number,
           left_path,
           sort_left_path);

  if (system(cmd_buf)) {
    fprintf("[ERROR] external cmd failed: %s\n", cmd_buf);
    return FAILURE;
  }

  // what about header?
  snprintf(cmd_buf, MAX_CMD_LEN, "sort %s -t $'\\t' -k %d,%d %s > %s",
           right_numeric ? "-n" : "",
           right_column_number,
           right_column_number,
           right_path,
           sort_right_path);

  if (system(cmd_buf)) {
    fprintf("[ERROR] external cmd failed: %s\n", cmd_buf);
    return FAILURE;
  }

  char *join_flag = join_type_to_join_flag(join->join_type);

  if (!join_flag) {
    return FAILURE;
  }

  snprintf(cmd_buf, MAX_CMD_LEN, "join -1 %d -2 %d %s %s %s > %s",
           left_column_number,
           right_column_number,
           join_flag,
           sort_left_path,
           sort_right_path,
           join_path);

  if (system(cmd_buf)) {
    fprintf("[ERROR] external cmd failed: %s\n", cmd_buf);
    return FAILURE;
  }

  return SUCCESS;
}
*/
