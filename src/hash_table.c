#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "hash_table.h"
#include "linked_list.h"

#define HASH_TABLE_ARRAY_SIZE ((size_t)17)

typedef struct entry entry_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[HASH_TABLE_ARRAY_SIZE];
  size_t size;
};

ioopm_hash_table_t *ioopm_hash_table_create(void)
{
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
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

static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));

  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;

  return new_entry;
}

static entry_t *find_previous_entry_for_key(entry_t *bucket, int key)
{
  entry_t *last_checked = bucket;
  while (last_checked->next != NULL)
  {
    if (last_checked->next->key >= key)
    {
      break;
    }
    
    last_checked = last_checked->next;
  }
  return last_checked;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  // Make sure key is not negative.
  assert(key >= 0);

  int bucket = key % HASH_TABLE_ARRAY_SIZE;

  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && next->key == key)
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
      ht->size += 1;
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result)
{
  if (key < 0)
  {
    return false;
  }

  int ht_index = key % HASH_TABLE_ARRAY_SIZE;
  entry_t *prev_entry_ptr = find_previous_entry_for_key(&ht->buckets[ht_index], key);

  if (prev_entry_ptr->next == NULL)
  {
    return false;
  }
  else
  {
    *result = prev_entry_ptr->next->value;
    return true;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key)
{
  int bucket = key % HASH_TABLE_ARRAY_SIZE;
  entry_t *prev_entry_ptr = find_previous_entry_for_key(&ht->buckets[bucket], key);
  entry_t *to_be_removed = prev_entry_ptr->next;

  if (to_be_removed == NULL)
  {
    return false;
  }
  else
  {
    entry_t *final_next = to_be_removed->next;
    prev_entry_ptr->next = final_next;
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
      ioopm_linked_list_append(key_list, int_elem(current->key));
      current = current->next;
    }
  }

  return key_list;
}

char **ioopm_hash_table_values(const ioopm_hash_table_t *ht)
{
  char **values = calloc(ioopm_hash_table_size(ht), sizeof(char *));
  int value_counter = 0;

  const entry_t *buckets = ht->buckets;
  entry_t *current = NULL;
  for (int i = 0; i < HASH_TABLE_ARRAY_SIZE; i++)
  {
    current = buckets[i].next;
    while (current != NULL)
    {
      values[value_counter] = current->value;
      value_counter++;

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
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, int key)
{
  char *temp = NULL;
  return ioopm_hash_table_lookup(ht, key, &temp);
}

// Using ioopm_hash_table_any here would
// degrade performance at a small benefit and add coupling.
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, const char *value)
{
  char **ht_values = ioopm_hash_table_values(ht);
  size_t size = ioopm_hash_table_size(ht);

  for (int i = 0; i < size; i++)
  {
    // strcmp has weird return values, 0 means equal.
    if (strcmp(ht_values[i], value) == 0)
    {
      free(ht_values);
      return true;
    }
  }

  free(ht_values);
  return false;
}

bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_ht_predicate *pred, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  char **values = ioopm_hash_table_values(ht);
  bool result = true;
  for (int i = 0; i < size && result; ++i)
  {
    // TODO perf with iterator
    int key = ioopm_linked_list_get(keys, i).int_v;
    result = result && pred(key, values[i], arg);
  }

  ioopm_linked_list_destroy(keys);
  free(values);

  return result;
}

bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_ht_predicate *pred, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  char **values = ioopm_hash_table_values(ht);
  bool result = false;
  for (int i = 0; i < size && result == false; ++i)
  {
    // TODO perf with iterator
    int key = ioopm_linked_list_get(keys, i).int_v;
    result = pred(key, values[i], arg);
  }

  ioopm_linked_list_destroy(keys);
  free(values);

  return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_ht_apply_function *apply_fun, void *arg)
{
  size_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  char **values = ioopm_hash_table_values(ht);
  for (int i = 0; i < size; ++i)
  {
    // TODO perf with iterator
    int key = ioopm_linked_list_get(keys, i).int_v;
    apply_fun(key, &values[i], arg);
  }

  ioopm_linked_list_destroy(keys);
  free(values);
}
