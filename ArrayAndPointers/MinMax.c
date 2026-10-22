#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Необходимо написать программу, которая:
    Считывает со стандартного ввода размер массива и дальше сам массив.
    Печатает на стандартный вывод минимальный и максимальный элементы.
    Если в массиве только один элемент, напечатайте его
    Если элементов нет, напечатайте "0 0"
*/

void check_input_res(int res) {
  if (res == -1) {
    printf("Wrong input!\n");
    abort();
  }
}

struct mm_pair {
  int max, min;
};

struct mm_pair get_min_max(int const *array, const unsigned size) {
  struct mm_pair answer;
  answer.max = array[1];
  answer.min = array[1];
  for (int i = 1; i < size; i++) {
    if (array[i] > answer.max)
      answer.max = array[i];
    if (array[i] < answer.min)
      answer.min = array[i];
  }

  return answer;
}

int main() {
  unsigned size;
  check_input_res(scanf("%u", &size));

  int *array = calloc(size, sizeof(int));
  int value  = 0;
  for (int i = 0; i < size; i++) {
    check_input_res(scanf("%i", &value));
    array[i] = value;
  }

  struct mm_pair answer = get_min_max(array, size);

  printf("%d %d", answer.min, answer.max);
  free(array);

  return 0;
}
