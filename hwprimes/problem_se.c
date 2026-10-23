#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem SE -- количество простых чисел
 *
 * ОПИСАНИЕ:
 *   На стандартном вводе дано число n.
 *   Вам надо вывести на стандартный вывод количество простых чисел меньших или равных n.
 *
 * ПРИМЕРЫ:
 *   Вход: 13
 *   Выход: 6
 *
 *   Вход: 1000000
 *   Выход: 78498
 ******************************************************************************/

struct sieve_t {
  int n;
  unsigned char *s;
};

void fill_sieve(struct sieve_t *sv) {
  for (int i = 2; i * i <= sv->n; i++)
    if (sv->s[i] == 0)
      for (int j = i * i; j <= sv->n; j += i)
        sv->s[j] = 1;
}

int prime_count(struct sieve_t *sv, int n) {
  assert(n <= sv->n);
  int count = 0;
  for (int i = 2; i <= n; i++) {
    if (sv->s[i] == 0) {
      count++;
    }
  }

  return count;
}

int sieve_bound(int num) {
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log(dnum) + log(log(dnum)));
  return (int)round(dres);
}

int main() {
  int n, outp;
  int res;
  struct sieve_t *s;

  res = scanf("%d", &n);
  assert(res == 1);
  s    = (struct sieve_t *)malloc(sizeof(struct sieve_t));
  s->n = sieve_bound(n);
  s->s = (unsigned char *)calloc(s->n, sizeof(unsigned char));

  fill_sieve(s);
  outp = prime_count(s, n);
  printf("%d\n", outp);

  free(s->s);
  free(s);

  return EXIT_SUCCESS;
}