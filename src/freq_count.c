#include "common.h"
#include "hash_table.h"
#include "linked_list.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Hash function grabbed from half-finished instruction file.
int string_sum_hash(elem_t e)
{
  char *str = e.ptr_v;
  int result = 0;
  do
  {
    result += *str;
  }
  while (*++str != '\0');

  return result;
}

// Returns true if new element, false if already exists.
bool add_or_increment(ioopm_hash_table_t *ht, char *word)
{
  elem_t word_value = { .int_v = 0 };
  if (ioopm_hash_table_lookup(ht, ptr_elem(word), &word_value))
  {
    word_value.int_v++;
    ioopm_hash_table_insert(ht, ptr_elem(word), word_value);
    return false;
  }
  else
  {
    ioopm_hash_table_insert(ht, ptr_elem(word), int_elem(1));
    return true;
  }  
}

// Cant do CUNIT test_case since there would be 2 main functions.
void raw_test_add_or_increment()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, ioopm_equals_str, string_sum_hash);

  char word[] = "Captain Beefheart";
  elem_t result;
  for (int i = 0; i < 3; i++)
  {
    add_or_increment(ht, word);
  }

  bool success = ioopm_hash_table_lookup(ht, ptr_elem(word), &result);
  assert(success);
  assert(result.int_v == 3);

  ioopm_hash_table_destroy(ht);
}

static void free_ptr_elem(elem_t key, elem_t *value, void *unusued)
{
  free(key.ptr_v);
}

// Grabbed from half-finished instruction file.
static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}
// Grabbed from half-finished instruction file.
void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

// Open file and add all words to hash table.
void process_file(ioopm_hash_table_t *ht, char *file_path)
{
  FILE *current_file = fopen(file_path, "r");

  char *line = NULL;
  size_t bufsize = 1000;
  while (getline(&line, &bufsize, current_file) != -1)
  {
    // Delimiters grabbed from half-finished instruction file.
    char *word = strtok(line, "+-#@()[]{}.,:;!? \t\n\r");
    char *copy_of_word = NULL;
    bool word_is_new = true;
    while (word != NULL)
    {
      copy_of_word = strdup(word);
      word_is_new = add_or_increment(ht, copy_of_word);
      if (!word_is_new) { free(copy_of_word); }

      // NULL in arg says to use the string from previous call and continue.
      word = strtok(NULL, "+-#@()[]{}.,:;!? \t\n\r");
    }
  }

  free(line);
  fclose(current_file);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printf("File path needs to be inputted as arguments!\n");
    return 0;
  }

  raw_test_add_or_increment();

  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, ioopm_equals_str, string_sum_hash);

  int last_file_index = argc - 1;
  int current_file_index = 1;
  while (current_file_index <= last_file_index)
  {
    process_file(ht, argv[current_file_index]);
    current_file_index++;
  }

  ioopm_list_t *word_list = ioopm_hash_table_keys(ht);

  // TODO Temp until iterators exist.
  int unique_count = ioopm_hash_table_size(ht);
  char *word_arr[unique_count];
  for (int i = 0; i < unique_count; i++)
  {
    word_arr[i] = ioopm_linked_list_get(word_list, i).ptr_v;
  }
  sort_keys(word_arr, unique_count);

  for (int i = 0; i < unique_count; i++)
  {
    elem_t occurences_of_word;
    ioopm_hash_table_lookup(ht, ptr_elem(word_arr[i]), &occurences_of_word);
    printf("%s: %d\n", word_arr[i], occurences_of_word.int_v);
  }


  ioopm_hash_table_apply_to_all(ht, free_ptr_elem, NULL);
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(word_list);

  return 0;
}