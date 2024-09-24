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


