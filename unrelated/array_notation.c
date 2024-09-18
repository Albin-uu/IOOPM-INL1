#include <stdint.h>
#include <stdio.h>
int arr_int[] = {1, 2, 3, 4, 5};
char arr_str[] = "somestring";

long arr_long[] = {5, 6, 7, 8, 9};

int main(void)
{
  printf("sizeof int: %lu\n", sizeof(int));
  printf("normal int: %d\n", arr_int[2]);
  printf("wrong int: %d\n\n", 2[arr_int]);

  printf("sizeof char: %lu\n", sizeof(char));
  printf("normal char: %c\n", arr_str[2]);
  printf("wrong char: %c\n\n", 2[arr_str]);

  printf("sizeof long: %lu\n", sizeof(long));
  printf("normal long: %ld\n", arr_long[2]);
  printf("wrong long: %ld\n\n", 2[arr_long]);

  return 0;
}
