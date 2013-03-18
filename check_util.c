#include <check.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

START_TEST(test_trim) {

  char *s = strdup(" foo ");

  fail_unless(strlen(s) == 5, "strlen(s) not 5");

  trim(s);

  fail_unless(strlen(s) == 3, "strlen(s) not 3");

  fail_unless(strcmp(s, "foo") == 0, "s is not \"foo\"");
}
END_TEST

START_TEST(test_trim_empty) {

  char *s = strdup("");

  fail_unless(strlen(s) == 0, "strlen(s) not 0");

  trim(s);

  fail_unless(strlen(s) == 0, "strlen(s) not 0");

  fail_unless(strcmp(s, "") == 0, "s is not \"\"");
}
END_TEST

START_TEST(test_trim_whitesp) {

  char *s = strdup("   ");

  fail_unless(strlen(s) == 3, "strlen(s) not 3");

  trim(s);

  fail_unless(strlen(s) == 0, "strlen(s) not 0");

  fail_unless(strcmp(s, "") == 0, "s is not \"\"");
}
END_TEST

START_TEST(test_split) {

  char **parts = split("one|two|three", '|');

  fail_unless(strcmp(parts[0], "one") == 0, "parts[0] is not \"one\"");
  fail_unless(strcmp(parts[1], "two") == 0, "parts[1] is not \"two\"");
  fail_unless(strcmp(parts[2], "three") == 0, "parts[2] is not \"three\"");
  fail_unless(parts[3] == NULL, "parts[3] is not NULL");

}
END_TEST

START_TEST(test_split_no_delim) {

  char **parts = split("one", '|');

  fail_unless(strcmp(parts[0], "one") == 0, "parts[0] is not \"one\"");
  fail_unless(parts[1] == NULL, "parts[1] is not NULL");

}
END_TEST

START_TEST(test_split_empty) {

  char **parts = split("", '|');

  fail_unless(parts[0] == NULL, "parts[0] is not NULL");

}
END_TEST

START_TEST(test_make_tmpfile) {

  char *tmpfile = make_tmpfile();

  fail_unless(strncasecmp(tmpfile, "/tmp/tsv-sql.", 13) == 0,
              "beginning of tmpfile is not /tmp/tsv-sql.");

}
END_TEST


Suite *
suite_util(void) {
  Suite *ste = suite_create("suite: util");
  TCase *tc1 = tcase_create("case: trim");
  TCase *tc2 = tcase_create("case: trim empty");
  TCase *tc3 = tcase_create("case: trim whitespace");
  TCase *tc4 = tcase_create("case: split");
  TCase *tc5 = tcase_create("case: split no delim");
  TCase *tc6 = tcase_create("case: split empty");
  TCase *tc7 = tcase_create("case: make_tmpfile");

  tcase_add_test(tc1, test_trim);
  suite_add_tcase(ste, tc1);

  tcase_add_test(tc2, test_trim_empty);
  suite_add_tcase(ste, tc2);

  tcase_add_test(tc3, test_trim_whitesp);
  suite_add_tcase(ste, tc3);

  tcase_add_test(tc4, test_split);
  suite_add_tcase(ste, tc4);

  tcase_add_test(tc5, test_split_no_delim);
  suite_add_tcase(ste, tc5);

  tcase_add_test(tc6, test_split_empty);
  suite_add_tcase(ste, tc6);

  tcase_add_test(tc7, test_make_tmpfile);
  suite_add_tcase(ste, tc7);

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
