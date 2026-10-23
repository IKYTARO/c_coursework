/**
 * @file stack.h
 * @brief Реализация стека для использования в алгоритмах обхода деревьев
 */

#ifndef STACK_H
#define STACK_H

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
void stack_push(stack_t **root, void *data);

/**
 * @brief Удаляет и возвращает элемент с вершины стека
 * @param root Указатель на вершину стека
 * @return Данные удаленного элемента или NULL если стек пуст
 */
void *stack_pop(stack_t **root);

/**
 * @brief Проверяет, пуст ли стек
 * @param root Вершина стека
 * @return int
 * 1 если стек пуст,
 * 0 в противном случае
 */
int stack_empty(stack_t const *root);

/**
 * @brief Освобождает память, занятую стеком
 * @param stack Вершина стека
 * @note Не освобождает данные, хранящиеся в узлах стека
 */
void stack_free(stack_t *stack);

#endif  // STACK_H