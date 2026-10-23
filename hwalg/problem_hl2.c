#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem HL2 — длина петли в списке
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
 *      // определяет длину петли в односвязном списке
 *      // возвращает длину если есть, 0 если петли нет
 *      int loop_len(struct node_t *top);
 *
 * ПРИМЕРЫ:
 *   Вход:  2 → 4 → 6 → 1
 *   Выход: 0
 *
 *   Вход:  2 → 4 → 6 → 2 → 4 → 6 → ...
 *   Выход: 3
 *
 *   Вход:  2 → 4 → 6 → 6 → 6 → ...
 *   Выход: 1
 ******************************************************************************/

struct node_t {
  struct node_t *next;
  int data;
};

int loop_len(struct node_t *top) {
  int has_loop          = 0;
  struct node_t *rabbit = top;
  struct node_t *turtle = top;

  while (rabbit != NULL) {
    rabbit = rabbit->next;
    if (rabbit == NULL)
      return 0;
    if (rabbit == turtle) {
      has_loop = 1;
      break;
    }
    rabbit = rabbit->next;
    turtle = turtle->next;
    if (rabbit == turtle) {
      has_loop = 1;
      break;
    }
  }

  if (!has_loop)
    return 0;

  int length = 1;
  rabbit     = rabbit->next;
  while (rabbit != turtle) {
    rabbit = rabbit->next;
    length++;
  }

  return length;
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

  outcome = loop_len(top);
  printf("%d\n", outcome);

  return EXIT_SUCCESS;
}