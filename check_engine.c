#include <check.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include "engine.h"
#include "def.h"

START_TEST(test_format_step_too_few_specifiers) {

  step tmp_stp;

  tmp_stp.stdout_cmd_fmt = "";
  tmp_stp.tmpfile_cmd_fmt = "";
  tmp_stp.tmpfile = "/tmp/foo";
  tmp_stp.stdin_prerequisite = NULL;
  tmp_stp.tmpfile_prerequisites = NULL;
  tmp_stp.next = NULL;

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = NULL;
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = &tmp_stp;
  stp.next = NULL;

  char buf[2000];

  int retval = format_step(buf, "foo", &stp);

  fail_unless(retval == FAILURE,
              "did not return FAILURE when more tmpfiles than %%i");
}
END_TEST

START_TEST(test_format_step_too_many_specifiers) {

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = "/tmp/foo";
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = format_step(buf, "foo %i %i", &stp);

  fail_unless(retval == FAILURE,
              "did not return FAILURE when fewer tmpfiles than %%i");
}
END_TEST

START_TEST(test_format_step) {

  step tmp_stp;

  tmp_stp.stdout_cmd_fmt = "";
  tmp_stp.tmpfile_cmd_fmt = "";
  tmp_stp.tmpfile = "/tmp/foo";
  tmp_stp.stdin_prerequisite = NULL;
  tmp_stp.tmpfile_prerequisites = NULL;
  tmp_stp.next = NULL;

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = NULL;
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = &tmp_stp;
  stp.next = NULL;

  char buf[2000];

  int retval = format_step(buf, "foo %i bar", &stp);

  fail_unless(retval == 0, "did not return 0");
  fail_unless(strcmp(buf, "foo /tmp/foo bar") == 0,
              "did not set buf to 'foo /tmp/foo bar'");
}
END_TEST


START_TEST(test_execute_stdout_step) {

  step stp;

  stp.stdout_cmd_fmt = "ls";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = NULL;
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = execute_stdout_step(&stp);

  fail_unless(retval == SUCCESS, "did not return SUCCESS");
}
END_TEST

START_TEST(test_execute_stdout_step_pipe) {

  step pre_stp;

  pre_stp.stdout_cmd_fmt = "ls";
  pre_stp.tmpfile_cmd_fmt = "";
  pre_stp.tmpfile = NULL;
  pre_stp.stdin_prerequisite = NULL;
  pre_stp.tmpfile_prerequisites = NULL;
  pre_stp.next = NULL;

  step stp;

  stp.stdout_cmd_fmt = "sort";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = NULL;
  stp.stdin_prerequisite = &pre_stp;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = execute_stdout_step(&stp);

  fail_unless(retval == SUCCESS, "did not return SUCCESS");
}
END_TEST

START_TEST(test_execute_stdout_step_tmpfile) {

  step pre_stp;

  pre_stp.stdout_cmd_fmt = "";
  pre_stp.tmpfile_cmd_fmt = "ls > %o";
  pre_stp.tmpfile = NULL;
  pre_stp.stdin_prerequisite = NULL;
  pre_stp.tmpfile_prerequisites = NULL;
  pre_stp.next = NULL;

  step stp;

  stp.stdout_cmd_fmt = "head -1 %i";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = NULL;
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = &pre_stp;
  stp.next = NULL;

  char buf[2000];

  int retval = execute_stdout_step(&stp);

  fail_unless(retval == SUCCESS, "did not return SUCCESS");
}
END_TEST

Suite *
suite_engine(void) {
  Suite *ste = suite_create("suite: engine");
  TCase *tc1 = tcase_create("case: format_step_too_few_specifiers");
  TCase *tc2 = tcase_create("case: format_step_too_many_specifiers");
  TCase *tc3 = tcase_create("case: format_step");
  TCase *tc4 = tcase_create("case: execute_stdout_step");
  TCase *tc5 = tcase_create("case: execute_stdout_step_pipe");
  TCase *tc6 = tcase_create("case: execute_stdout_step_tmpfile");

  tcase_add_test(tc1, test_format_step_too_few_specifiers);
  suite_add_tcase(ste, tc1);

  tcase_add_test(tc2, test_format_step_too_many_specifiers);
  suite_add_tcase(ste, tc2);

  tcase_add_test(tc3, test_format_step);
  suite_add_tcase(ste, tc3);

  tcase_add_test(tc4, test_execute_stdout_step);
  suite_add_tcase(ste, tc4);

  tcase_add_test(tc5, test_execute_stdout_step_pipe);
  suite_add_tcase(ste, tc5);

  tcase_add_test(tc6, test_execute_stdout_step_tmpfile);
  suite_add_tcase(ste, tc6);

  return ste;
}

int
main(void) {
  int number_failed;
  Suite *ste = suite_engine();
  SRunner *sr = srunner_create(ste);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}
