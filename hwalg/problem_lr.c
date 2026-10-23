#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem LR — разворот списка
 *
 * ОПИСАНИЕ:
 *   Заранее задан (определять его не надо) тип структуры
 *
 *      struct node_t {
 *          struct node_t *next;
 *          int data;
 *      };
 *
 *   Следующий код рекурсивно переворачивает список в памяти (вы можете быть уверены,
 *   что в списке нет петли).
 *
 *      struct node_t * reverse(struct node_t *top) {
 *          struct node_t *xs;
 *          if (NULL == top) return NULL;
 *          if (NULL == top->next) return top;
 *          xs = reverse(top->next);
 *          top->next->next = top;
 *          top->next = NULL;
 *          return xs;
 *      }
 *
 *   Вам необходимо написать и отправить на проверку такую же по смыслу функцию reverse,
 *   но работающую итеративно.
 *
 * ПРИМЕРЫ:
 *   Вход:  2 → 4 → 6 → 1
 *   Выход: 1 → 6 → 4 → 2
 ******************************************************************************/

struct node_t {
  struct node_t *next;
  int data;
};

struct node_t *reverse(struct node_t *top) {
  if (top == NULL)
    return NULL;
  if (top->next == NULL)
    return top;

  struct node_t *first  = top;
  struct node_t *second = top->next;
  struct node_t *tmp    = NULL;
  top->next             = NULL;

  while (second != NULL) {
    tmp          = second->next;
    second->next = first;
    first        = second;
    second       = tmp;
  }

  return first;
}

int main() {
  int i, ncorr, nnodes, res;
  struct node_t *top = NULL, *cur = NULL;

  res = scanf("%d", &nnodes);
  assert(res == 1);
  assert(nnodes >= 0);

  if (nnodes > 0) {
    top       = calloc(1, sizeof(struct node_t));
    top->data = 0;
    cur       = top;
  }

  for (i = 1; i < nnodes; ++i) {
    cur->next       = calloc(1, sizeof(struct node_t));
    cur->next->data = cur->data + 1;
    cur             = cur->next;
  }

  ncorr = nnodes;
  top   = reverse(top);
  while (top && top->next) {
    if (top->data <= top->next->data) {
      ncorr -= 1;
      printf("Wrong: %d\n", top->data);
    }
    top = top->next;
  }
  printf("%d\n", ncorr);
  return EXIT_SUCCESS;
}