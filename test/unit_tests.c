#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../src/common.h"
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
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_ht_insert_once() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  elem_t value1 = { .ptr_v = NULL };
  bool success1 = ioopm_hash_table_lookup(ht, int_elem(5), &value1);
  CU_ASSERT_FALSE(success1);
  CU_ASSERT_PTR_NULL(value1.ptr_v);

  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("test1"));
  elem_t value2 = { .ptr_v = NULL };
  bool success2 = ioopm_hash_table_lookup(ht, int_elem(5), &value2);
  CU_ASSERT_TRUE(success2);
  CU_ASSERT_STRING_EQUAL(value2.ptr_v, "test1");

  ioopm_hash_table_destroy(ht);
}

void test_ht_lookup_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  for (int i = 0; i < 18; ++i) // 18 to overflow array modulo which is 17.
    {
      elem_t *result = NULL;
      CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i), result));
      CU_ASSERT_PTR_NULL(result);
    }

  ioopm_hash_table_destroy(ht);
}

void test_ht_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  elem_t empty_elem = { .ptr_v = NULL };
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, int_elem(4), &empty_elem));
  CU_ASSERT_PTR_NULL(empty_elem.ptr_v);


  elem_t temp = { .ptr_v = NULL };
  ioopm_hash_table_insert(ht, int_elem(4), ptr_elem("testvalue"));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(4), &temp));

  bool success2 = ioopm_hash_table_remove(ht, int_elem(4), NULL);
  CU_ASSERT_TRUE(success2);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(4), &temp));

  ioopm_hash_table_destroy(ht);
}

void test_ht_size()
{
  ioopm_hash_table_t *ht_empty = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_empty), 0);
  ioopm_hash_table_destroy(ht_empty);

  ioopm_hash_table_t *ht_single = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  ioopm_hash_table_insert(ht_single, int_elem(2), ptr_elem("somevalue"));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_single), 1);
  ioopm_hash_table_destroy(ht_single);

  ioopm_hash_table_t *ht_multi = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  ioopm_hash_table_insert(ht_multi, int_elem(5), ptr_elem("value1"));
  ioopm_hash_table_insert(ht_multi, int_elem(2455), ptr_elem("value2"));
  ioopm_hash_table_insert(ht_multi, int_elem(51), ptr_elem("value3"));
  ioopm_hash_table_insert(ht_multi, int_elem(3), ptr_elem("value4"));
  ioopm_hash_table_insert(ht_multi, int_elem(81), ptr_elem("value5"));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht_multi), 5);
  ioopm_hash_table_destroy(ht_multi);
}

void test_ht_empty()
{
  ioopm_hash_table_t *ht_empty = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_empty));
  ioopm_hash_table_destroy(ht_empty);

  ioopm_hash_table_t *ht_nonempty = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  ioopm_hash_table_insert(ht_nonempty, int_elem(2), ptr_elem("val1"));
  ioopm_hash_table_insert(ht_nonempty, int_elem(432), ptr_elem("val2"));
  ioopm_hash_table_insert(ht_nonempty, int_elem(52), ptr_elem("val3"));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht_nonempty));
  ioopm_hash_table_destroy(ht_nonempty);
}

void test_ht_clear()
{
  ioopm_hash_table_t *ht_already_empty = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  ioopm_hash_table_clear(ht_already_empty);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_already_empty));
  ioopm_hash_table_destroy(ht_already_empty);


  ioopm_hash_table_t *ht_nonempty = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  ioopm_hash_table_insert(ht_nonempty, int_elem(2), ptr_elem("val1"));
  ioopm_hash_table_insert(ht_nonempty, int_elem(432), ptr_elem("val2"));
  ioopm_hash_table_insert(ht_nonempty, int_elem(52), ptr_elem("val3"));

  ioopm_hash_table_clear(ht_nonempty);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht_nonempty));

  ioopm_hash_table_destroy(ht_nonempty);
}

void test_ht_keys()
{
  int keys[5] = {5, 0, 22, 511, 32};
  bool found[5] = { false };

  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  char *temp = "somevalue";
  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(temp));
  }

  ioopm_list_t *gotten_keys = ioopm_hash_table_keys(ht);
  bool found_in_keys = false;

  for (int i = 0; i < 5; i++)
  {
    found_in_keys = false;

    for (int j = 0; j < 5; j++)
    {
      if (ioopm_linked_list_get(gotten_keys, i).int_v == keys[j])
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

  ioopm_linked_list_destroy(gotten_keys);
  ioopm_hash_table_destroy(ht);
}

void test_ht_values()
{
  char *values[5] = {"val1", "val2", "val3", "val4", "val5"};
  int keys[5] = {5, 0, 22, 511, 32};
  bool found[5] = { false };

  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
  }

  ioopm_list_t *gotten_values = ioopm_hash_table_values(ht);
  bool found_in_values = false;
  for (int i = 0; i < 5; i++)
  {
    found_in_values = false;
    for (int j = 0; j < 5; j++)
    {
      char *gotten_str = ioopm_linked_list_get(gotten_values, i).ptr_v;
      if (strcmp(gotten_str, values[j]) == 0)
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

  ioopm_linked_list_destroy(gotten_values);
  ioopm_hash_table_destroy(ht);
}

void test_ht_keys_values_match()
{
  int keys[5] = {3, 10, 42, 0, 99};
  char *values[5] = {"three", "ten", "fortytwo", "zero", "ninetynine"};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
  }

  ioopm_list_t *gotten_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *gotten_values = ioopm_hash_table_values(ht);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      if (ioopm_linked_list_get(gotten_keys, i).int_v == keys[j])
      {
        char *gotten_str = ioopm_linked_list_get(gotten_values, i).ptr_v;
        CU_ASSERT_STRING_EQUAL(gotten_str, values[j]);
        break;
      }
    }
  }

  ioopm_linked_list_destroy(gotten_keys);
  ioopm_linked_list_destroy(gotten_values);
  ioopm_hash_table_destroy(ht);
}

