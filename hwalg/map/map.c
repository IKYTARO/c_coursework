#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static unsigned hash_djb2(char const *s) {
  unsigned hash = 5381;
  int symbol;

  while ((symbol = *s++)) {
    hash = ((hash << 5) + hash) + symbol;
  }
  return hash;
}

static size_t get_index(struct hash_table *table, char const *key) {
  unsigned hash = hash_djb2(key);
  return hash & (table->capacity - 1);
}

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