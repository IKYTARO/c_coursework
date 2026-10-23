#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem KC — алгоритм Рабина-Карпа с учётом коллизий
 *
 * ОПИСАНИЕ:
 *   Считайте что где-то для вас уже написаны следующие функции
 *
 *      // подсчитать хеш строки от start до fin
 *      int get_hash(const char *start, const char *fin);

 *      // обновить хеш current, удалив из него cprev и добавив в него cnext
 *      // здесь n это pow_mod(R, right - 1, Q)
 *      // возвращает новый хеш
 *      int update_hash(int current, int n, char cprev, char cnext);
 *
 *      // raise n to power k modulo m
 *      unsigned long long pow_mod (unsigned n, unsigned k, unsigned m);
 *
 *   Считайте что для вас также уже объявлены константы R и Q.
 *   Алгоритм Рабина-Карпа без учёта коллизий выглядит следующим образом
 *
 *      // или -1 если ничего не найдено
 *      int rabin_karp(const char *needle, const char *haystack) {
 *          unsigned n, target, cur, count = 0, left = 0, right = strlen(needle);
 *          target = get_hash(needle, needle + right);
 *          cur = get_hash(haystack, haystack + right);
 *          n = pow_mod(R, right - 1, Q);
 *          while(target != cur && haystack[right] != 0) {
 *          cur = update_hash(cur, n, haystack[left], haystack[right]);
 *          left += 1;
 *          right += 1;
 *          }
 *          return (target == cur) ? left : -1;
 *      }
 *   Вам нужно изменить и отправить на проверку алгоритм Рабина-Карпа, учтя в нём
 *   возможные коллизии.
 ******************************************************************************/

const unsigned R = 10u;
const unsigned Q = 31u;

// say hash(c1, c2, c3) = c1*r^2 + c2*r + c3
//                      = ((c1 * r + c2) * r) + c3
unsigned get_hash(const char *pat, const char *end) {
  unsigned p = 0;
  for (; pat != end; ++pat)
    p = (p * R + *pat) % Q;
  return p;
}

// (c1*r^2 + c2*r + c3 - c1*r^2) * r + c4
unsigned update_hash(unsigned hash, unsigned n, char cf, char cl) {
  unsigned nf = (cf * n) % Q;
  if (hash < nf)
    hash += Q;
  hash = ((hash - nf) * R + cl) % Q;
  return hash;
}

unsigned long long pow_mod(unsigned n, unsigned k, unsigned m) {
  unsigned long long mult, prod;
  if (n == 0 || n == 1 || k == 1)
    return n;
  if (k == 0)
    return 1;
  mult = n;
  prod = 1;
  while (k > 0) {
    if ((k % 2) == 1)
      prod = (prod * mult) % m;
    mult = (mult * mult) % m;
    k    = k / 2;
  }
  return prod;
}

int rabin_karp(const char *needle, const char *haystack) {
  unsigned n, target, current, left = 0, right = strlen(needle);

  target  = get_hash(needle, needle + right);
  current = get_hash(haystack, haystack + right);
  n       = pow_mod(R, right - 1, Q);

  while (target != current && haystack[right] != 0) {
    current = update_hash(current, n, haystack[left], haystack[right]);
    left++;
    right++;
  }

  return (target == current) ? left : 0;
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
  int nlen, hlen, count = 0;
  char *needle, *haystack, *sres;

  haystack = read_word(&hlen);
  needle   = read_word(&nlen);

  sres = haystack;
  for (;;) {
    int pos = rabin_karp(needle, sres);
    if (pos == -1)
      break;
    count += 1;
    sres += (pos + nlen);
  }

  printf("%d\n", count);

  free(haystack);
  free(needle);
  return EXIT_SUCCESS;
}