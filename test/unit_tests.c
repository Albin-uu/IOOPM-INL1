#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../src/hash_table.h"
#include "../src/linked_list.h"

#define Free(ptr) {free(ptr); ptr = NULL; }

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


// Start of hash_table function tests.

void test_ht_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_ht_insert_once() {
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

void test_ht_lookup_empty()
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

void test_ht_remove()
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

void test_ht_size()
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

void test_ht_empty()
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

void test_ht_clear()
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

void test_ht_keys()
{
  int keys[5] = {5, 0, 22, 511, 32};
  bool found[5] = { false };

  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *temp = "somevalue";
  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], temp);
  }

  int *gotten_keys = ioopm_hash_table_keys(ht);
  bool found_in_keys = false;

  for (int i = 0; i < 5; i++)
  {
    found_in_keys = false;

    for (int j = 0; j < 5; j++)
    {
      if (gotten_keys[i] == keys[j])
      {
        found[j] = true;
        found_in_keys = true;
        break;
      }
    }

    if (!found_in_keys) { CU_FAIL("Found a key that was never inserted!"); }
  }

  // Validate that all keys were found.
  for (int i = 0; i < 5; i++) { CU_ASSERT_TRUE(found[i]); }

  Free(gotten_keys);
  ioopm_hash_table_destroy(ht);
}

void test_ht_values()
{
  char *values[5] = {"val1", "val2", "val3", "val4", "val5"};
  int keys[5] = {5, 0, 22, 511, 32};
  bool found[5] = { false };

  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  char **gotten_values = ioopm_hash_table_values(ht);
  bool found_in_values = false;
  for (int i = 0; i < 5; i++)
  {
    found_in_values = false;
    for (int j = 0; j < 5; j++)
    {
      if (gotten_values[i] == values[j])
      {
        found[j] = true;
        found_in_values = true;
        break;
      }
    }

    if (!found_in_values) { CU_FAIL("Found a key that was never inserted!"); }
  }

  // Validate that all keys were found.
  for (int i = 0; i < 5; i++) { CU_ASSERT_TRUE(found[i]); }

  Free(gotten_values);
  ioopm_hash_table_destroy(ht);
}

void test_ht_keys_values_match()
{
  int keys[5] = {3, 10, 42, 0, 99};
  char *values[5] = {"three", "ten", "fortytwo", "zero", "ninetynine"};

  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  int *gotten_keys = ioopm_hash_table_keys(ht);
  char **gotten_values = ioopm_hash_table_values(ht);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      if (gotten_keys[i] == keys[j])
      {
        CU_ASSERT_STRING_EQUAL(gotten_values[i], values[j]);
        break;
      }
    }
  }

  Free(gotten_keys);
  Free(gotten_values);
  ioopm_hash_table_destroy(ht);
}

void test_ht_has_key()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
 
  ioopm_hash_table_insert(ht, 42, "somevalue");
  ioopm_hash_table_insert(ht, 525, "somevalue");

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, 42));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, 525));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, 123));

  ioopm_hash_table_destroy(ht);
}

void test_ht_has_value()
{
  // Note C lang does not specify if two equal string literals are stored
  // in the same place in memory. Need to check properly.
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char char_arr[] = "char array";
  char *literal1 = "literal";
  // Remeber to free anything from strdup.
  char *literal2 = strdup(literal1);
 
  ioopm_hash_table_insert(ht, 42, literal1);
  ioopm_hash_table_insert(ht, 5, char_arr);

  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, literal1));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, literal2));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, char_arr));

  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, "not inserted"));

  Free(literal2);
  ioopm_hash_table_destroy(ht);
}

static bool matches_string(int key, char *value, char *match_for)
{
  return strcmp(value, match_for) == 0;
}

void test_ht_valid_for_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 42, "value to match");
  ioopm_hash_table_insert(ht, 311, "value to match");
  ioopm_hash_table_insert(ht, 8, "value to match");
  ioopm_hash_table_insert(ht, 241, "other value");
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_remove(ht, 241);
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_destroy(ht);
}

