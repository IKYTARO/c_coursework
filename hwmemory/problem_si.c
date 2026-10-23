#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem SI — сортировка вставками
 *
 * ОПИСАНИЕ:
 *   Основная часть алгоритма сортировки вставками выглядит следующим образом
 *
 *      void inssort(int *arr, int len) {
 *          int i;
 *          for (i = 0; i < len; ++i) {
 *          int key, pos;
 *          key = arr[i];
 *          pos = moveright(arr, key, i);
 *          arr[pos] = key;
 *          }
 *      }
 *
 *  Использованная здесь процедура moveright сдвигает к младшим адресам текущий ключ
 *  от позиции i к той позиции, которую он должен занять. Сама вставка на вычисленную
 *  позицию происходит после неё.
 *
 *  Ваша задача написать эту функцию с заранее заданной сигнатурой
 *
 *      int moveright(int *arr, int key, int last) {
 *          // TODO: ваш код здесь
 *      }
 *
 *  Здесь arr это указатель на первый элемент массива, key это ключ, позиция для
 *  которого ищется, last это позиция ключа (или иначе первая позиция за последним
 *  отсортированным элементом). Функция возвращает позицию вставки ключа.
 *  На этой позиции должно остаться то значение, с которого началось распространение вправо.
 *
 * ПРИМЕРЫ:
 *   Пример 1
 *   Вход    : arr = {1, 2, 4, 5, 3, 2}, key = 3, last = 4
 *   Выход   : arr = {1, 2, 4, 4, 5, 2}, pos = 2
 *
 *   Пример 2
 *   Вход    : arr = {5, 2, 4, 1, 3, 2}, key = 5, last = 0
 *   Выход   : arr = {1, 2, 4, 4, 5, 2}, pos = 2
 ******************************************************************************/

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

  return EXIT_SUCCESS;
}