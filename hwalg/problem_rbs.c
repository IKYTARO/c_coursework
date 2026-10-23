#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem RBS — корзинная сортировка
 *
 * ОПИСАНИЕ:
 *   Сформируем бакеты разделив максимальное число M на N частей.
 *   На вход приходит длина массива и далее сам массив.
 *   Задача: распечатать через пробелы все бакеты, разделив их нулевыми символами (ноль в конце
 *   каждого бакета).
 *
 * ПРИМЕРЫ:
 *   Вход:  8 126 348 343 432 316 171 556 670
 *   Выход: 0 126 0 171 0 316 0 343 348 0 432 0 556 0 670 0
 *
 *   Вход:  10 187 329 731 517 71 468 429 237 621 860
 *   Выход: 71 0 0 187 237 0 329 0 429 0 468 0 517 0 621 0 731 0 860 0
 ******************************************************************************/

struct node_t {
  struct node_t *next;
  int data;
};

void print_list(struct node_t const *list);
void sorted_push(struct node_t **list, int value);
void free_list(struct node_t *list);

int input_size(size_t *size);
int input_array(int *array, size_t size);
int get_max(int *array, size_t size);

int main() {
  size_t arr_size = 0;
  if (!input_size(&arr_size)) {
    perror("Invalid argument: array size");
    abort();
  }

  int *array = (int *)calloc(arr_size, sizeof(int));
  if (!input_array(array, arr_size)) {
    free(array);
    perror("Invalid argument: array data");
    abort();
  }

  int max_elem            = get_max(array, arr_size);
  int backet_step         = (int)round(max_elem / arr_size);
  struct node_t **backets = (struct node_t **)calloc(arr_size, sizeof(struct node_t *));
  for (size_t i = 0; i < arr_size; i++) {
    int backet_id = (array[i] == max_elem) ? (int)arr_size - 1 : array[i] / backet_step;
    sorted_push(&backets[backet_id], array[i]);
  }

  for (size_t i = 0; i < arr_size; i++) {
    print_list(backets[i]);
    printf("%d ", 0);
    free_list(backets[i]);
  }
  printf("\n");

  free(backets);
  free(array);
  return EXIT_SUCCESS;
}

int input_size(size_t *size) {
  return scanf("%zu", size) == 1;
}

int input_array(int *array, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (scanf("%d", array + i) != 1)
      return 0;
  }
  return 1;
}

int get_max(int *array, size_t size) {
  int max = array[0];
  for (size_t i = 1; i < size; i++)
    if (array[i] > max)
      max = array[i];
  return max;
}

void sorted_push(struct node_t **list, int value) {
  struct node_t *new_node = (struct node_t *)calloc(1, sizeof(struct node_t));
  new_node->data          = value;

  if (*list == NULL || (*list)->data >= value) {
    new_node->next = *list;
    *list          = new_node;
  } else {
    struct node_t *curr_node = *list;
    while (curr_node->next && curr_node->next->data < value)
      curr_node = curr_node->next;

    new_node->next  = curr_node->next;
    curr_node->next = new_node;
  }
}

void print_list(struct node_t const *list) {
  struct node_t const *elem;
  for (elem = list; elem != NULL; elem = elem->next)
    printf("%d ", elem->data);
}

void free_list(struct node_t *list) {
  struct node_t *elem, *tmp;
  for (elem = list; elem != NULL;) {
    tmp = elem->next;
    free(elem);
    elem = tmp;
  }
}