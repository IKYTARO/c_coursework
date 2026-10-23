#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem TT — дерево из обходов
 *
 * ОПИСАНИЕ:
 *   Задача — написать программу, которая принимает со stdin количество вершин,
 *   а потом preorder и inorder обходы дерева. Печатает на stdout postorder обход
 *   дерева.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - n: int Количество вершин дерева
 *   - num1, num2, ...: int preorder обход дерева
 *   - num1, num2, ...: int inorder  обход дерева
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - num1, num2, ...: int postorder обход дерева
 *
 * ПРИМЕРЫ:
 *   Вход: 5 4 3 1 0 2 1 3 4 2 0
 *   Выход: 1 3 2 0 4
 *
 *   Вход: 8 4 2 1 8 6 5 10 9 1 2 4 5 6 10 8 9
 *   Выход: 1 2 5 10 6 9 8 4
 ******************************************************************************/

int *read_array(int size) {
  int *traversal = (int *)calloc(size, sizeof(int));
  if (!traversal) {
    return 0;
  }

  for (int i = 0; i < size; i++) {
    if (scanf("%d", traversal + i) != 1) {
      free(traversal);
      return NULL;
    }
  }

  return traversal;
}

int inorder_root_index(int const *inorder, int in_start, int in_stop, int root_value) {
  for (int index = in_start; index < in_stop; index++) {
    if (inorder[index] == root_value) {
      return index;
    }
  }
  return 0;
}

void print_postorder(int const *preorder, int const *inorder, int in_start, int in_stop,
                     int *pre_index) {
  if (in_start >= in_stop) {
    return;
  }

  int root = preorder[*pre_index];
  (*pre_index)++;

  int root_id = inorder_root_index(inorder, in_start, in_stop, root);

  print_postorder(preorder, inorder, in_start, root_id, pre_index);
  print_postorder(preorder, inorder, root_id + 1, in_stop, pre_index);
  printf("%d ", root);
}

int main() {
  int n;
  if (scanf("%d", &n) != 1) {
    abort();
  }

  int *preorder = read_array(n);
  if (!preorder) {
    abort();
  }

  int *inorder = read_array(n);
  if (!inorder) {
    abort();
  }

  int preorder_index = 0;
  print_postorder(preorder, inorder, 0, n, &preorder_index);

  free(inorder);
  free(preorder);
  return EXIT_SUCCESS;
}