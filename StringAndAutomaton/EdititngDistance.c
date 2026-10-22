#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem ED — расстояние редактирования
 *
 * ОПИСАНИЕ:
 *   Вам необходимо написать программу, которая, считывая со стандартного ввода:
 *       - стоимость добавления
 *       - стоимость удаления
 *       - стоимость замены
 *       - длину первой строки
 *       - первую строку
 *       - длину второй строки
 *       - вторую строку
 *
 * Выводит на стандартный вывод минимальное расстояние редактирования
 *(последовательности операций добавления, удалениия и вставки).
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: int стоимость добавления
 *   - [параметр2]: int стоимость удаления
 *   - [параметр3]: int стоимость замены
 *   - [параметр4]: int длина первой строки
 *   - [параметр5]: char [] первая строка
 *   - [параметр6]: int длина второй строки
 *   - [параметр7]: char [] вторая строка
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: int минимальное расстояние редактирования
 *
 * ПРИМЕРЫ:
 *   Вход: 1 1 2 5 spoon 6 sponge
 *   Выход: 3 (spoon -> spon (+1) -> spong (+1) -> sponge (+1))
 ******************************************************************************/

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MIN3(a, b, c) MIN2(MIN2(a, b), c)

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
 * @brief Рекурсивное вычисление расстояния редактирования
 *
 * @param c_ins  стоимость вставки
 * @param c_del  стоимость удаления
 * @param c_rep  стоимость замены
 * @param S1     первая строка
 * @param S2     вторая строка
 * @param i      текущая длина префикса S1
 * @param j      текущая длина префикса S2
 * @param V      таблица мемоизации размером (|S1|+1) x (|S2|+1)
 * @return int   минимальная стоимость редактирования
 */
static int edit_dist(int c_ins, int c_del, int c_rep, const char *S1, const char *S2, int i, int j,
                     int **V) {
  if (i == 0)
    return j * c_ins;
  if (j == 0)
    return i * c_del;

  if (V[i][j] != -1)
    return V[i][j];

  int rep_cost = (S1[i - 1] == S2[j - 1]) ? 0 : c_rep;

  int del_val = edit_dist(c_ins, c_del, c_rep, S1, S2, i - 1, j, V) + c_del;
  int ins_val = edit_dist(c_ins, c_del, c_rep, S1, S2, i, j - 1, V) + c_ins;
  int rep_val = edit_dist(c_ins, c_del, c_rep, S1, S2, i - 1, j - 1, V) + rep_cost;

  V[i][j] = MIN3(del_val, ins_val, rep_val);
  return V[i][j];
}

int main() {
  int c_ins, c_del, c_rep;
  if (scanf("%d %d %d", &c_ins, &c_del, &c_rep) != 3)
    return EXIT_FAILURE;

  int len1;
  if (scanf("%d", &len1) != 1)
    return EXIT_FAILURE;
  char S1[len1 + 1];
  if (scanf("%s", S1) != 1)
    return EXIT_FAILURE;

  int len2;
  if (scanf("%d", &len2) != 1)
    return EXIT_FAILURE;
  char S2[len2 + 1];
  if (scanf("%s", S2) != 1)
    return EXIT_FAILURE;

  int **V = make_matrix(len1 + 1, len2 + 1);
  int ans = edit_dist(c_ins, c_del, c_rep, S1, S2, len1, len2, V);
  free_matrix(V);

  printf("%d\n", ans);
  return EXIT_SUCCESS;
}
