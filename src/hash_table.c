#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "hash_table.h"

typedef struct entry entry_t;

struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[17];
};

ioopm_hash_table_t *ioopm_hash_table_create(void)
{
  return calloc(1, sizeof(ioopm_hash_table_t));
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
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
  /// Calculate the bucket for this entry
  int bucket = key % 17;
  /// Search for an existing entry for a key
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
    }
}

char *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key)
{
  return NULL;
}