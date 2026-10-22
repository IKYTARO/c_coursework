#include <stdlib.h>

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