#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem PN -- N-е простое число
 *
 * ОПИСАНИЕ:
 *   Первым простым числом является 2, шестым является 13.
 *   Задача заключается в том, чтобы вычислить N-e простое число.
 *   Посылка должна состоять из программы, считывающей со стандартного ввода
 *   число N и выводящей на стандартный вывод N-e простое.
 *
 * ПРИМЕРЫ:
 *   Вход: 6
 *   Выход: 13
 *
 *   Вход: 100
 *   Выход: 541
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

int nth_prime(struct sieve_t *sv, int N) {
  int cnt = 0, index = 2;
  while (cnt < N) {
    if (sv->s[index] == 0)
      cnt++;
    index++;
  }

  return index - 1;
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
  outp = nth_prime(s, n);
  printf("%d\n", outp);

  free(s->s);
  free(s);

  return EXIT_SUCCESS;
}