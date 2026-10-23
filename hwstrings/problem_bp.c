#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem BP — упаковка рюкзака
 *
 * ОПИСАНИЕ:
 *   Вам необходимо написать программу, которая, считывая со стандартного ввода:
 *       - Общий вес входящий в рюкзак
 *       - Количество вещей
 *       - Вес каждой вещи
 *
 *   Подсчитает наибольшее количество вещей которые можно положить в рюкзак
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: int Общий вес входящий в рюкзак
 *   - [параметр2]: int Количество вещей
 *   - [параметр3]: int [] Вес каждой вещи
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: int Наибольшее количество вещей которые можно положить в
 *                       рюкзак
 *
 * ПРИМЕРЫ:
 *   Вход: 10 4 3 1 3 4
 *   Выход: 3
 *
 *   Вход: 15 4 3 1 3 4
 *   Выход: 4
 *
 *   Вход: 2478 9 112 196 956 1106 1515 1794 1 1799 79
 *   Выход: 6
 ******************************************************************************/

/**
 * @brief Поиск максимума двух чисел
 *
 * @param a - левый операнд
 * @param b - правый операнд
 * @return int максимум из a и b
 */
static int max(int a, int b) {
  return (a > b) ? a : b;
}

/**
 * @brief Создание двумерной матриццы и заполнение ее "-1"
 *
 * @param row_count - количество строк
 * @param col_count - количество столбцов
 * @return int** - матрица
 */
int **make_matrix(int row_count, int col_count) {
  int **matrix = (int **)malloc(row_count * sizeof(int *) + (row_count * col_count) * sizeof(int));
  if (!matrix) {
    perror("make_matrix: malloc failed");
    exit(EXIT_FAILURE);
  }

  int *data_start = (int *)(matrix + row_count);
  for (int row = 0; row < row_count; row++) {
    matrix[row] = data_start + row * col_count;
  }

  memset(data_start, -1, (row_count * col_count) * sizeof(int));

  return matrix;
}

/**
 * @brief Удаление матрицы
 *
 * @param matrix - исходная матрица
 */
void free_matrix(int **matrix) {
  free(matrix);
}

/**
 * @brief Рекурсивный алгоритм укладки рюкзака
 *
 * @param i - индекс рассматриваемого предмета (0..N)
 * @param w - оставшаяся вместимость рюкзака
 * @param V - таблица мемоизации размерностью (N+1) x (W+1)
 * @param N - общее количество предметов
 * @param weights - массив весов предметов
 * @return int - максимальное количество предметов для состояния (i, w)
 */
static int knapsack(int i, int w, int **V, int N, int *weights) {
  if (i == N)
    return 0;
  if (V[i][w] != -1)
    return V[i][w];

  int decision = knapsack(i + 1, w, V, N, weights);

  if (w >= weights[i]) {
    int take_case = knapsack(i + 1, w - weights[i], V, N, weights) + 1;
    decision      = max(decision, take_case);
  }

  V[i][w] = decision;
  return V[i][w];
}

/**
 * @brief Решение задачи об укладке рюкзака
 *
 * @param N - общее количество предметов
 * @param W - вместимость рюкзака
 * @param weights - массив весов предметов
 * @return int - максимальное количество предметов, которое можно положить в
 * рюкзак
 */
static int solve_knapsack(int N, int W, int *weights) {
  int **V    = make_matrix(N, W + 1);
  int result = knapsack(0, W, V, N, weights);
  free_matrix(V);

  return result;
}

int main() {
  int W, N;

  if (scanf("%d %d", &W, &N) != 2) {
    fprintf(stderr, "Error reading W and N.\n");
    return EXIT_FAILURE;
  }

  int *weights = malloc(N * sizeof(int));
  if (!weights) {
    perror("main: malloc failed");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < N; ++i) {
    if (scanf("%d", &weights[i]) != 1) {
      fprintf(stderr, "Error reading weight %d.\n", i);
      free(weights);
      return EXIT_FAILURE;
    }
  }

  printf("%d\n", solve_knapsack(N, W, weights));

  free(weights);
  return EXIT_SUCCESS;
}