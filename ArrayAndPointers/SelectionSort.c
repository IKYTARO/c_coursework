#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Сортировка выбором.
    Сортировка выбором использует линейный поиск в несортированной части массива
    и обмен с подходящим значением в сортированной части.
*/

void print_arr(int const *arr, unsigned size) {
  for (unsigned i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

void swap(int *lhs, int *rhs) {
  int tmp = *lhs;
  *lhs    = *rhs;
  *rhs    = tmp;
}

unsigned get_min_pos(int const *arr, unsigned start, unsigned end) {
  unsigned min_pos = start;
  for (unsigned i = start + 1; i < end; i++)
    if (arr[i] < arr[min_pos])
      min_pos = i;
  return min_pos;
}

void selection_iteration(int *arr, unsigned size, unsigned sorted_pos) {
  unsigned min_pos = get_min_pos(arr, sorted_pos + 1, size);
  if (arr[min_pos] < arr[sorted_pos])
    swap(&arr[sorted_pos], &arr[min_pos]);
}

void selection_sort(int *arr, unsigned size) {
  for (unsigned i = 0; i < size; i++)
    selection_iteration(arr, size, i);
}

int main() {
  int answ, size, sorted_pos;
  int *arr;

  int res = scanf("%d", &size);
  assert(res == 1);
  assert(size > 0);

  arr = (int *)calloc(size, sizeof(int));

  for (int i = 0; i < size; ++i) {
    res = scanf("%d", &arr[i]);
    assert(res == 1);
  }

  res = scanf("%d", &sorted_pos);
  assert(res == 1);
  assert(sorted_pos < size);

  selection_iteration(arr, size, sorted_pos);
  print_arr(arr, size);

  free(arr);

  return 0;
}
