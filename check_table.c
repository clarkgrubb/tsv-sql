#include <check.h>
#include <string.h>

#include "table.h"

START_TEST(test_path_to_name) {

  char *name = path_to_name("bar.tsv");

  fail_unless(strcmp(name, "bar") == 0, "name is not \"bar\"");
}
END_TEST

START_TEST(test_path_to_name_full) {

  char *name = path_to_name("/tmp/baz.tsv");

  fail_unless(strcmp(name, "baz") == 0, "name is not \"baz\"");
}
END_TEST

START_TEST(test_path_to_name_bad_suffix) {

  char *name = path_to_name("/tmp/wumpus.txt");

  fail_unless(name == NULL, "name is not NULL");
}
END_TEST

START_TEST(test_read_column) {

  column *col = read_column("foo");

  fail_unless(strcmp(col->name, "foo") == 0, "name is not \"foo\"");
}
END_TEST

START_TEST(test_read_column_with_type) {

  column *col = read_column("baz{int}");

  fail_unless(strcmp(col->type, "int") == 0, "type is not \"int\"");
}
END_TEST

START_TEST(test_read_column_invalid_name) {

  column *col = read_column("$$$");

  fail_unless(col == NULL, "col is not NULL");
}
END_TEST


Suite *
suite_table(void) {
  Suite *ste = suite_create("suite: table");
  TCase *tc1 = tcase_create("case: path_to_name");
  TCase *tc2 = tcase_create("case: path_to_name full");
  TCase *tc3 = tcase_create("case: path_to_name bad suffix");
  TCase *tc4 = tcase_create("case: read_column");
  TCase *tc5 = tcase_create("case: read_column with type");
  TCase *tc6 = tcase_create("case: read_column invalid name");

  tcase_add_test(tc1, test_path_to_name);
  suite_add_tcase(ste, tc1);

  tcase_add_test(tc2, test_path_to_name_full);
  suite_add_tcase(ste, tc2);

  tcase_add_test(tc3, test_path_to_name_bad_suffix);
  suite_add_tcase(ste, tc3);

  tcase_add_test(tc4, test_read_column);
  suite_add_tcase(ste, tc4);

  tcase_add_test(tc5, test_read_column_with_type);
  suite_add_tcase(ste, tc5);

  tcase_add_test(tc6, test_read_column_invalid_name);
  suite_add_tcase(ste, tc6);

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
