#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "table.h"



char *
make_temporary_file(char *template, int suffix_len, char *contents) {

  char buf[PATH_MAX];

  strcpy(buf, template);

  int fd = mkstemps(buf, suffix_len);

  if (fd != -1) {
    FILE *f = fdopen(fd, "w");

    if (NULL == f) {
      perror("fdopen failed");
      return NULL;
    } else {
      fputs(contents, f);
      if (fclose(f) == EOF) {
        perror("failed to close temp file");
        return NULL;
      }
      return strdup(buf);
    }

  } else {
    perror("mkstemp failed");
    return NULL;
  }
}

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
  fail_unless(strcmp(col->type, "text") == 0, "type is not \"text\"");
  fail_unless(col->next == NULL, "next is not NULL");
}
END_TEST

START_TEST(test_read_column_with_type) {

  column *col = read_column("baz{int}");

  fail_unless(strcmp(col->name, "baz") == 0, "name is not \"baz\"");
  fail_unless(strcmp(col->type, "int") == 0, "type is not \"int\"");
  fail_unless(col->next == NULL, "next is not NULL");
}
END_TEST

START_TEST(test_read_column_invalid_name) {

  column *col = read_column("$$$");

  fail_unless(col == NULL, "col is not NULL");
}
END_TEST

START_TEST(test_read_columns) {

  char *path = make_temporary_file("/tmp/check_table.XXXXXX.tsv",
                                   4,
                                   "foo\tbar\none\ttwo");
  column *cols = read_columns(path);

  fail_unless(cols != NULL, "cols is NULL");
  fail_unless(strcmp(cols->name, "foo") == 0, "cols->name is not \"foo\"");
  fail_unless(strcmp(cols->type, "text") == 0, "cols->text is not \"text\"");
  fail_unless(cols->next != NULL, "cols->next is NULL");
  fail_unless(strcmp(cols->next->name, "bar") == 0,
              "cols->name is not \"bar\"");
  fail_unless(strcmp(cols->next->type, "text") == 0,
              "cols->text is not \"text\"");
  fail_unless(cols->next->next == NULL, "cols->next->next is not NULL");
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
  TCase *tc7 = tcase_create("case: read_columns");

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

  tcase_add_test(tc7, test_read_columns);
  suite_add_tcase(ste, tc7);

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
