/**
 * @file binary_tree.h
 * @brief Бинарное дерево с поддержкой различных алгоритмов обхода
 */

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

/**
 * @brief Узел бинарного дерева
 */
typedef struct binary_tree_node {
  struct binary_tree_node *parent; /**< Родительский узел */
  struct binary_tree_node *left;   /**< Левый дочерний узел */
  struct binary_tree_node *right;  /**< Правый дочерний узел */
  void *data;                      /**< Данные узла */
} binary_tree_t;

/**
 * @brief Тип функции обработки данных узла дерева
 * @param data Указатель на данные узла
 */
typedef void (*data_handler)(void *data);

/**
 * @brief Функция сравнения данных узлов дерева
 * @param data_lhs Данные первого узла
 * @param data_rhs Данные второго узла
 * @return Отрицательное число если data_lhs < data_rhs,
 *         0 если data_lhs == data_rhs,
 *         Положительное число если data_lhs > data_rhs
 */
typedef int (*data_comparator)(void const *data_lhs, void const *data_rhs);

/**
 * @brief Рекурсивный preorder обход дерева (корень → лево → право)
 * @param root Корень дерева
 * @param handler Функция обработки данных каждого узла
 */
void preorder_recursive(binary_tree_t *root, data_handler handler);

/**
 * @brief Итеративный preorder обход дерева (корень → лево → право)
 * @param root Корень дерева
 * @param handler Функция обработки данных каждого узла
 */
void preorder_iterative(binary_tree_t *root, data_handler handler);

/**
 * @brief Итеративный inorder обход дерева (лево → корень → право)
 * @param root Корень дерева
 * @param handler Функция обработки данных каждого узла
 */
void inorder_iterative(binary_tree_t *root, data_handler handler);

/**
 * @brief Проверяет, является ли дерево бинарным деревом поиска (BST)
 * @param top Корень дерева
 * @param cmp Функция сравнения данных
 * @return 1 если дерево является BST, 0 в противном случае
 * @note Пустое дерево считается BST
 */
int check_is_bst(binary_tree_t *top, data_comparator cmp);

#endif  // BINARY_TREE_H