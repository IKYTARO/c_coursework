#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem CF -- цепные дроби
 *
 * ОПИСАНИЕ:
 *   Цепная дробь это красивый математический объект, представляющий
 *   деление двух чисел как итерированные паривльные дроби.
 *
 *   Например:
 *      101/60 = 1 + 1/(1 + 1/(2 + 1/(6 + 1/3)))
 *      101/60 = [1, 1, 2, 6, 3] - поразрядная запись
 *
 *   Необходимо написать программу, которая по заданным числителю и знаменателю
 *   дроби выдает все разряды цепной дроби в ее дробной части.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [numerator]:   [long long] [числитель]
 *   - [denominator]: [long long] [знаменатель]
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [a, b, ...]: [long long] [разряды цепной дроби]
 *
 * ПРИМЕРЫ:
 *   Вход: 1 3
 *   Выход: 0 3
 *
 *   Вход: 49 13
 *   Выход: 3 1 3 3
 ******************************************************************************/

void print_CF(long long numerator, long long denominator);

int main() {
  long long numerator, denominator;
  if (scanf("%lld%lld", &numerator, &denominator) != 2) {
    perror("Invalid input");
    abort();
  }

  print_CF(numerator, denominator);

  return EXIT_SUCCESS;
}

void print_CF(long long numerator, long long denominator) {
  while (denominator != 0) {
    long long remain  = numerator % denominator;
    long long integer = numerator / denominator;

    printf("%lld ", integer);
    numerator   = denominator;
    denominator = remain;
  }
  printf("\n");
}