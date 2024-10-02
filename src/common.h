#pragma once

#include <stdbool.h>

typedef union elem elem_t;

// v for value
union elem
{
  int int_v;
  unsigned int uint_v;
  bool bool_v;
  float float_v;
  void *ptr_v;
};

#define int_elem(x) (elem_t) { .int_v = x}
#define uint_elem(x) (elem_t) { .uint_v = x}
#define bool_elem(x) (elem_t) { .bool_v = x }
#define float_elem(x) (elem_t) { .float_v = x }
#define ptr_elem(x) (elem_t) { .ptr_v = x }


typedef bool ioopm_predicate(const elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(const elem_t key, elem_t *value, void *extra);
// Compares two elements and returns true if they are equal
typedef bool ioopm_eq_function(elem_t a, elem_t b);

// Simple equals functions for elem_t types

bool ioopm_equals_int(elem_t a, elem_t b);
bool ioopm_equals_str(elem_t a, elem_t b);