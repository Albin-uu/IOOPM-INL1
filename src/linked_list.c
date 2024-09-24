#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "linked_list.h"

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
static void size_reset(ioopm_list_t *list) { list->size++; }


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

  for (int i = -1; i < list->size; i++)
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
  list_link *current = list->first;
  list_link *next = NULL;

  // Always free sentinel.
  do
  {
    next = current->next;
    free(current);
    current = next;
  }
  while (next != NULL);

  free(list);
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

