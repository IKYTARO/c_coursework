#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem SQ — быстрая сортировка
 *
 * ОПИСАНИЕ:
 *   Основная часть алгоритма быстрой сортировки выглядит следующим образом
 *
 *      void qsort_impl(int *arr, unsigned low, unsigned high) {
 *          if (low >= high) return;
 *          unsigned pi = partition(arr, low, high);
 *          if (pi > low) qsort_impl(arr, low, pi - 1);
 *          qsort_impl(arr, pi + 1, high);
 *      }
 *
 *      void qsort(int *arr, unsigned len) { qsort_impl(arr, 0u, len - 1); }
 *
 *   Использованная здесь процедура partition делит массив по отношению к первому
 *   элементу на большую и меньшую часть и возвращает позицию сечения.
 *
 *   Ваша задача написать эту функцию с заранее заданной сигнатурой
 *
 *      unsigned partition(int *arr, unsigned low, unsigned high) {
 *          // TODO: ваш код здесь
 *      }
 *
 *   Здесь arr это указатель на начало массива, low это позиция элемента относительно
 *   которого проводить разбиение от позиции low до позиции high включительно.
 *
 *   Возвращаемый результат этой функции это новая позиция элемента, который был
 *   на позиции low. Ваш элемент должен стоять после всех элементов которые меньше
 *   или равны его и до всех элементов которые строго больше его.
 *
 * ПРИМЕРЫ:
 *   Пример 1
 *   Вход    : arr = {2 1 3}, low = 0, high = 2
 *   Выход   : arr = {1 2 3}, результат = 1
 *
 *   Пример 2
 *   Вход    : arr = {1 1 1}, low = 0, high = 2
 *   Выход   : arr = {1 1 1}, результат = 2
 ******************************************************************************/

void swap(int *lhs, int *rhs) {
  int tmp = *lhs;
  *lhs    = *rhs;
  *rhs    = tmp;
}

unsigned partition(int *arr, unsigned low, unsigned high) {
  int pivot = arr[low];
  int left = low + 1, right = high;
  while (left < right) {
    while (arr[left] <= pivot)
      left++;
    while (arr[right] >= pivot)
      right--;
    if (left < right)
      swap(&arr[left], &arr[right]);
  }
  swap(&arr[left - 1], &arr[low]);
  return left - 1;
}

void print_fail(int *arr, int *shadow, int n, int i, int pivot) {
  int j;
  printf("\nFAILED!\nSource array: ");
  for (j = 0; j < n; ++j)
    printf("%d ", shadow[j]);
  printf("\nAfter partition (pivot = %d):\n", pivot);
  for (j = 0; j < n; ++j)
    printf("%d ", arr[j]);
  printf("\nLook at position %d, here something gone wrong\n", i);
}

int main() {
  int res, answ, n, i, pivot;
  int *parr, *shadow;

  res = scanf("%d", &n);
  assert(res == 1);
  assert(n > 0);

  parr = calloc(n, sizeof(int));
  assert(parr);

  shadow = calloc(n, sizeof(int));
  assert(shadow);

  for (i = 0; i < n; ++i) {
    res       = scanf("%d", parr + i);
    shadow[i] = parr[i];
    assert(res == 1);
  }

  pivot = parr[0];

  answ = partition(parr, 0, n - 1);

#if defined(VISUAL)
  for (i = 0; i < n; ++i)
    printf("%d ", shadow[i]);
  printf("\n");
  for (i = 0; i < n; ++i)
    printf("%d ", parr[i]);
  printf("\n");
#endif

  for (i = 0; i < n; ++i) {
    if ((i < answ && parr[i] > pivot) || (i > answ && parr[i] < pivot))
      print_fail(parr, shadow, n, i, pivot);
  }

  printf("%d\n", answ);

  free(parr);
  return EXIT_SUCCESS;
}