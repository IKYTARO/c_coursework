#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct item {
  char *key;
  int value;
  struct item *next;
};

struct item *make_item(const char *key, int value);

struct hash_table {
  struct item **items;
  unsigned size;
  unsigned capacity;
};

struct hash_table *make_hash_table(unsigned capacity);
void destroy_hash_table(struct hash_table *table);

int hash_table_add(struct hash_table *table, const char *key, int value);
int hash_table_get(struct hash_table *table, const char *key);
void hash_table_remove(struct hash_table *table, const char *key);

void hash_table_update(struct hash_table *table, const char *key);

size_t hash_table_size(struct hash_table *table);

#endif  // HASH_TABLE_H