#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem FY — тасование массива
 *
 * ОПИСАНИЕ:
 *   Ваша задача -- написать программу, которая:
 *   - Считывает со стандартного ввода размер массива и дальше сам массив.
 *   - Печатает на стандартный вывод перетасованный массив.
 *   - Для тасования используйте алгоритм Фишера-Йетса и следующую функцию myrand.
 *
 *      int myrand() {
 *          static unsigned long int seed = 1;
 *          seed = seed * 1103515245 + 12345;
 *          return (unsigned int)(seed / 65536) % 32768;
 *      }
 *
 *   Использовать именно такую функцию необходимо чтобы ваша тасовка совпала с ответом в тесте.
 *   Если в массиве только один элемент, напечатайте его.
 *   Если элементов нет, напечатайте "0".
 *
 * ПРИМЕРЫ:
 *   Пример 1
 *   Вход    : 1 42
 *   Выход   : 42
 *
 *   Пример 2
 *   Вход    : 5 1 2 3 4 5
 *   Выход   : 5 2 1 3 4
 ******************************************************************************/

void check_input_res(int res) {
  if (res == -1) {
    printf("Wrong input!\n");
    abort();
  }
}

int myrand() {
  static unsigned long int seed = 1;
  seed                          = seed * 1103515245 + 12345;
  return (unsigned int)(seed / 65536) % 32768;
}

void shuffle(int *array, unsigned size) {
  if (size <= 1)
    return;

  int ran_pos     = myrand() % size;
  int last_elem   = array[size - 1];
  array[size - 1] = array[ran_pos];
  array[ran_pos]  = last_elem;
  shuffle(array, size - 1);
}

int main() {
  unsigned size;
  check_input_res(scanf("%u", &size));
  if (size == 0)
    printf("0");

  int *array = (int *)calloc(size, sizeof(int));
  assert(array);
  int value = 0;
  for (unsigned i = 0; i < size; i++) {
    check_input_res(scanf("%i", &value));
    array[i] = value;
  }

  shuffle(array, size);
  for (unsigned i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
  free(array);

  return EXIT_SUCCESS;
}