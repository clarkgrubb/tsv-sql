#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend.h"
#include "def.h"
#include "ir.h"
#include "util.h"

/* FIXME research what this value should be */
#define MAX_CMD_LEN 20000

char cmd_buf[MAX_CMD_LEN];

/*  'fmt' can contain these specifiers:
 *
 *     %i: step.tmpfile_prerequisites.tmpfile
 *     %o: step.tmpfile
 *     %%: %
 */
int
format_step(char *buf, char *fmt, step *stp) {

  /* TODO: buffer offerflow problems? */

  if (!buf) {
    fprintf(stderr, "[ERROR] format_step(): buf was NULL\n");
    return FAILURE;
  }

  if (!fmt) {
    fprintf(stderr, "[ERROR] format_step(): fmt was NULL\n");
    return FAILURE;
  }

  if (!stp) {
    fprintf(stderr, "[ERROR] format_step(): stp was NULL\n");
    return FAILURE;
  }

  char *bufp = buf;
  char *fmtp = fmt;
  step *prereq = stp->tmpfile_prerequisites;
  int tmpfile_len;

  if (stp->stdin_prerequisite) {

    if (format_step(buf,
                    stp->stdin_prerequisite->stdout_cmd_fmt,
                    stp->stdin_prerequisite) == FAILURE) {
      return FAILURE;
    }

    int buflen = strlen(buf);

    strncpy(buf + buflen, " | ", 3);
    bufp = buf + buflen + 3;
  }

  while (1) {

    char *p = strchr(fmtp, '%');

    if (p == NULL) {

      if (prereq) {
        fprintf(stderr, "[ERROR] format_step(): "
                "more tmpfile_prerequisites than %%i specifiers\n");
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
      case 'i':
        if (!prereq) {
          fprintf(stderr, "[ERROR] format_step(): "
                  "more %%i specifiers than tmpfile_prerequisites\n");
          return FAILURE;
        }
        tmpfile_len = strlen(prereq->tmpfile);
        strncpy(bufp, prereq->tmpfile, tmpfile_len);
        bufp += tmpfile_len;
        prereq = prereq->next;
        fmtp = p + 2;
        break;
      case 'o':
        if (!stp->tmpfile) {
          fprintf(stderr, "[ERROR] format_step(): "
                  "tmpfile for step was not set.\n");
          return FAILURE;
        }
        tmpfile_len = strlen(stp->tmpfile);
        strncpy(bufp, stp->tmpfile, tmpfile_len);
        bufp += tmpfile_len;
        fmtp = p + 2;
        break;
      default:
        fprintf(stderr, "[ERROR] format_step(): "
                "unrecognized format specifier: %%%c\n", *(p + 1));
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

    execute_tmpfile_step(tmpfile_step);
  }

  if (format_step(cmd_buf,
                  stp->stdout_cmd_fmt,
                  stp) == SUCCESS) {
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

    execute_tmpfile_step(tmpfile_step);
  }

  stp->tmpfile = make_tmpfile();

  if (format_step(cmd_buf, stp->tmpfile_cmd_fmt, stp) == SUCCESS) {
    printf("[DEBUG]: about to execute: %s\n", cmd_buf);
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
