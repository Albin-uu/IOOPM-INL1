#pragma once

#include <stdbool.h>
// Needed for size_t.
#include <stdlib.h>

#include "common.h"
#include "linked_list.h"

/**
 * @file hash_table.h
 * @author Albin Lord, Karl Wästvind
 * @date 16 Sep 2024
 * @brief Simple hash table that maps elem_t keys to elem_t values.
 * value comparator, key comparator, and key hashing function are sent on creation.
 * if these higher-order functions are omitted, the hash table defaults to ints.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

typedef struct hash_table ioopm_hash_table_t;

typedef int ioopm_hash_function(const elem_t key);

/// @brief Create a new hash table
/// @brief if value_eq_fn is omitted hash table defaults to int values
/// @brief if key_eq_fn is omitted hash table defaults to int keys
/// @brief if hash_fn is omitted hash table defaults to the int keys being its own hash
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(
    ioopm_eq_function *value_eq_fn,
    ioopm_eq_function *key_eq_fn,
    ioopm_hash_function *hash_fn);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @brief return value need to be checked before accessing result.
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result pointer to elem_t that will contain looked up value on success
/// @return bool denoting if lookup succeeded
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @param send_back the value that was removed,
/// can be NULL if only the removal should be done. 
/// @return bool denoting if removal succeeded
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *send_back);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @brief the list needs to be ioopm_linked_list_destroy'd after use.
/// @param h hash table operated upon
/// @return a linked list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(const ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @brief the list needs to be ioopm_linked_list_destroy'd after use.
/// @param h hash table operated upon
/// @return a linked list of values for hash table h
ioopm_list_t *ioopm_hash_table_values(const ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, const elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(const ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg);
