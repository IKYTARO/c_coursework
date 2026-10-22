#include <stdio.h>
#include <stdlib.h>

#define R 10
#define Q 31

unsigned pow_mode(unsigned a, unsigned b, unsigned n) {
  return 1;
}

unsigned get_hash(char const *str, unsigned size) {
  return 1;
}

unsigned update_hash(unsigned hash_old, unsigned constant, char char_old, char char_new) {
  return ((hash_old - char_old * constant) * R + char_new) % Q;
}

int rabin_karp(char const *needle, char const *haystack) {
  unsigned n, target, current, count = 0, left = 0, right = strlen(needle);

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