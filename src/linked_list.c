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

