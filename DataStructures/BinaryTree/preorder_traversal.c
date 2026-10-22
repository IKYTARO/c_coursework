#include "binary_tree.h"

void preorder_recursive(binary_tree_t *root, data_handler handler) {
  if (!root)
    return;

  handler(root->data);
  preorder_recursive(root->left, handler);
  preorder_recursive(root->right, handler);
}