void test_ht_has_key()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
 
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("somevalue"));
  ioopm_hash_table_insert(ht, int_elem(525), ptr_elem("somevalue"));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(42)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(525)));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(123)));

  ioopm_hash_table_destroy(ht);
}

void test_ht_has_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  char insert_str[] = "expected value";
  char other_value[] = "other value";
  char check_against[] = "expected value";
  char not_inserted[] = "not inserted";
 
  ioopm_hash_table_insert(ht, int_elem(111), ptr_elem(other_value));
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem(insert_str));
  ioopm_hash_table_insert(ht, int_elem(222), ptr_elem(other_value));

  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(insert_str)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(check_against)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem("expected value")));

  // Since key is the same, value will be replaced.
  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem(other_value));
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem(insert_str)));

  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem(not_inserted)));

  ioopm_hash_table_destroy(ht);
}

// Helper pred to for_all, for_any test fns.
static bool matches_string(elem_t key, elem_t value, char *match_for)
{
  return strcmp(value.ptr_v, match_for) == 0;
}

void test_ht_valid_for_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("value to match"));
  ioopm_hash_table_insert(ht, int_elem(311), ptr_elem("value to match"));
  ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("value to match"));
  ioopm_hash_table_insert(ht, int_elem(241), ptr_elem("other value"));
  CU_ASSERT_FALSE(ioopm_hash_table_all(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_remove(ht, int_elem(241), NULL);
  CU_ASSERT_TRUE(ioopm_hash_table_all(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_destroy(ht);
}

void test_ht_valid_for_any()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);

  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("value1"));
  ioopm_hash_table_insert(ht, int_elem(311), ptr_elem("value2"));
  ioopm_hash_table_insert(ht, int_elem(8), ptr_elem("value3"));
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, (ioopm_predicate *) matches_string, "value to match"));

  ioopm_hash_table_insert(ht, int_elem(241), ptr_elem("value to match"));
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
  ioopm_hash_table_t *ht = ioopm_hash_table_create(ioopm_equals_str, NULL, NULL);
  
  occur_struct occ_struct = { .count = 0, .compare = "comparevalue" };

  ioopm_hash_table_insert(ht, int_elem(42), ptr_elem("comparevalue"));
  ioopm_hash_table_insert(ht, int_elem(12), ptr_elem("comparevalue"));
  ioopm_hash_table_insert(ht, int_elem(55), ptr_elem("other"));
  ioopm_hash_table_insert(ht, int_elem(822), ptr_elem("comparevalue"));
  ioopm_hash_table_insert(ht, int_elem(5), ptr_elem("something else"));

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
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);
  CU_ASSERT_PTR_NOT_NULL(lst);
  ioopm_linked_list_destroy(lst);
}

void test_list_prepend()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_prepend(lst, int_elem(5));
  ioopm_linked_list_prepend(lst, int_elem(6));
  ioopm_linked_list_prepend(lst, int_elem(7));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 7);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).int_v, 6);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 5);

  ioopm_linked_list_destroy(lst);
}

void test_list_append()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(25));
  ioopm_linked_list_append(lst, int_elem(41));
  ioopm_linked_list_append(lst, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 25);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).int_v, 41);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 5);

  ioopm_linked_list_destroy(lst);
}

void test_list_insert()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_insert(lst, 0, int_elem(5));
  ioopm_linked_list_insert(lst, 1, int_elem(10));
  ioopm_linked_list_insert(lst, 1, int_elem(15));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 5);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).int_v, 15);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 10);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 3);

  ioopm_linked_list_destroy(lst);
}

void test_list_remove()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(10));
  ioopm_linked_list_append(lst, int_elem(20));
  ioopm_linked_list_append(lst, int_elem(30));
  ioopm_linked_list_append(lst, int_elem(40));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 30);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 4);

  CU_ASSERT_EQUAL(ioopm_linked_list_remove(lst, 2).int_v, 30);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 40);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 3);

  CU_ASSERT_EQUAL(ioopm_linked_list_remove(lst, 0).int_v, 10);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 20);

  ioopm_linked_list_destroy(lst);
}

