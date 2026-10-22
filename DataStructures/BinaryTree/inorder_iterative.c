#include <stdlib.h>

#include "../Stack/stack.h"
#include "binary_tree.h"

void inorder_iterative(binary_tree_t *root, data_handler handler) {
  if (!root || !handler)
    return;

  stack_t *stack           = NULL;
  binary_tree_t *curr_node = root;

  while (curr_node || !stack_empty(stack)) {
    while (curr_node) {
      stack_push(&stack, curr_node);
      curr_node = curr_node->left;
    }

    curr_node = (binary_tree_t *)stack_pop(&stack);
    if (curr_node) {
      handler(curr_node->data);
      curr_node = curr_node->right;
    }
  }
}