#pragma once
#include <stdbool.h>

/**
 * @file linked_list.h
 * @author Albin Lord, Karl Wästvind
 * @date 24 Sep 2024
 * @brief TODO.
 */

typedef struct list ioopm_list_t;

typedef bool ioopm_int_predicate(int key, int value, void *extra);
typedef void ioopm_apply_int_function(const int key, int *value, void *extra);

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(void);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

