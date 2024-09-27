#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"

#define Free(ptr) {free(ptr); ptr = NULL; }

typedef struct link list_link;
struct link
{
  int value;
  list_link *next;
};

typedef struct list ioopm_list_t;
struct list
{
  list_link *first;
  list_link *last;
  int size;
};

static void size_increment(ioopm_list_t *list) { list->size++; }
static void size_decrement(ioopm_list_t *list) { list->size--; }
static void size_reset(ioopm_list_t *list) { list->size = 0; }


static list_link *first_proper_elem(ioopm_list_t *list)
  { return list->first->next; }

static list_link *last_proper_elem(ioopm_list_t *list)
  { return list->last; }

static list_link *get_sentinel(ioopm_list_t *list)
  { return list->first; }


static list_link *get_link_before_index(ioopm_list_t *list, int index)
{
  int size = ioopm_linked_list_size(list);
  list_link *current_link = get_sentinel(list);

  // Index for sentinel is -1.
  for (int i = -1; i < size; i++)
  {
    if (i == index - 1)
    {
      break;
    }
    else
    {
      current_link = current_link->next;
    }  
  }
  
  return current_link;
}

// Frees all links including and onward from the argument link.
// Does not change any pointers!
static void free_links_from(list_link *current)
{
  list_link *next = NULL;
  while (current != NULL)
  {
    next = current->next;
    Free(current);
    current = next;
  }
}

typedef struct
{
  bool result;
  bool set_result_on_success;
  bool set_result_on_failure;
  ioopm_predicate *pred;
  void *extra;
} pred_struct;

static void matches_pred(int index, void *value, pred_struct *p_struct)
{
  bool set_if_success = p_struct->set_result_on_success;
  bool set_if_fail = p_struct->set_result_on_failure;
  bool pred_successful = p_struct->pred(index, value, p_struct->extra);

  if (set_if_success && pred_successful)
  {
    p_struct->result = true;
  }
  else if (set_if_fail && !pred_successful)
  {
    p_struct->result = false;
  }
}

static void iter_for_each(ioopm_list_t *list, ioopm_apply_function *arg_func, void *arg_opt)
{
  list_link *current = get_sentinel(list);
  int index = -1;

  // Check sentinel first in case list empty.
  while (current->next != NULL)
  {
    current = current->next;
    index++;
    // & temp until value is generalised
    arg_func(index, &current->value, arg_opt);
  }
}

ioopm_list_t *ioopm_linked_list_create(void)
{
  ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
  list_link *sentinel = calloc(1, sizeof(list_link));

  sentinel->next = NULL;
  sentinel->value = 0;

  list->first = sentinel;
  list->last = sentinel;
  list->size = 0;

  return list;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  // Also free's sentinel.
  free_links_from(list->first);
  Free(list);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
  list_link *current_first = first_proper_elem(list);
  list_link *new_link = calloc(1, sizeof(list_link));

  new_link->value = value;
  new_link->next = current_first;

  list_link *sentinel = get_sentinel(list);
  sentinel->next = new_link;

  size_increment(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
  list_link *current_last = last_proper_elem(list);
  list_link *new_link = calloc(1, sizeof(list_link));

  new_link->value = value;
  new_link->next = NULL;

  current_last->next = new_link;
  list->last = new_link;

  size_increment(list);
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, int value)
{
  // +1 since inserting will count index as after the insertion.
  assert(index >= 0 && index < ioopm_linked_list_size(list) + 1);

  list_link *prev_link = get_link_before_index(list, index);
  list_link *new_link = calloc(1, sizeof(list_link));

  new_link->next = prev_link->next;
  prev_link->next = new_link;

  new_link->value = value;
  size_increment(list);
}

int ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  assert(index >= 0 && index < ioopm_linked_list_size(list));

  list_link *prev_link = get_link_before_index(list, index);
  list_link *to_be_removed = prev_link->next;

  // Edge case if elem is last in list.
  if (index == ioopm_linked_list_size(list) - 1) { list->last = prev_link; }

  prev_link->next = to_be_removed->next;
  
  int value_of_removed = to_be_removed->value;
  Free(to_be_removed);
  size_decrement(list);

  return value_of_removed;
}

int ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  assert(index >= 0 && index < ioopm_linked_list_size(list));

  list_link *prev_link = get_link_before_index(list, index);
  return prev_link->next->value;
}

// TODO figure out general function
static bool is_equal_int(int index, void *value, void *compare_value)
{
  int *val = (int *) value;
  int *compare_val = (int *) compare_value;
  return *val == *compare_val;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, int element)
{
  // temp &
  pred_struct p_struct =
  {
    // TODO int function to general
    .pred = &is_equal_int,
    .extra = &element,
    .set_result_on_success = true,
    .set_result_on_failure = false,
    .result = false,
  };

  iter_for_each(list, (ioopm_apply_function *) matches_pred, &p_struct);
  return p_struct.result;
}

int ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return ioopm_linked_list_size(list) == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  // Don't free sentinel.
  list_link *first_to_remove = first_proper_elem(list);
  free_links_from(first_to_remove);
  first_to_remove = NULL;

  list_link *sentinel = get_sentinel(list);
  sentinel->next = NULL;
  list->last = sentinel;
  size_reset(list);
}

