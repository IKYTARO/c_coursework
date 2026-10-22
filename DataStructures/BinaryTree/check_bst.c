#include <stdlib.h>

#include "../Stack/stack.h"
#include "binary_tree.h"

int check_is_bst(binary_tree_t *top, data_comparator cmp) {
  if (!top || !cmp) {
    return 1;
  }

  stack_t *stack         = NULL;
  binary_tree_t *current = top, *previous = NULL;

  while (current || !stack_empty(stack)) {
    if (current != NULL) {
      stack_push(&stack, (void *)current);
      current = current->left;
    } else {
      current = (binary_tree_t *)stack_pop(&stack);

      if (previous && cmp(previous->data, current->data) >= 0) {
        stack_free(stack);
        return 0;
      }

      previous = current;
      current  = current->right;
    }
  }

  return 1;
}