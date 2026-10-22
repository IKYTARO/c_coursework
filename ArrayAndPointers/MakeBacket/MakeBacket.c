#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

int *make_backet(int const *arr, unsigned size) {
  int backet_size = get_max_elem(arr, size) + 1;
  int *backet     = (int *)calloc(backet_size, sizeof(int));
  for (unsigned i = 0; i < size; i++)
    backet[arr[i]]++;
  return backet;
}

int main() {
  unsigned size;
  scan_and_check(scanf("%d", &size), 1);
  int *arr = (int *)calloc(size, sizeof(int));
  for (unsigned i = 0; i < size; i++)
    scan_and_check(scanf("%d", &arr[i]), 1);

  int backet_size = get_max_elem(arr, size) + 1;
  int *backet     = make_backet(arr, size);

#if TEST
  int hit_count       = backet_size;
  int *correct_backet = (int *)calloc(backet_size, sizeof(int));
  for (unsigned i = 0; i < backet_size; i++) {
    scan_and_check(scanf("%d", &correct_backet[i]), 1);
    if (backet[i] == correct_backet[i])
      hit_count--;
  }

  if (hit_count == 0)
    printf("Test passed;\n");
  else {
    printf("Test failed: correct {");
    print_arr(correct_backet, backet_size);
    printf("} != given {");
    print_arr(backet, backet_size);
    printf("}\n");
  }

  free(correct_backet);
#else
  print_arr(backet, backet_size);
#endif

  free(arr);
  free(backet);
  return 0;
}
