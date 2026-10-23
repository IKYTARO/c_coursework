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

static unsigned hash_djb2(char const *s) {
  unsigned hash = 5381;
  int symbol;

  while ((symbol = *s++)) {
    hash = ((hash << 5) + hash) + symbol;
  }
  return hash;
}

struct item {
  char *key;
  int value;
  struct item *next;
};

struct item *make_item(const char *key, int value) {
  struct item *item = (struct item *)calloc(1, sizeof(struct item));
  if (!item) {
    return NULL;
  }

  item->key = strdup(key);
  if (!item->key) {
    free(item);
    return NULL;
  }

  item->value = value;
  item->next  = NULL;
  return item;
}

struct hash_table {
  struct item **items;
  unsigned size;
  unsigned capacity;
};

static size_t get_index(struct hash_table *table, char const *key) {
  unsigned hash = hash_djb2(key);
  return hash & (table->capacity - 1);
}

static unsigned round_capacity(unsigned capacity) {
  if (capacity == 0) {
    return 16;
  }
  if ((capacity & (capacity - 1)) != 0) {
    capacity--;
    capacity |= capacity >> 1;
    capacity |= capacity >> 2;
    capacity |= capacity >> 4;
    capacity |= capacity >> 8;
    capacity |= capacity >> 16;
    capacity++;
  }
  return capacity;
}

struct hash_table *make_hash_table(unsigned capacity) {
  struct hash_table *hash_table = (struct hash_table *)calloc(1, sizeof(struct hash_table));
  if (!hash_table) {
    return NULL;
  }

  hash_table->items = (struct item **)calloc(capacity, sizeof(struct item *));
  if (!hash_table->items) {
    free(hash_table);
    return NULL;
  }

  hash_table->capacity = round_capacity(capacity);
  hash_table->size     = 0;
  return hash_table;
}

void destroy_hash_table(struct hash_table *table) {
  for (unsigned i = 0; i < table->capacity; i++) {
    struct item *curr_item = table->items[i];
    while (curr_item) {
      struct item *next_item = curr_item->next;
      free(curr_item->key);
      free(curr_item);
      curr_item = next_item;
    }
  }
  free(table->items);
  free(table);
}

int hash_table_add(struct hash_table *table, const char *key, int value) {
  if (!table || !key) {
    return 0;
  }

  size_t key_index       = get_index(table, key);
  struct item *curr_item = table->items[key_index];

  while (curr_item) {
    if (strcmp(curr_item->key, key) == 0) {
      curr_item->value = value;
      return 1;
    }
    curr_item = curr_item->next;
  }

  struct item *item = make_item(key, value);
  if (!item) {
    return 0;
  }

  item->next              = table->items[key_index];
  table->items[key_index] = item;
  table->size++;

  return 1;
}

void hash_table_update(struct hash_table *table, const char *key) {
  if (!table || !key) {
    return;
  }

  size_t key_index       = get_index(table, key);
  struct item *curr_item = table->items[key_index];

  while (curr_item) {
    if (strcmp(curr_item->key, key) == 0) {
      curr_item->value++;
      return;
    }
    curr_item = curr_item->next;
  }

  struct item *item = make_item(key, 1);
  if (!item) {
    return;
  }

  item->next              = table->items[key_index];
  table->items[key_index] = item;
  table->size++;

  return;
}

int hash_table_get(struct hash_table *table, const char *key) {
  if (!table || !key) {
    return 0;
  }

  size_t key_index       = get_index(table, key);
  struct item *curr_item = table->items[key_index];

  while (curr_item) {
    if (strcmp(curr_item->key, key) == 0) {
      return curr_item->value;
    }
    curr_item = curr_item->next;
  }

  return 0;
}

void hash_table_remove(struct hash_table *table, const char *key) {
  if (!table || !key) {
    return;
  }

  size_t key_index       = get_index(table, key);
  struct item *curr_item = table->items[key_index];
  struct item *prev_item = NULL;

  while (curr_item) {
    if (strcmp(curr_item->key, key) == 0) {
      if (!prev_item) {
        table->items[key_index] = curr_item->next;
      } else {
        prev_item->next = curr_item->next;
      }
      free(curr_item->key);
      free(curr_item);
      table->size--;
      return;
    }
    prev_item = curr_item;
    curr_item = curr_item->next;
  }
}

size_t hash_table_size(struct hash_table *table) {
  return table->size;
}

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