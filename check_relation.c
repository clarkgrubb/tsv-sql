#include <check.h>
#include <string.h>

#include "relation.h"
#include "def.h"

START_TEST(test_dummy) {

  fail_unless(1 == 1,
              "dummy test failed %%i");
}
END_TEST

Suite *
suite_relation(void) {
  Suite *ste = suite_create("suite: relation");
  TCase *tc1 = tcase_create("case: dummy");

  tcase_add_test(tc1, test_dummy);
  suite_add_tcase(ste, tc1);

  return ste;
}

int
main(void) {
  int number_failed;
  Suite *ste = suite_relation();
  SRunner *sr = srunner_create(ste);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}
