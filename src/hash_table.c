#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "hash_table.h"

#define HASH_TABLE_SIZE ((int)17)

typedef struct entry entry_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[HASH_TABLE_SIZE];
  int size;
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
  entry_t *current = NULL;
  entry_t *next = NULL;

  for (int i = 0; i < HASH_TABLE_SIZE; i++)
  {
    current = &ht->buckets[i];
    next = current->next;
    
    if (next == NULL)
    {
      continue;
    }
    else
    {
      // Skip trying to deallocate the sentinel.
      do
      {
        current = next;
        next = current->next;
        entry_destroy(current);
      }
      while (next != NULL);
    }
  }

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

  int bucket = key % HASH_TABLE_SIZE;

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

  int ht_index = key % HASH_TABLE_SIZE;
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
  int bucket = key % HASH_TABLE_SIZE;
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

int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}

