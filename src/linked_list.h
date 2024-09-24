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

