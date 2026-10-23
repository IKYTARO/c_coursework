#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem NO — натуральный порядок
 *
 * ОПИСАНИЕ:
 *   Задача — написать программу, которая считывает поисковое дерево из файла
 *(stdin) как количество вершин + последовательность данных и выводит preorder
 *обход дерева.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - n: unsigned Количество вершин дерева
 *   - num1, num2, ...: int последовательность вершин дерева
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - num1, num2, ...: int preorder обход дерева
 *
 * ПРИМЕРЫ:
 *   Вход: 8 4 9 10 6 5 8 2 1
 *   Выход: 4 2 1 9 6 5 8 10
 *
 *   Вход: 10 8 4 3 7 5 2 0 6 1 9
 *   Выход: 8 4 3 2 0 1 7 5 6 9
 ******************************************************************************/

typedef struct tree_node {
  struct tree_node *left;
  struct tree_node *right;
  int data;
} tree_t;

typedef struct stack_node {
  struct stack_node *next;
  tree_t *node;
} stack_t;

static void stack_push(stack_t **stack, tree_t *node) {
  stack_t *new_node = (stack_t *)malloc(sizeof(stack_t));
  if (!new_node)
    return;

  new_node->node = node;
  new_node->next = *stack;
  *stack         = new_node;
}

static tree_t *stack_pop(stack_t **stack) {
  if (!stack || !*stack)
    return NULL;

  stack_t *top = *stack;
  tree_t *node = top->node;
  *stack       = top->next;
  free(top);

  return node;
}

static int stack_empty(stack_t *stack) {
  return stack == NULL;
}

static tree_t *create_tree_node(int data) {
  tree_t *node = (tree_t *)calloc(1, sizeof(tree_t));
  if (node) {
    node->data = data;
  }
  return node;
}

static void insert_tree_node(tree_t **root, int data) {
  if (!root)
    return;

  if (!*root) {
    *root = create_tree_node(data);
    return;
  }

  tree_t *curr_node = *root;
  while (1) {
    if (data < curr_node->data) {
      if (!curr_node->left) {
        curr_node->left = create_tree_node(data);
        break;
      }
      curr_node = curr_node->left;
    } else {
      if (!curr_node->right) {
        curr_node->right = create_tree_node(data);
        break;
      }
      curr_node = curr_node->right;
    }
  }
}

static void tree_preorder_iterative(tree_t *root) {
  if (!root) {
    return;
  }

  stack_t *stack = NULL;
  stack_push(&stack, root);

  while (!stack_empty(stack)) {
    tree_t *current = stack_pop(&stack);

    printf("%d ", current->data);

    if (current->right) {
      stack_push(&stack, current->right);
    }

    if (current->left) {
      stack_push(&stack, current->left);
    }
  }
}

static void tree_free(tree_t *root) {
  if (!root)
    return;

  stack_t *stack       = NULL;
  tree_t *current      = root;
  tree_t *last_visited = NULL;

  while (current || !stack_empty(stack)) {
    if (current) {
      stack_push(&stack, current);
      current = current->left;
    } else {
      tree_t *peek = stack_empty(stack) ? NULL : stack->node;

      if (peek && peek->right && peek->right != last_visited) {
        current = peek->right;
      } else {
        last_visited = stack_pop(&stack);
        free(last_visited);
      }
    }
  }
}

static tree_t *read_bst_from_stdin(void) {
  unsigned num = 0;
  if (scanf("%u", &num) != 1) {
    return NULL;
  }

  tree_t *root = NULL;

  for (unsigned i = 0; i < num; i++) {
    int data = 0;
    if (scanf("%d", &data) != 1) {
      tree_free(root);
      return NULL;
    }
    insert_tree_node(&root, data);
  }

  return root;
}

int main() {
  tree_t *root = read_bst_from_stdin();

  if (!root) {
    return EXIT_FAILURE;
  }

  tree_preorder_iterative(root);
  tree_free(root);

  return EXIT_SUCCESS;
}