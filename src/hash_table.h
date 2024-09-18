#pragma once

/**
 * @file hash_table.h
 * @author Albin Lord, Karl Wästvind
 * @date 16 Sep 2024
 * @brief Simple hash table that maps integer keys to string values.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(void);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key >= 0, to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result pointer to string that will contain looked up value on success
/// @return bool denoting if lookup succeeded
/// Return value need to be checked before accessing result.
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key, char **result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return bool denoting if removal succeeded
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key);
