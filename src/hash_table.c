#include <stdlib.h>
#include <stdio.h>
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
  entry_t *buckets[17];
};

ioopm_hash_table_t *ioopm_hash_table_create(void) {
  return calloc(1, sizeof(ioopm_hash_table_t));
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  free(ht);
}

