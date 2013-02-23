#include <check.h>

#include "table.h"

START_TEST(test_trim) {

  char *s = strdup(" foo ");

  fail_unless(strlen(s) == 5, "strlen(s) not 5");

  trim(s);

  fail_unless(strlen(s) == 3, "strlen(s) not 3");

  fail_unless(strcmp(s, "foo") == 0, "s is not \"foo\"");
}
END_TEST

Suite *
suite_table(void) {
  Suite *ste = suite_create("suite: table");
  TCase *tc = tcase_create("case: trim");

  tcase_add_test(tc, test_trim);
  suite_add_tcase(ste, tc);

  return ste;
}

int
main(void) {
  int number_failed;
  Suite *ste = suite_table();
  SRunner *sr = srunner_create(ste);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}
