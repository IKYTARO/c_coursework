#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct sieve_t {
  int size;
  char *sieve;
};

int sieve_bound(int num) {
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log(dnum) + log(log(dnum)));
  return (int)round(dres);
}

int is_prime(struct sieve_t *sv, unsigned num) {
  int byte_num     = num / CHAR_BIT;
  int bit_num      = num % CHAR_BIT;
  unsigned bit_val = (sv->sieve[byte_num] >> bit_num) & 1u;

  return (bit_val == 0) ? 1 : 0;
}

void set_composite(struct sieve_t *sv, unsigned num) {
  int byte_num = num / CHAR_BIT;
  int bit_num  = num % CHAR_BIT;
  sv->sieve[byte_num] |= (1u << bit_num);
}

void fill_sieve(struct sieve_t *sv) {
  for (int i = 2; i * i <= sv->size; i++)
    if (is_prime(sv, i))
      for (int j = i * i; j <= sv->size; j += i)
        set_composite(sv, j);
}

int nth_prime(struct sieve_t *sv, int N) {
  int cnt, prime;
  if (N == 1)
    return 2;
  if (N == 2)
    return 3;

  cnt   = 2;
  prime = 5;

  for (;;) {
    if (is_prime(sv, prime)) {
      cnt += 1;
      if (cnt == N)
        break;
    }
    prime += 2;
    if (is_prime(sv, prime)) {
      cnt += 1;
      if (cnt == N)
        break;
    }
    prime += 4;
  }

  return prime;
}

int main() {
  int n, outp;
  int res;
  struct sieve_t *sv;

  res = scanf("%d", &n);
  assert(res == 1);

  sv = (struct sieve_t *)malloc(sizeof(struct sieve_t));
  assert(sv);

  sv->size  = (sieve_bound(n) / CHAR_BIT) + CHAR_BIT;
  sv->sieve = (char *)calloc(sv->size, sizeof(char));
  assert(sv->sieve);

  fill_sieve(sv);
  outp = nth_prime(sv, n);
  printf("%d\n", outp);

  free(sv->sieve);
  free(sv);

  return 0;
}