void test_ht_valid_for_any()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 42, "value1");
  ioopm_hash_table_insert(ht, 311, "value2");
  ioopm_hash_table_insert(ht, 8, "value3");
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_insert(ht, 241, "value to match");
  CU_ASSERT_TRUE(ioopm_hash_table_any(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_destroy(ht);
}

typedef struct
{
  int count;
  char *compare;
} occur_struct;

static void count_occurences(int key, char **value, void *arg)
{
  occur_struct *stc = arg;
  if (strcmp(*value, stc->compare) == 0)
  {
    stc->count++;
  }
}

void test_ht_apply_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  
  occur_struct occ_struct = { .count = 0, .compare = "comparevalue" };

  ioopm_hash_table_insert(ht, 42, "comparevalue");
  ioopm_hash_table_insert(ht, 12, "comparevalue");
  ioopm_hash_table_insert(ht, 55, "other");
  ioopm_hash_table_insert(ht, 822, "comparevalue");
  ioopm_hash_table_insert(ht, 5, "something else");

  ioopm_hash_table_apply_to_all(ht, (ioopm_apply_function *) count_occurences, &occ_struct);
  CU_ASSERT_EQUAL(occ_struct.count, 3);

  ioopm_hash_table_destroy(ht);
}


// Start of linked_list function tests.

/*
Test boilerplate

  ioopm_list_t *lst = ioopm_linked_list_create();



  ioopm_linked_list_destroy(lst);
*/

void test_list_create_destroy()
{
  ioopm_list_t *lst = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(lst);
  ioopm_linked_list_destroy(lst);
}

void test_list_prepend()
{
  ioopm_list_t *lst = ioopm_linked_list_create();

  ioopm_linked_list_prepend(lst, 5);
  ioopm_linked_list_prepend(lst, 6);
  ioopm_linked_list_prepend(lst, 7);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0), 7);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1), 6);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2), 5);

  ioopm_linked_list_destroy(lst);
}

void test_list_append()
{
  ioopm_list_t *lst = ioopm_linked_list_create();

  ioopm_linked_list_append(lst, 25);
  ioopm_linked_list_append(lst, 41);
  ioopm_linked_list_append(lst, 5);

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0), 25);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1), 41);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2), 5);

  ioopm_linked_list_destroy(lst);
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite hash_table_suite = CU_add_suite("Hash table", init_suite, clean_suite);
  CU_pSuite linked_list_suite = CU_add_suite("Linked list", init_suite, clean_suite);
  if (hash_table_suite == NULL ||
      linked_list_suite == NULL)
  {
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
    (CU_add_test(hash_table_suite, "create destroy", test_ht_create_destroy) == NULL) ||
    (CU_add_test(hash_table_suite, "insert once, lookup", test_ht_insert_once) == NULL) ||
    (CU_add_test(hash_table_suite, "lookup empty", test_ht_lookup_empty) == NULL) ||
    (CU_add_test(hash_table_suite, "remove", test_ht_remove) == NULL) ||
    (CU_add_test(hash_table_suite, "size", test_ht_size) == NULL) ||
    (CU_add_test(hash_table_suite, "empty", test_ht_empty) == NULL) ||
    (CU_add_test(hash_table_suite, "clear", test_ht_clear) == NULL) ||
    (CU_add_test(hash_table_suite, "keys", test_ht_keys) == NULL) ||
    (CU_add_test(hash_table_suite, "values", test_ht_values) == NULL) ||
    (CU_add_test(hash_table_suite, "keys values index match", test_ht_keys_values_match) == NULL) ||
    (CU_add_test(hash_table_suite, "has key", test_ht_has_key) == NULL) ||
    (CU_add_test(hash_table_suite, "has value", test_ht_has_value) == NULL) ||
    (CU_add_test(hash_table_suite, "valid for all", test_ht_valid_for_all) == NULL) ||
    (CU_add_test(hash_table_suite, "valid for any", test_ht_valid_for_any) == NULL) ||
    (CU_add_test(hash_table_suite, "apply all", test_ht_apply_all) == NULL) ||
    0 ||
    (CU_add_test(linked_list_suite, "create destroy", test_list_create_destroy) == NULL) ||
    (CU_add_test(linked_list_suite, "prepend", test_list_prepend) == NULL) ||
    (CU_add_test(linked_list_suite, "append", test_list_append) == NULL) ||
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
