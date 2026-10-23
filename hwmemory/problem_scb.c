#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off

/******************************************************************************
 * ЗАДАЧА: Problem SCB — обобщённый бинарный поиск
 *
 * ОПИСАНИЕ:
 *   Бинарный поиск можно осуществлять в массиве произвольных элементов, обладая
 *   только информацией о размере элемента и функцией-компаратором.
 *   В этом задании считайте тип компаратора заданным заранее
 *
 *      typedef int (*cmp_t)(const void *lhs, const void *rhs);
 *
 *   Компаратор возвращает отрицательное число, если lhs логически меньше, чем rhs,
 *   0 если они равны и положительное число, если lhs логически больше, чем rhs.
 *
 *   Ваша задача — написать функцию обобщённого бинарного поиска с заранее заданной сигнатурой.
 *
 *      void *cbsearch(const void *key, const void *base, int num, int size, cmp_t cmp);
 *
 *   - key  это элемент, который ищется, base это массив в котором осуществляется поиск;
 *   - size это размер элементов массива (и, соответственно, элемента key);
 *   - num  это количество элементов в массиве.
 *
 *   Решение более чем с логарифмической сложностью вряд ли уложится в лимит по времени.
 *   Функция cbsearch должна возвращать указатель на найденный элемент.
 *   Если ничего не найдено то можно вернуть нулевой указатель или указатель за концом массива.
 *
 * ПРИМЕРЫ:
 *   Пример 1
 *   Вход    : 6
 *             1 2 3 4 5 6
 *   Выход   : 0
 *
 *   Пример 2
 *   Вход    : 40
 *             44 60 65 153 153 174 190 192 196 236 257 264 267 300 319 324 376 445 452 464 511 576 584 609 671 682 684 693 735 737 840 906 918 931 936 940 955 960 965 975 
 *   Выход   : 324 376 445
 ******************************************************************************/

// clang-format on

typedef int (*cmp_t)(void const *key, void const *elt);

/**
 * @brief        : Бинарный поиск в массиве произвольных элементов;
 *
 * @param key    : элемент, который ищется;
 * @param base   : массив, в котором осуществляется поиск;
 * @param num    : количество элементов в массиве;
 * @param size   : размер элементов массива (и, соответственно, элемента key);
 * @param cmp    : функция-компаратор, возвращает отрицательное число, если lhs логически меньше,
 *                 чем rhs, 0 если они равны и положительное число, если lhs логически больше, чем
 * rhs;
 * @return void* : указатель на найденный элемент,
 *                 если ничего не найдено то нулевой указатель;
 */
void *cbsearch(void const *key, void const *base, int num, int size, cmp_t cmp) {
  char *left  = (char *)base;
  char *right = left + (num - 1) * size;

  while (left <= right) {
    char *median = left + (right - left) / 2;
    int cmp_res  = cmp(key, median);
    if (cmp_res < 0)
      right = median - 1;
    if (cmp_res > 0)
      left = median + 1;
    if (cmp_res == 0)
      return (void *)median;
  }

  return NULL;
}

struct triple {
  int x, y, z;
};

int tcmp(void const *key, void const *elt) {
  struct triple const *lhs = (struct triple const *)key;
  struct triple const *rhs = (struct triple const *)elt;
  return (lhs->y - rhs->y);
}

int main() {
  int i, res, n;
  struct triple found = {-1, -1, -1};
  struct triple *parr, *answ;

  res = scanf("%d", &n);
  assert(res == 1);
  assert(n > 2);

  parr = (struct triple *)calloc(n / 3, sizeof(struct triple));

  for (i = 0; i < n / 3; ++i) {
    res = scanf("%d %d %d", &parr[i].x, &parr[i].y, &parr[i].z);
    assert(res == 3);
    if ((i == n / 7) && ((i % 10) != 0))
      found = parr[i];
  }

  answ = (struct triple *)cbsearch(&found, parr, n / 3, sizeof(found), &tcmp);

  if (!answ || answ == NULL) {
    printf("0\n");
    free(parr);
    return 0;
  }

  printf("%d %d %d\n", answ->x, answ->y, answ->z);

  free(parr);
  return EXIT_SUCCESS;
}