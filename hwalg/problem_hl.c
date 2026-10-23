#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem HL — петля в списке
 *
 * ОПИСАНИЕ:
 *   Заранее задан (определять его не надо) тип структуры
 *
 *      struct node_t {
 *          struct node_t *next;
 *          int data;
 *      };
 *
 *   Вам необходимо написать функцию с заранее заданной сигнатурой
 *
 *      // определяет есть ли петля в односвязном списке
 *      // возвращает 0 если нет и 1 если есть
 *      int list_is_a_loop(struct node_t *top);
 *
 * ПРИМЕРЫ:
 *   Вход:  2 → 4 → 6 → 1
 *   Выход: 0
 *
 *   Вход:  2 → 4 → 6 → 2 → 4 → 6 → ...
 *   Выход: 1
 *
 *   Вход:  2 → 4 → 6 → 6 → 6 → ...
 *   Выход: 1
 ******************************************************************************/

struct node_t {
  struct node_t *next;
  int data;
};

int list_is_a_loop(struct node_t *root) {
  struct node_t *rabbit = root;
  struct node_t *turtle = root;
  while (rabbit != NULL) {
    rabbit = rabbit->next;
    if (rabbit == NULL)
      return 0;
    if (rabbit == turtle)
      return 1;
    rabbit = rabbit->next;
    turtle = turtle->next;
    if (rabbit == turtle)
      return 1;
  }
  return 0;
}

int main() {
  int i, nnodes, looppos, res, outcome;
  struct node_t *top = NULL, *cur = NULL, *loopelem = NULL;

  res = scanf("%d%d", &nnodes, &looppos);
  assert(res == 2);
  assert(nnodes >= 0);
  assert(looppos > -2);
  assert(looppos < nnodes);

  if (nnodes > 0) {
    top = calloc(1, sizeof(struct node_t));
    cur = top;
    if (looppos == 0)
      loopelem = cur;
  }

  for (i = 1; i < nnodes; ++i) {
    cur->next = calloc(1, sizeof(struct node_t));
    if (looppos == i)
      loopelem = cur;
    cur = cur->next;
  }

  if (looppos != -1)
    cur->next = loopelem;

  outcome = list_is_a_loop(top);
  printf("%d\n", outcome);

  return EXIT_SUCCESS;
}