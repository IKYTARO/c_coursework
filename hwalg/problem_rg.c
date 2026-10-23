#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem RG — период генератора
 *
 * ОПИСАНИЕ:
 *   Заранее задан (определять его не надо) генератор f(x)
 *
 *      typedef int (*generator_t)(int);
 *
 *   Вам необходимо написать функцию с заранее заданной сигнатурой
 *
 *      // определяет длину петли в генераторе
 *      unsigned cycle_len(generator_t gen);
 *
 *   Генератор считает с нуля и в следующий раз принимает результат предыдущего вызова.
 *   Вы можете быть уверены, что петля есть. Функция должна возвращать её длину.
 *
 * ПРИМЕРЫ:
 *   Вход:  f(x) = (x + 2) % 5 : 0, 2, 4, 1, 3, 0, 2, ...
 *   Выход: 5
 *
 *   Вход:  f(x) = (x + 2) % 5 + 1 : 0, 3, 1, 4, 2, 5, 3, 1, 4 ...
 *   Выход: 5
 *
 *   Вход:  f(x) = (x + 2) % 5 + 3 : 0, 5, 5, 5, 5, ...
 *   Выход: 1
 ******************************************************************************/

int a, b, c, d;

int mygen(int num) {
  return (a * num + b) % c + d;
}

typedef int (*generator_t)(int);

unsigned cycle_len(generator_t gen) {
  int turtle = gen(0), rabbit = gen(gen(0));
  while (rabbit != turtle) {
    rabbit = gen(gen(rabbit));
    turtle = gen(turtle);
  }
  int loop_start = turtle, value = gen(turtle), length = 1;
  while (value != loop_start) {
    value = gen(value);
    length++;
  }
  return length;
}

int main() {
  int res = scanf("%d%d%d%d", &a, &b, &c, &d);
  assert(res);
  printf("%u\n", cycle_len(mygen));
  return EXIT_SUCCESS;
}
