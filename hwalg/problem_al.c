#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem AL — файл в список
 *
 * ОПИСАНИЕ:
 *   Заранее задан (определять его не надо) тип структуры
 *
 *      struct node_t {
 *          struct node_t *next;
 *          int data;
 *      };
 *
 *   Вам необходимо написать две функции с заранее заданными сигнатурами
 *
 *   // считывает список из входного файла, так, чтобы все чётные числа были в начале
 *   struct node_t *read_list(FILE *inp);
 *
 *   // удаляет список элемент за элементом
 *   void delete_list(struct node_t *top);
 *
 *   Обратите внимание: все чётные числа должны быть строго в начале списка
 *   (а нечётные в конце) в том же порядке, в каком они идут в файле.
 *
 * ПРИМЕРЫ:
 *   Вход:  1 2 3 4 5 6 7
 *   Выход: 2 → 4 → 6 → 1 → 3 → 5 → 7
 *
 *   Вход:  11 3 3 1 1 5 7 3 9 1
 *   Выход: 11 → 3 → 3 → 1 → 1 → 5 → ...
 *
 *   Вход:  пустой файл
 *   Выход: NULL
 ******************************************************************************/

struct node_t {
  struct node_t *next;
  int data;
};

void print_list(struct node_t const *list) {
  struct node_t const *elem;
  for (elem = list; elem != NULL; elem = elem->next) {
    printf("%d ", elem->data);
  }
  printf("\n");
}

void delete_list(struct node_t *list) {
  struct node_t *elem, *tmp;
  for (elem = list; elem != NULL;) {
    tmp = elem->next;
    free(elem);
    elem = tmp;
  }
}

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

struct node_t *read_list(FILE *input) {
  struct node_t *even_list = NULL, *odd_list = NULL;
  struct node_t *last_odd = NULL;

  int res, value;
  for (;;) {
    res = fscanf(input, "%d", &value);
    if (res == EOF)
      break;
    if (res == -1) {
      fprintf(stderr, "File has broken format\n");
      abort();
    }
    struct node_t *new_node = (struct node_t *)calloc(1, sizeof(struct node_t));
    new_node->data          = value;
    if (value % 2 == 0) {
      new_node->next = even_list;
      even_list      = new_node;
    } else {
      if (odd_list == NULL)
        last_odd = new_node;
      new_node->next = odd_list;
      odd_list       = new_node;
    }
  }

  if (last_odd == NULL) {
    return iterative_reverse(even_list);
  }
  assert(last_odd->next == NULL);
  last_odd->next = even_list;
  return iterative_reverse(odd_list);
}

int main() {
  struct node_t *list = read_list(stdin);
  print_list(list);
  delete_list(list);

  return EXIT_SUCCESS;
}