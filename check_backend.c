#include <check.h>
#include <string.h>

#include "backend.h"
#include "def.h"

START_TEST(test_substitute_tmpfiles_too_few_specifiers) {

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = "/tmp/foo";
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = substitute_tmpfiles(buf, "foo", &stp);

  fail_unless(retval == FAILURE,
              "did not return FAILURE when more tmpfiles than %s");
}
END_TEST

START_TEST(test_substitute_tmpfiles_too_many_specifiers) {

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = "/tmp/foo";
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = substitute_tmpfiles(buf, "foo %s %s", &stp);

  fail_unless(retval == FAILURE,
              "did not return FAILURE when fewer tmpfiles than %s");
}
END_TEST

START_TEST(test_substitute_tmpfiles) {

  step stp;

  stp.stdout_cmd_fmt = "";
  stp.tmpfile_cmd_fmt = "";
  stp.tmpfile = "/tmp/foo";
  stp.stdin_prerequisite = NULL;
  stp.tmpfile_prerequisites = NULL;
  stp.next = NULL;

  char buf[2000];

  int retval = substitute_tmpfiles(buf, "foo %s bar", &stp);

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

Suite *
suite_util(void) {
  Suite *ste = suite_create("suite: util");
  TCase *tc1 = tcase_create("case: substitute_tmpfiles_too_few_specifiers");
  TCase *tc2 = tcase_create("case: substitute_tmpfiles_too_many_specifiers");
  TCase *tc3 = tcase_create("case: substitute_tmpfiles");
  TCase *tc4 = tcase_create("case: execute_stdout_step");

  tcase_add_test(tc1, test_substitute_tmpfiles_too_few_specifiers);
  suite_add_tcase(ste, tc1);

  tcase_add_test(tc2, test_substitute_tmpfiles_too_many_specifiers);
  suite_add_tcase(ste, tc2);

  tcase_add_test(tc3, test_substitute_tmpfiles);
  suite_add_tcase(ste, tc3);

  tcase_add_test(tc4, test_execute_stdout_step);
  suite_add_tcase(ste, tc4);

  return ste;
}

int
main(void) {
  int number_failed;
  Suite *ste = suite_util();
  SRunner *sr = srunner_create(ste);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}
