#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST 1

void scan_and_check(int input_res, int valid_res) {
  if (input_res != valid_res) {
    printf("Wrong input!\n");
    abort();
  }
}

void print_arr(int const *arr, unsigned size) {
  for (unsigned i = 0; i < size - 1; i++)
    printf("%d ", arr[i]);
  printf("%d", arr[size - 1]);
}

int get_max_elem(int const *arr, unsigned size) {
  int max_elem = arr[0];
  for (unsigned i = 1; i < size; i++)
    if (arr[i] > max_elem)
      max_elem = arr[i];
  return max_elem;
}

int get_ndigits(int number) {
  if (number == 0)
    return 1;

  int ndigits = 0;
  while (number > 0) {
    number /= 10;
    ndigits++;
  }
  return ndigits;
}

void radix_step(int *arr, unsigned size, int expn, int *sorted_arr) {
  int backet[10]     = {0};
  int tmp_backet[10] = {0};
  for (unsigned i = 0; i < size; i++) {
    int digit = (arr[i] / (expn + 1)) % 10;
    backet[digit]++;
    tmp_backet[digit] = backet[digit];
  }
  for (unsigned i = 1; i < 10; i++)
    backet[i] += backet[i - 1];
  for (unsigned i = 0; i < 10; i++)
    backet[i] -= tmp_backet[i];

  for (unsigned i = 0; i < size; i++) {
    int digit           = (arr[i] / (expn + 1)) % 10;
    int new_pos         = backet[digit];
    sorted_arr[new_pos] = arr[i];
    backet[digit]++;
  }
}

void radix_sort(int *arr, unsigned size) {
  int expn        = 0;
  int ndigits     = get_ndigits(get_max_elem(arr, size));
  int *sorted_arr = (int *)calloc(size, sizeof(int));
  for (unsigned i = 0; i < ndigits; i++) {
    radix_step(arr, size, expn, sorted_arr);
    memcpy(arr, sorted_arr, size * sizeof(int));
    expn *= 10;
  }
  free(sorted_arr);
}

int main() {
  unsigned size, expn;
  scan_and_check(scanf("%d", &size), 1);
  int *arr = (int *)calloc(size, sizeof(int));
  for (unsigned i = 0; i < size; i++)
    scan_and_check(scanf("%d", &arr[i]), 1);
  scan_and_check(scanf("%d", &expn), 1);

#if TEST
  int *sorted_arr = (int *)calloc(size, sizeof(int));
  radix_step(arr, size, expn, sorted_arr);

  print_arr(sorted_arr, size);
  printf("\n");

  // int hit_count = size;
  // int* correct_sorted = (int*) calloc(size, sizeof(int));
  // for (unsigned i = 0; i < size; i++) {
  //     scan_and_check(scanf("%d", &correct_sorted[i]), 1);
  //     if (sorted_arr[i] == correct_sorted[i])
  //         hit_count--;
  // }

  // if (hit_count == 0)
  //     printf("Test passed;\n");
  // else {
  //     printf("Test failed: correct {");
  //     print_arr(correct_sorted, size);
  //     printf("} != given {");
  //     print_arr(sorted_arr, size);
  //     printf("}\n");
  // }

  // free(correct_sorted);
  free(sorted_arr);
#else
  radix_sort(arr, size);
  print_arr(arr, size);
  printf("\n");
#endif

  free(arr);
  return 0;
}
