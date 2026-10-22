#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Сортировка вставками.
*/

void check_input_res(int res) {
  if (res == -1) {
    printf("Wrong input!\n");
    abort();
  }
}

int moveright(int *arr, int key, int last) {
  for (int i = 0; i < last; i++) {
    if (arr[i] > key) {
      for (int j = last; j > i; j--)
        arr[j] = arr[j - 1];
      return i;
    }
  }
  return last;
}

void insertion_sort(int *arr, int size) {
  for (int i = 0; i < size; ++i) {
    int key, pos;
    key      = arr[i];
    pos      = moveright(arr, key, i);
    arr[pos] = key;
  }
}

int main() {
  int res, answ, n, last, i;
  int *parr;

  res = scanf("%d", &n);
  assert(res == 1);
  assert(n > 0);

  parr = (int *)calloc(n, sizeof(int));

  for (i = 0; i < n; ++i) {
    res = scanf("%d", parr + i);
    assert(res == 1);
  }

  res = scanf("%d", &last);
  assert(res == 1);
  assert(last < n);

  answ = moveright(parr, parr[last], last);

  for (i = 0; i < n; ++i)
    printf("%d ", parr[i]);
  printf("\n");
  printf("%d\n", answ);
  free(parr);

  return 0;
}
