#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem BI -- инверсия бита по индексу
 *
 * ОПИСАНИЕ:
 *   Вам предлагается инвертировать N-й по счёту бит в массиве. Номер бита отсчитывается от
 *   нулевого бита нулевой ячейки массива. Нулевой бит первой ячейки будет иметь номер 8 и т.д.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - size: unsigned количество чисел, каждое не больше одного байта
 *   - num1, num2, ...: int все эти числа
 *   - pos: unsigned номер бита
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - num1, num2, ...: int массив в котором битс номером pos инвертирован
 *
 * ПРИМЕРЫ:
 *   Вход: 4 4 3 3 3 17
 *   Выход: 4 3 1 3
 *
 *   Вход: 4 3 3 3 3 31
 *   Выход: 3 3 3 131
 *
 *   Вход: 4 3 3 3 3 35
 *   Выход: 3 3 3 3
 ******************************************************************************/

void check_input_res(int res);
void inverse(int *array, unsigned pos);
void print_array(int *array, unsigned size);

int main() {
  unsigned size, pos;
  int res = scanf("%u", &size);
  check_input_res(res);

  int *array = (int *)calloc(size, sizeof(int));
  int value  = 0;
  for (unsigned i = 0; i < size; i++) {
    res = scanf("%i", &value);
    check_input_res(res);
    array[i] = value;
  }

  res = scanf("%u", &pos);
  check_input_res(res);

  inverse(array, pos);
  print_array(array, size);
  free(array);

  return EXIT_SUCCESS;
}

void check_input_res(int res) {
  if (res == -1) {
    printf("Wrong input!\n");
    abort();
  }
}

void inverse(int *array, unsigned pos) {
  unsigned slot = pos / __CHAR_BIT__;
  unsigned cell = pos % __CHAR_BIT__;
  array[slot]   = array[slot] ^ (1u << cell);
}

void print_array(int *array, unsigned size) {
  for (unsigned i = 0; i < size; i++) {
    printf("%i ", array[i]);
  }
}