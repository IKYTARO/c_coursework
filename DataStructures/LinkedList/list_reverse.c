#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct node_t {
  struct node_t *next;
  int data;
};

// Выводим список на стандартный вывод.
void print_list(struct node_t const *list) {
  struct node_t const *elem;
  for (elem = list; elem != NULL; elem = elem->next)
    printf("%d ", elem->data);
  printf("\n");
}

// Удаляет список элемент за элементом.
void free_list(struct node_t *list) {
  struct node_t *elem, *tmp;
  for (elem = list; elem != NULL;) {
    tmp = elem->next;
    free(elem);
    elem = tmp;
  }
}

// Рекурсивный разворот списка
struct node_t *reverse(struct node_t *list) {
  if (list == NULL)
    return NULL;
  if (list->next == NULL)
    return list;

  struct node_t *xs = reverse(list->next);
  list->next->next  = list;
  list->next        = NULL;

  return xs;
}

// Итеративный разворот списка.
struct node_t *iterative_reverse(struct node_t *list) {
  if (list == NULL)
    return NULL;
  if (list->next == NULL)
    return list;

  struct node_t *first = list, *second = list->next;
  struct node_t *tmp = NULL;
  list->next         = NULL;

  while (second != NULL) {
    tmp          = second->next;
    second->next = first;
    first        = second;
    second       = tmp;
  }

  return first;
}

int main() {
  struct node_t *list = read_list(stdin);
  print_list(list);
  free_list(list);

  return 0;
}
