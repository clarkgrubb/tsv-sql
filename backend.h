#ifndef BACKEND_H_
#define BACKEND_H_

/*
 *    stdout_cmd_fmt
 *      version of the cmd used when writing to stdout.  It is
 *      a sprintf style string with %s format specifiers, one for
 *      each step in the tmpfile_prerequisities list and in the
 *      same order.  Once the cmd has been processed by sprintf
 *      it can be given to a system() for execution.
 *
 *    tmpfile_cmd_file
 *      version of the cmd used when the output of the command is
 *      to be written to a temp file.  It is a sprintf sytle string
 *      with %s format specifiers for each of the tmpfile_prerequisites,
 *      plus an extra %s specifier at the end for output temp file.
 *
 *    stdin_prerequisite
 *      the step which should be connected via a pipeline to this
 *      step.  This is done by concatenating the command for the
 *      prerequisite, "|", and the command for this step together
 *      and calling system() on the result.
 *
 *    tmpfile_prerequisites
 *      steps which create the tempfiles needed for executing
 *      stdout_cmd_fmt or tmpfile_cmd_fmt.
 *
 */
typedef struct step {
  char *stdout_cmd_fmt;
  char *tmpfile_cmd_fmt;
  char *tmpfile;
  struct step *stdin_prerequisite;
  struct step *tmpfile_prerequisites;
  struct step *next;
} step;

int
format_step(char *buf, char *fmt, step *step);

#endif
