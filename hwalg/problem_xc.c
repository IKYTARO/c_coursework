#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem XC — подсчёт коллизий в хеш-функции
 *
 * ОПИСАНИЕ:
 *   Ваша задача: имея произвольную функцию хеширования строки и произвольную
 *   последовательность строк, подсчитать количество коллизий.
 *
 *      typedef int (*get_hash_t)(const char *s);
 *
 *      // подсчитать количество коллизий функции f над // массивом strs
 *      int ncollisions(char **strs, int n, get_hash_t f);
 *
 *   Вы можете быть уверены, что значение хеша для каждой из строк не больше
 *   чем заданная наперед константа HASH_MAX.
 *
 * ПРИМЕРЫ:
 *   Вход:  сумма кодов символов: {"ad", "bc"}
 *   Выход:  1 (bc имеет такую же сумму кодов и таким образом является коллизией)
 ******************************************************************************/

#define HASH_MAX 13

typedef int (*get_hash_t)(const char *s);

int compare_int(void const *lhs, void const *rhs) {
  int lhs_int = *((int *)lhs);
  int rhs_int = *((int *)rhs);
  if (lhs_int > rhs_int)
    return 1;
  if (lhs_int < rhs_int)
    return -1;
  return 0;
}

int ncollisions(char **str_arr, int size, get_hash_t get_hash) {
  int *hash_arr = (int *)calloc(size, sizeof(int));
  for (int i = 0; i < size; i++) {
    hash_arr[i] = get_hash(str_arr[i]);
  }

  qsort(hash_arr, size, sizeof(int), compare_int);

  int coll_count = 0;
  for (int i = 0; i < size - 1; i++) {
    if (hash_arr[i] == hash_arr[i + 1])
      coll_count++;
  }

  free(hash_arr);
  return coll_count;
}

int myhash(const char *s) {
  int sum = 0;
  while (*s) {
    sum += *s;
    s += 1;
  }

  return (sum % HASH_MAX);
}

static char *read_word(int *len) {
  char *text;
  int res, n, i;

  res = scanf("%d", len);
  assert(res == 1);

  n = *len;
  assert(n > 0);

  text = (char *)calloc(n + 1, sizeof(char));
  assert(text != NULL);

  res = scanf("%*c");
  (void)res;

  for (i = 0; i < n; ++i) {
    char c;
    res = scanf("%c", &c);
    assert(res == 1);
    text[i] = c;
  }

  return text;
}

int main() {
  char **strs;
  int i, n, res, col;

  res = scanf("%d", &n);
  assert(res == 1);

  strs = calloc(n, sizeof(char *));
  for (i = 0; i < n; ++i) {
    int len;
    strs[i] = read_word(&len);
  }

  col = ncollisions(strs, n, myhash);

  printf("%d\n", col);

  for (i = 0; i < n; ++i)
    free(strs[i]);
  free(strs);

  return EXIT_SUCCESS;
}