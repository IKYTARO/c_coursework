#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *lhs_item, void *rhs_item, unsigned elt_size) {
  void *buff = malloc(elt_size);
  memcpy(buff, lhs_item, elt_size);
  memcpy(lhs_item, rhs_item, elt_size);
  memcpy(rhs_item, buff, elt_size);
  free(buff);
}

struct triple {
  int x, y, z;
};

int tcmp(void const *key, void const *elt) {
  struct triple const *lhs = (struct triple const *)key;
  struct triple const *rhs = (struct triple const *)elt;
  if (lhs->x == rhs->x && lhs->y == rhs->y)
    return (lhs->z < rhs->z);
  if (lhs->x == rhs->x)
    return (lhs->y < rhs->y);
  return (lhs->x < rhs->x);
}

typedef int (*cmp_t)(void const *lhs, void const *rhs);

/**
 * @brief          : функция одного шага обобщённой сортировки выбором;
 *
 * @param arr      : массив, уже отсортированный до позиции nsorted;
 * @param elt_size : размер элементов массива;
 * @param size     : это количество элементов в массиве;
 * @param nsorted  : на этой позиции (с нуля) стоит первый несортированный элемент;
 * @param cmp      : функция-компаратор, возвращает 1 если lhs логически меньше,
 *                   чем rhs, иначе 0
 * @return int     : 0 при успешном выполнении.
 */
int selstep(void *arr, int elt_size, int size, int nsorted, cmp_t cmp) {
  unsigned min_pos = nsorted;
  for (unsigned i = nsorted + 1; i < size; i++) {
    void *lhs_item = (void *)((char *)arr + i * elt_size);
    void *rhs_item = (void *)((char *)arr + min_pos * elt_size);
    if (cmp(lhs_item, rhs_item) == 1)
      min_pos = i;
  }
  void *min_pos_item = (void *)((char *)arr + min_pos * elt_size);
  void *nsorted_item = (void *)((char *)arr + nsorted * elt_size);
  if (cmp(min_pos_item, nsorted_item) == 1)
    printf("%d %d\n", min_pos, nsorted);
  swap(min_pos_item, nsorted_item, elt_size);

  return 0;
}

int main() {
  int i, res, n, last;
  struct triple *parr;

  res = scanf("%d", &n);
  assert(res == 1);
  assert(n > 2);

  parr = (struct triple *)calloc(n / 3, sizeof(struct triple));

  for (i = 0; i < n / 3; ++i) {
    res = scanf("%d %d %d", &parr[i].x, &parr[i].y, &parr[i].z);
    assert(res == 3);
  }

  res = scanf("%d", &last);
  assert(res == 1);
  assert(last < n);

  selstep(parr, sizeof(struct triple), n / 3, last / 3, &tcmp);

  for (i = 0; i < n / 3; ++i)
    printf("%d %d %d\n", parr[i].x, parr[i].y, parr[i].z);

  free(parr);
  return 0;
}
