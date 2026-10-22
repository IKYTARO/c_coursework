#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/******************************************************************************
 * ЗАДАЧА: Homework problem HWH — словарь
 *
 * ОПИСАНИЕ:
 *   На стандартном вводе: количество ожидаемых ответов, длина, текст,
 *разделённый пробелами, длина, список искомых слов, разделённый пробелами.
 *
 *   На стандартный вывод: сколько раз слово из списка слов встретилось в
 *тексте.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - n: [int]    Количество ожидаемых ответов
 *   - m: [int]    Длина исходного текста
 *   - t: [char *] Исходный текст, разделённый пробелами
 *   - k: [int]    Длина списка искомых слов
 *   - s: [char *] Cписок искомых слов, разделённый пробелами
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - num1, num2, ...: int Cколько раз слова из списка слов встретились в
 *тексте
 *
 * ПРИМЕРЫ:
 *   Вход: 2
 *         27
 *         abc bca abc bac cba bca abc
 *         7
 *         abc bca
 *
 *   Выход: 3 2
 ******************************************************************************/

struct problem_context {
  int answers_count;
  int text_length;
  char *text;
  int words_length;
  char *words;
};

int read_context(struct problem_context *context) {
}

void free_context(struct problem_context *context) {
  free(context->words);
  free(context->text);
}

int is_space(char c) {
  if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
    return 1;
  }
  return 0;
}

void add_word_to_table(struct hash_table *table, const char *start, char const *end) {
  size_t len = end - start;

  char buffer[256];
  strncpy(buffer, start, len);
  buffer[len] = '\0';

  hash_table_update(table, buffer);
}

void fill_hash_table(struct hash_table *table, struct problem_context const *context) {
  char const *start   = context->text;
  char const *current = start;

  while (1) {
    if (is_space(*current)) {
      if (current > start) {
        add_word_to_table(table, start, current);
      }
      start = current + 1;

      if (*current == '\0')
        break;
    }
    current++;
  }
}

int get_answer_from_table(struct hash_table *table, const char *start, char const *end) {
  size_t len = end - start;

  char buffer[256];
  strncpy(buffer, start, len);
  buffer[len] = '\0';

  return hash_table_get(table, buffer);
}

void print_answers(struct hash_table *table, struct problem_context const *context) {
  char const *start   = context->words;
  char const *current = start;

  while (1) {
    if (is_space(*current)) {
      if (current > start) {
        printf("%d ", get_answer_from_table(table, start, current));
      }
      start = current + 1;

      if (*current == '\0')
        break;
    }
    current++;
  }
}

int main() {
  struct problem_context context = {};
  if (!read_context(&context)) {
    printf("Could not read context.\n");
    return 0;
  }

  struct hash_table *hash_table = make_hash_table(1024);

  fill_hash_table(hash_table, &context);
  print_answers(hash_table, &context);

  destroy_hash_table(hash_table);
  free_context(&context);

  return EXIT_SUCCESS;
}