void test_list_get()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(100));
  ioopm_linked_list_append(lst, int_elem(110));
  ioopm_linked_list_append(lst, int_elem(120));
  ioopm_linked_list_append(lst, int_elem(130));
  ioopm_linked_list_append(lst, int_elem(140));

  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 100);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 120);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 4).int_v, 140);

  ioopm_linked_list_destroy(lst);
}

void test_list_contains()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(10));
  ioopm_linked_list_append(lst, int_elem(20));
  ioopm_linked_list_append(lst, int_elem(30));
  ioopm_linked_list_append(lst, int_elem(40));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(lst, int_elem(30)));

  ioopm_linked_list_remove(lst, 2);
  CU_ASSERT_FALSE(ioopm_linked_list_contains(lst, int_elem(30)));

  ioopm_linked_list_destroy(lst);
}

void test_list_size()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(25));
  ioopm_linked_list_append(lst, int_elem(25));
  ioopm_linked_list_append(lst, int_elem(25));
  ioopm_linked_list_append(lst, int_elem(25));

  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 4);

  ioopm_linked_list_destroy(lst);
}

void test_list_empty()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(lst));
  ioopm_linked_list_append(lst, int_elem(42));
  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(lst));

  ioopm_linked_list_destroy(lst);
}

void test_list_clear()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(1));
  ioopm_linked_list_append(lst, int_elem(2));
  ioopm_linked_list_append(lst, int_elem(3));
  ioopm_linked_list_append(lst, int_elem(4));

  ioopm_linked_list_clear(lst);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 0);

  ioopm_linked_list_append(lst, int_elem(5));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 1);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 5);

  ioopm_linked_list_destroy(lst);
}

// Higher-order function to pred tests.
static bool simple_int_equality(int index, elem_t value, void *extra)
{
  elem_t *comp_elem = (elem_t *) extra;
  return ioopm_equals_int(value, *comp_elem);
}

void test_list_holds_pred_all()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);
  int comp_num = 42;

  ioopm_linked_list_append(lst, int_elem(42));
  ioopm_linked_list_append(lst, int_elem(42));
  ioopm_linked_list_append(lst, int_elem(42));
  ioopm_linked_list_append(lst, int_elem(42));
  ioopm_linked_list_append(lst, int_elem(42));
  // Compare int gets type cast to elem_t.
  CU_ASSERT_TRUE(ioopm_linked_list_all(lst, (ioopm_list_predicate *) simple_int_equality, &comp_num));

  ioopm_linked_list_insert(lst, 3, int_elem(55)); 
  CU_ASSERT_FALSE(ioopm_linked_list_all(lst, (ioopm_list_predicate *) simple_int_equality, &comp_num));

  ioopm_linked_list_destroy(lst);
}

void test_list_holds_pred_any()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);
  int comp_num = 42;

  ioopm_linked_list_append(lst, int_elem(1));
  ioopm_linked_list_append(lst, int_elem(2));
  ioopm_linked_list_append(lst, int_elem(3));
  ioopm_linked_list_append(lst, int_elem(4));
  ioopm_linked_list_append(lst, int_elem(5));
  CU_ASSERT_FALSE(ioopm_linked_list_any(lst, (ioopm_list_predicate *) simple_int_equality, &comp_num));

  ioopm_linked_list_insert(lst, 3, int_elem(42)); 
  CU_ASSERT_TRUE(ioopm_linked_list_any(lst, (ioopm_list_predicate *) simple_int_equality, &comp_num));

  ioopm_linked_list_destroy(lst);
}

// HOF to test in apply_all
static void double_ints(int index, void *value, void *extra)
{
  int *val = (int *) value;
  *val = *val * 2;
}

void test_list_apply_all()
{
  ioopm_list_t *lst = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  ioopm_linked_list_append(lst, int_elem(1));
  ioopm_linked_list_append(lst, int_elem(2));
  ioopm_linked_list_append(lst, int_elem(3));
  ioopm_linked_list_append(lst, int_elem(4));
  ioopm_linked_list_append(lst, int_elem(5));

  ioopm_linked_list_apply_to_all(lst, (ioopm_apply_list_function *) double_ints, NULL);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).int_v, 2);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).int_v, 4);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).int_v, 6);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 3).int_v, 8);
  CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 4).int_v, 10);

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
    (CU_add_test(linked_list_suite, "insert", test_list_insert) == NULL) ||
    (CU_add_test(linked_list_suite, "remove", test_list_remove) == NULL) ||
    (CU_add_test(linked_list_suite, "get", test_list_get) == NULL) ||
    (CU_add_test(linked_list_suite, "contains", test_list_contains) == NULL) ||
    (CU_add_test(linked_list_suite, "size", test_list_size) == NULL) ||
    (CU_add_test(linked_list_suite, "empty", test_list_empty) == NULL) ||
    (CU_add_test(linked_list_suite, "clear", test_list_clear) == NULL) ||
    (CU_add_test(linked_list_suite, "pred all", test_list_holds_pred_all) == NULL) ||
    (CU_add_test(linked_list_suite, "pred any", test_list_holds_pred_any) == NULL) ||
    (CU_add_test(linked_list_suite, "apply all", test_list_apply_all) == NULL) ||
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
