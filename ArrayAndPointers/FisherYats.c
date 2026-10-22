#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Написать написать программу, которая:
    Считывает со стандартного ввода размер массива и дальше сам массив.
    Печатает на стандартный вывод перетасованный массив. Для тасования используйте алгоритм
   Фишера-Йетса.
*/

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
  for (int i = 0; i < size; i++) {
    check_input_res(scanf("%i", &value));
    array[i] = value;
  }

  shuffle(array, size);
  for (int i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
  free(array);

  return 0;
}
