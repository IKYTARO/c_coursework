#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem IS — проверка поисковости
 *
 * ОПИСАНИЕ:
 *   Ваша задача — написать функцию, которая:
 *   - Принимает указатель на вершину дерева
 *   - Печатает 1 на stdout, если дерево поисковое и 0, если нет.
 *
 *      int check_is_bst(struct tree_t *top) {
 *          // TODO: ваш код здесь
 *      }
 *
 *   Считайте, что дерево задано следующей структурой
 *
 *      struct tree_t {
 *          struct tree_t *left;
 *          struct tree_t *right;
 *          int data;
 *      };
 *
 *   Изначально дерево задано в формате: количество вершин + топология + данные,
 *   где топология это правильное скобочное выражение, но код считывания,
 *   реконструкции и освобождения дерева уже написан за вас.
 *
 * ПРИМЕРЫ:
 *   Вход:  8 1 1 1 0 0 0 1 1 1 0 0 1 0 0 1 0 4 2 1 9 6 5 8 10
 *   Выход: 0
 *
 *   Вход:  8 1 1 1 0 0 0 1 1 1 0 0 1 0 0 1 0 4 2 1 8 6 5 10 9
 *   Выход: 1
 ******************************************************************************/

struct tree_t {
  struct tree_t *left;
  struct tree_t *right;
  int data;
};

struct tree_file_t {
  int n;
  int *topology;  // 2 * n
  int *values;    // n
};

struct tree_file_t readfile(FILE *f) {
  struct tree_file_t ret;
  int res, i;

  res = fscanf(f, "%d", &ret.n);
  assert(res == 1);

  ret.topology = calloc(2 * ret.n, sizeof(int));
  ret.values   = calloc(ret.n, sizeof(int));

  for (i = 0; i < 2 * ret.n; ++i) {
    res = fscanf(f, "%d", ret.topology + i);
    assert(res == 1);
  }

  for (i = 0; i < ret.n; ++i) {
    res = fscanf(f, "%d", ret.values + i);
    assert(res == 1);
  }

  return ret;
}

void free_temp(struct tree_file_t data) {
  free(data.topology);
  free(data.values);
}

void free_tree(struct tree_t *top) {
  if (top == NULL)
    return;
  free_tree(top->left);
  free_tree(top->right);
  free(top);
}

struct tree_t *create_node(int data, struct tree_t *l, struct tree_t *r) {
  struct tree_t *ret = malloc(sizeof(struct tree_t));
  assert(ret);

  ret->left  = l;
  ret->right = r;
  ret->data  = data;
  return ret;
}

struct tree_t *do_recover(struct tree_file_t data, int *cursor, int *curval) {
  struct tree_t *n = NULL;
  int cur          = *cursor;
  int curv         = *curval;
  *cursor += 1;
  if (cur < (data.n * 2) && data.topology[cur] == 1) {
    struct tree_t *l, *r;
    *curval += 1;
    l = do_recover(data, cursor, curval);
    r = do_recover(data, cursor, curval);
    assert(curv < data.n);
    n = create_node(data.values[curv], l, r);
  }
  return n;
}

struct tree_t *recover(struct tree_file_t data) {
  int cursor = 0;
  int curval = 0;
  return do_recover(data, &cursor, &curval);
}

/**
 * @brief Узел стека
 */
typedef struct stacknode {
  struct stacknode *next; /**< Следующий узел в стеке */
  void *data;             /**< Данные узла стека */
} stack_t;

/**
 * @brief Добавляет элемент на вершину стека
 * @param root Указатель на вершину стека
 * @param data Данные для добавления
 */
void stack_push(stack_t **root, void *data) {
  stack_t *new_node = (stack_t *)calloc(1, sizeof(stack_t));
  if (new_node) {
    new_node->data = data;
    new_node->next = *root;
    *root          = new_node;
  }
}

/**
 * @brief Удаляет и возвращает элемент с вершины стека
 * @param root Указатель на вершину стека
 * @return Данные удаленного элемента или NULL если стек пуст
 */
void *stack_pop(stack_t **root) {
  void *data = NULL;
  if (root && *root) {
    data              = (*root)->data;
    stack_t *new_root = (*root)->next;
    free(*root);
    *root = new_root;
  }
  return data;
}

/**
 * @brief Проверяет, пуст ли стек
 * @param root Вершина стека
 * @return int
 * 1 если стек пуст,
 * 0 в противном случае
 */
int stack_empty(stack_t const *root) {
  return root == NULL;
}

/**
 * @brief Освобождает память, занятую стеком
 * @param stack Вершина стека
 * @note Не освобождает данные, хранящиеся в узлах стека
 */
void stack_free(stack_t *stack) {
  if (stack) {
    stack_t *curr_node = stack;
    while (curr_node) {
      stack_t *next_node = curr_node->next;
      free(curr_node);
      curr_node = next_node;
    }
  }
}

int check_is_bst(struct tree_t *top) {
  if (!top) {
    return 1;
  }

  stack_t *stack          = NULL;
  struct tree_t *current  = top;
  struct tree_t *previous = NULL;

  while (current || !stack_empty(stack)) {
    if (current != NULL) {
      stack_push(&stack, (void *)current);
      current = current->left;
    } else {
      current = (struct tree_t *)stack_pop(&stack);
      if (previous && previous->data >= current->data) {
        stack_free(stack);
        return 0;
      }
      previous = current;
      current  = current->right;
    }
  }

  return 1;
}

int main() {
  int is_search;
  struct tree_t *top;
  struct tree_file_t filetree;

  // read tree from file
  filetree = readfile(stdin);

  // recover tree
  top = recover(filetree);

  // check if search
  is_search = check_is_bst(top);

  printf("%d\n", is_search);

  // free everything
  free_tree(top);
  free_temp(filetree);

  return EXIT_SUCCESS;
}