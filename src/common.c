#include <stdbool.h>

#include "common.h"

bool ioopm_equals_int(elem_t a, elem_t b)
{
  return a.int_v == b.int_v;
}

