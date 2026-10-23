#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem IPO — preorder traversal
 *
 * ОПИСАНИЕ:
 *   Ваша задача — написать функцию, которая:
 *   - Принимает указатель на вершину дерева
 *   - Печатает на stdout её preorder обход
 *
 *      void print_pre(struct tree_t *top) {
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
 *   Вход:  8
 *          1 1 1 0 0 0 1 1 1 0 0 1 0 0 1 0
 *          4 2 1 9 6 5 8 10
 *   Выход: 4 2 1 9 6 5 8 10
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

void print_pre(struct tree_t *top) {
  if (!top) {
    return;
  }

  printf("%d ", top->data);
  print_pre(top->left);
  print_pre(top->right);
}

int main() {
  struct tree_t *top;
  struct tree_file_t filetree;

  // read tree from file
  filetree = readfile(stdin);

  // recover tree
  top = recover(filetree);

  // print preorder
  print_pre(top);
  printf("\n");

  // free everything
  free_tree(top);
  free_temp(filetree);

  return EXIT_SUCCESS;
}