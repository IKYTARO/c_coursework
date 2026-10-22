#include <stdio.h>

/*
    Заранее задан генератор f(x)
        typedef int (*generator_t)(int);
    Необходимо написать функцию, которая определяет длину петли в генераторе.
        unsigned cycle_len(generator_t gen);
    Генератор считает с нуля и в следующий раз принимает результат предыдущего
   вызова

    Sample 1: f(x) = (x + 2) % 5;
              0, 2, 4, 1, 3, 0, 2, ...
    Output 1: 5

    Sample 2: f(x) = (x + 2) % 5 + 1;
              0, 3, 1, 4, 2, 5, 3, 1, 4 ...
    Output 2: 5

    Sample 3: f(x) = (x + 2) % 5 + 3;
              0, 5, 5, 5, 5, ...
    Output 3: 1

    Sample 4: f(x) = (3 * x + 2) % 5 + 4;
              0, 6, 4, 8, 5, 6 ...
    Output 4: 4
*/

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
