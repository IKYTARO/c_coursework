#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*cmp_t)(void const *key, void const *elt);

/**
 * @brief        : Бинарный поиск в массиве произвольных элементов;
 *
 * @param key    : элемент, который ищется;
 * @param base   : массив, в котором осуществляется поиск;
 * @param num    : количество элементов в массиве;
 * @param size   : размер элементов массива (и, соответственно, элемента key);
 * @param cmp    : функция-компаратор, возвращает отрицательное число, если lhs логически меньше,
 * чем rhs, 0 если они равны и положительное число, если lhs логически больше, чем rhs;
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
  return 0;
}
