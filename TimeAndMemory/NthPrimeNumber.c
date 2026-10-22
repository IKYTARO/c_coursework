#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Вам заранее задан тип данных решета
    struct sieve_t {
    int n;
    char *s;
    };
    Ваша задача написать две функции с заданными наперёд именами и прототипами: fill_sieve и
   nth_prime

    void fill_sieve(struct sieve_t *sv);
    Функция fill_sieve заполняет решето, при этом инверсный признак хранится в каждом байте, то есть
   после работы этой функции sv->s[2] == 0, sv->s[3] == 0, sv->s[4] == 1 и т.д.

    Считайте, что на входе sv->n уже выставлено и sv->s уже аллоцировано не менее, чем в sv->n байт
   и инициализировано нулями. Выделения памяти в этой функции не происходит

    int nth_prime(struct sieve_t *sv, int N);
    Функция nth_prime ищет N-е простое число (первым является 2, шестым 13) используя решето s

    Предполагается, что решето корректно заполнено как минимум до N(logN + loglogN) предыдущей
   функцией
*/

struct sieve_t {
  int n;
  char *s;
};

void fill_sieve(struct sieve_t *sv) {
  for (int i = 2; i * i <= sv->n; i++)
    if (sv->s[i] == 0)
      for (int j = i * i; j <= sv->n; j += i)
        sv->s[j] = 1;
}

/*
    N-е простое число
*/
int nth_prime(struct sieve_t *sv, int N) {
  int cnt = 0, index = 2;
  while (cnt < N) {
    if (sv->s[index] == 0)
      cnt++;
    index++;
  }

  return index - 1;
}

/*
    Количество простых чисел меньших или равных n.
*/
int prime_count(struct sieve_t *sv, int n) {
  assert(n <= sv->n);
  int count = 0;
  for (int i = 2; i <= n; i++) {
    if (sv->s[i] == 0)
      count++;
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
  s->s = (char *)calloc(s->n, sizeof(char));

  fill_sieve(s);
  outp = nth_prime(s, n);
  printf("%d\n", outp);

  free(s->s);
  free(s);

  return 0;
}
