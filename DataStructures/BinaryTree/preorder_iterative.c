#include <stdlib.h>

#include "../Stack/stack.h"
#include "binary_tree.h"

void preorder_iterative(binary_tree_t *root, data_handler handler) {
  if (!root)
    return;

  stack_t *stack = NULL;
  stack_push(&stack, (void *)root);

  while (!stack_empty(stack)) {
    binary_tree_t *curr_node = (binary_tree_t *)stack_pop(&stack);

    if (curr_node) {
      handler(curr_node->data);
    }

    if (curr_node->right) {
      stack_push(&stack, (void *)curr_node->right);
    }

    if (curr_node->left) {
      stack_push(&stack, (void *)curr_node->left);
    }
  }
}