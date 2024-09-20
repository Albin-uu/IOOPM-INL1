#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdbool.h>
#include "../src/hash_table.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.
void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_insert_once() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *value1 = NULL;
  bool success1 = ioopm_hash_table_lookup(ht, 5, &value1);
  CU_ASSERT_FALSE(success1);
  CU_ASSERT_PTR_NULL(value1);

  ioopm_hash_table_insert(ht, 5, "test1");
  char *value2 = NULL;
  bool success2 = ioopm_hash_table_lookup(ht, 5, &value2);
  CU_ASSERT_TRUE(success2);
  CU_ASSERT_STRING_EQUAL(value2, "test1");

  ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (int i = 0; i < 18; ++i) /// 18 is a bit magical
    {
      char *result = NULL;
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, i, &result));
      CU_ASSERT_PTR_NULL(result);
    }
  char *negative_result = NULL;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, -1, &negative_result));
  CU_ASSERT_PTR_NULL(negative_result);

  ioopm_hash_table_destroy(ht);
}

void test_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *no_elem = NULL;
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, 4));
  CU_ASSERT_PTR_NULL(no_elem);


  char *temp = NULL;
  ioopm_hash_table_insert(ht, 4, "testvalue");
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, 4, &temp));

  bool success2 = ioopm_hash_table_remove(ht, 4);
  CU_ASSERT_TRUE(success2);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, 4, &temp));

  ioopm_hash_table_destroy(ht);
}

void test_size()
{
  ioopm_hash_table_t *ht_empty = ioopm_hash_table_create();
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_empty), 0);
  ioopm_hash_table_destroy(ht_empty);

  ioopm_hash_table_t *ht_single = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht_single, 2, "somevalue");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_single), 1);
  ioopm_hash_table_destroy(ht_single);

  ioopm_hash_table_t *ht_multi = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht_multi, 5, "value1");
  ioopm_hash_table_insert(ht_multi, 2455, "value2");
  ioopm_hash_table_insert(ht_multi, 51, "value3");
  ioopm_hash_table_insert(ht_multi, 3, "value4");
  ioopm_hash_table_insert(ht_multi, 81, "value5");
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_multi), 5);
  ioopm_hash_table_destroy(ht_multi);
}

void test_empty()
{
  ioopm_hash_table_t *ht_empty = ioopm_hash_table_create();
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_empty));
  ioopm_hash_table_destroy(ht_empty);

  ioopm_hash_table_t *ht_nonempty = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht_nonempty, 2, "val1");
  ioopm_hash_table_insert(ht_nonempty, 432, "val2");
  ioopm_hash_table_insert(ht_nonempty, 52, "val3");
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_nonempty));
  ioopm_hash_table_destroy(ht_nonempty);
}

void test_clear()
{
  ioopm_hash_table_t *ht_already_empty = ioopm_hash_table_create();
  ioopm_hash_table_clear(ht_already_empty);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_already_empty));
  ioopm_hash_table_destroy(ht_already_empty);


  ioopm_hash_table_t *ht_nonempty = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht_nonempty, 2, "val1");
  ioopm_hash_table_insert(ht_nonempty, 432, "val2");
  ioopm_hash_table_insert(ht_nonempty, 52, "val3");

  ioopm_hash_table_clear(ht_nonempty);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_nonempty));

  ioopm_hash_table_destroy(ht_nonempty);
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("Hash table", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "create destroy", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "insert once, lookup", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "lookup empty", test_lookup_empty) == NULL) ||
    (CU_add_test(my_test_suite, "remove", test_remove) == NULL) ||
    (CU_add_test(my_test_suite, "size", test_size) == NULL) ||
    (CU_add_test(my_test_suite, "empty", test_empty) == NULL) ||
    (CU_add_test(my_test_suite, "clear", test_clear) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
