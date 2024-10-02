#include <stdbool.h>
#include <string.h>

#include "common.h"


bool ioopm_equals_int(elem_t a, elem_t b)
{
  return a.int_v == b.int_v;
}

bool ioopm_equals_str(elem_t a, elem_t b)
{
  char *str_a = (char *) a.ptr_v;
  char *str_b = (char *) b.ptr_v;
  return (strcmp(str_a, str_b) == 0);
}
