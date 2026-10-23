#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Homework HWK — алгоритм КМП
 *
 * ОПИСАНИЕ:
 *   Написать функцию patpreproc, которая препроцессирует паттерн, чтобы далее
 *   использовать алгоритм Кнута-Морриса-Пратта.
 *
 *   Написать функцию strstrci с наперёд заданным прототипом, которая ищет подстроку
 *   в строке, независимо от регистра символов (ci означает case insensitive).
 *
 *   void patpreproc(char const * needle, int * needle_lps);
 *   char * strstrci(char const * needle, int const * needle_lps, char const *haystack);
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: int Длина строки haystack
 *   - [параметр2]: char* Строка haystack
 *   - [параметр3]: int Длина строки needle
 *   - [параметр4]: char* Строка needle
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: int Кодичество вхождений подстроки needle в строку haystack
 *
 * ПРИМЕРЫ:
 *   Вход:  205
 *          qrtfCISeEDNOBUeEDNOBUHRSqqUPZHRSqqUPZKjjQGePZARIuHQCkZHlnbbBnlcmNeEDNeEDNOBUHRSqqUPZOBUHRSqeEDNOeEDNOBUHRSqqUPZBUHRSqqUPZqUPZjjuSjILbdYAiJxbpunWVkuXeEDNOBUHRSqqUPZnltPmbCAxTkvFvYUfahQCgiQIhZFbHvINBeAfCXafa
 *          15
 *          eednobuhrsqqupz
 *   Выход: 4
 ******************************************************************************/

int is_equal_ci(char lhs, char rhs) {
  return tolower(lhs) == tolower(rhs);
}

void patpreproc(char const *needle, int *needle_lps) {
  needle_lps[0] = 0;

  int i = 1, length = 0;
  while (needle[i] != '\0') {
    if (is_equal_ci(needle[i], needle[length])) {
      length++;
      needle_lps[i] = length;
      i++;
    } else {
      if (length == 0) {
        needle_lps[i] = 0;
        i++;
      } else {
        length = needle_lps[length - 1];
      }
    }
  }
}

char *strstrci(char const *needle, int const *needle_lps, char const *haystack) {
  if (!needle || !haystack || !needle_lps) {
    return NULL;
  }

  int i = 0, j = 0;

  while (haystack[i] != '\0') {
    if (is_equal_ci(haystack[i], needle[j])) {
      i++;
      j++;

      if (needle[j] == '\0') {
        return (char *)(haystack + i - j);
      }
    } else {
      if (j != 0) {
        j = needle_lps[j - 1];
      } else {
        i++;
      }
    }
  }

  return NULL;
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
  int *needle_lps;

#if VISUALIZE
  int i;
#endif

  haystack = read_word(&hlen);
  needle   = read_word(&nlen);

  needle_lps = (int *)calloc(nlen + 1, sizeof(int));
  patpreproc(needle, needle_lps);

#if VISUALIZE
  for (i = 0; i < nlen; ++i)
    printf("%c ", needle[i]);
  printf("\n");

  for (i = 0; i < nlen; ++i)
    printf("%d ", needle_lps[i]);
  printf("\n");
#endif

  sres = haystack;
  for (;;) {
    sres = strstrci(needle, needle_lps, sres);
    if (!sres)
      break;
    count += 1;
    sres += nlen;
  }

  printf("%d\n", count);

  free(haystack);
  free(needle);
  free(needle_lps);

  return EXIT_SUCCESS;
}