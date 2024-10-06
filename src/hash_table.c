#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "hash_table.h"
#include "linked_list.h"

#define HASH_TABLE_ARRAY_SIZE ((size_t)257)

typedef struct entry entry_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  ioopm_eq_function *value_eq_fn;
  ioopm_eq_function *key_eq_fn;
  ioopm_hash_function *hash_fn;
  entry_t buckets[HASH_TABLE_ARRAY_SIZE];
  size_t size;
};

// Helper as default int hash function.
static int extract_int_hash_key(elem_t key)
{ return key.int_v; }

ioopm_hash_table_t *ioopm_hash_table_create(
  ioopm_eq_function *value_eq_fn,
  ioopm_eq_function *key_eq_fn,
  ioopm_hash_function *hash_fn)
{
  // If arguments are omited assume all keys and values will be int's.
  if (value_eq_fn == NULL) { value_eq_fn = ioopm_equals_int; }
  if (key_eq_fn == NULL) { key_eq_fn = ioopm_equals_int; }
  // If hash function is omitted, then keys will be ints and the raw key
  // will serve as the hashed value.
  if (hash_fn == NULL) { hash_fn = extract_int_hash_key; }

  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->value_eq_fn = value_eq_fn;
  ht->key_eq_fn = key_eq_fn;
  ht->hash_fn = hash_fn;
  ht->size = 0;
  return ht;
}

static void entry_destroy(entry_t * entry_ptr)
{
  free(entry_ptr);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  free(ht);
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));

  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;

  return new_entry;
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *bucket, elem_t key)
{
  entry_t *last_checked = bucket;
  while (last_checked->next != NULL)
  {
    if (ht->key_eq_fn(last_checked->next->key, key))
    {
      break;
    }
    
    last_checked = last_checked->next;
  }
  return last_checked;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  int hashed_key = ht->hash_fn(key);
  int bucket = hashed_key % HASH_TABLE_ARRAY_SIZE;

  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && ht->key_eq_fn(next->key, key))
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
      ht->size += 1;
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  int hashed_key = ht->hash_fn(key);
  int ht_index = hashed_key % HASH_TABLE_ARRAY_SIZE;
  entry_t *prev_entry_ptr = find_previous_entry_for_key(ht, &ht->buckets[ht_index], key);

  if (prev_entry_ptr->next == NULL)
  {
    return false;
  }
  else
  {
    // Result pointer is optional to allow checking only if a value exists with key.
    if (result != NULL) { *result = prev_entry_ptr->next->value; }
    return true;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *send_back)
{
  int hashed_key = ht->hash_fn(key);
  int bucket = hashed_key % HASH_TABLE_ARRAY_SIZE;
  entry_t *prev_entry_ptr = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *to_be_removed = prev_entry_ptr->next;

  if (to_be_removed == NULL)
  {
    return false;
  }
  else
  {
    entry_t *final_next = to_be_removed->next;
    prev_entry_ptr->next = final_next;

    if (send_back != NULL) { *send_back = to_be_removed->value; }
    free(to_be_removed);

    ht->size -= 1;
    return true;
  }
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  entry_t *sentinel = NULL;
  entry_t *current_entry = NULL;
  entry_t *next_entry = NULL;

  for (int i = 0; i < HASH_TABLE_ARRAY_SIZE; i++)
  {
    sentinel = &ht->buckets[i];
    current_entry = sentinel;
    next_entry = current_entry->next;
    
    if (next_entry == NULL)
    {
      continue;
    }
    else
    {
      // Skip trying to deallocate the sentinel.
      do
      {
        current_entry = next_entry;
        next_entry = next_entry->next;
        entry_destroy(current_entry);
      }
      while (next_entry != NULL);
    }

    // Reset sentinel in case the hash table will be used further.
    sentinel->next = NULL; 
  }

  ht->size = 0;
}

ioopm_list_t *ioopm_hash_table_keys(const ioopm_hash_table_t *ht)
{
  ioopm_list_t *key_list = ioopm_linked_list_create((ioopm_eq_function *) ioopm_equals_int);

  const entry_t *buckets = ht->buckets;
  entry_t *current = NULL;
  for (int i = 0; i < HASH_TABLE_ARRAY_SIZE; i++)
  {
    current = buckets[i].next;
    while (current != NULL)
    {
      ioopm_linked_list_append(key_list, current->key);
      current = current->next;
    }
  }

  return key_list;
}

ioopm_list_t *ioopm_hash_table_values(const ioopm_hash_table_t *ht)
{
  ioopm_list_t *values = ioopm_linked_list_create(ht->value_eq_fn);

  const entry_t *buckets = ht->buckets;
  entry_t *current = NULL;
  for (int i = 0; i < HASH_TABLE_ARRAY_SIZE; i++)
  {
    current = buckets[i].next;
    while (current != NULL)
    {
      ioopm_linked_list_append(values, current->value);
      current = current->next;
    }
  }

  return values;
}

/*static bool key_equiv(int key, char *value_ignored, void *x)
{
  int *other_key_ptr = x;
  int other_key = *other_key_ptr;
  return key == other_key;
}*/

// Using ioopm_hash_table_any here would massively
// degrade performance at minimal benefit.
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  elem_t temp;
  return ioopm_hash_table_lookup(ht, key, &temp);
}

// TODO REVISE PERF NOTE
// Using ioopm_hash_table_any here would
// degrade performance at a small benefit and add coupling.
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, const elem_t value)
{
  ioopm_list_t *ht_values = ioopm_hash_table_values(ht);
  size_t size = ioopm_linked_list_size(ht_values);

  for (int i = 0; i < size; i++)
  {
    // TODO PERF
    elem_t ht_val = ioopm_linked_list_get(ht_values, i);
    if (ht->value_eq_fn(ht_val, value))
    {
      ioopm_linked_list_destroy(ht_values);
      return true;
    }
  }

  ioopm_linked_list_destroy(ht_values);
  return false;
}

bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result = true;
  for (int i = 0; i < size && result; ++i)
  {
    // TODO perf with iterator
    elem_t key = ioopm_linked_list_get(keys, i);
    elem_t val = ioopm_linked_list_get(values, i);
    result = result && pred(key, val, arg);
  }

  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);

  return result;
}

bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result = false;
  for (int i = 0; i < size && result == false; ++i)
  {
    // TODO perf with iterator
    elem_t key = ioopm_linked_list_get(keys, i);
    elem_t val = ioopm_linked_list_get(values, i);
    result = pred(key, val, arg);
  }

  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);

  return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  for (int i = 0; i < size; ++i)
  {
    // TODO perf with iterator
    elem_t key = ioopm_linked_list_get(keys, i);
    elem_t val = ioopm_linked_list_get(values, i);
    apply_fun(key, &val, arg);
  }

  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
}
