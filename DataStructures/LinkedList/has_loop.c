#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct node_t {
  struct node_t *next;
  int data;
};

int has_loop(struct node_t *root) {
  struct node_t *rabbit = root;
  struct node_t *turtle = root;
  while (rabbit != NULL) {
    rabbit = rabbit->next;
    if (rabbit == NULL)
      return 0;
    if (rabbit == turtle)
      return 1;
    rabbit = rabbit->next;
    turtle = turtle->next;
    if (rabbit == turtle)
      return 1;
  }
  return 0;
}