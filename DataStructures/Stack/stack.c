#include "stack.h"

#include <stdlib.h>

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

void stack_push(stack_t **root, void *data) {
  stack_t *new_node = (stack_t *)calloc(1, sizeof(stack_t));
  if (new_node) {
    new_node->data = data;
    new_node->next = *root;
    *root          = new_node;
  }
}

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

int stack_empty(stack_t const *root) {
  return root == NULL;
}