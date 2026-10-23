#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem SU — поиск в строке независимо от регистра
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать функцию strstrci с наперёд заданным прототипом,
 *   которая ищет подстроку в строке, независимо от регистра символов (ci означает
 *   case insensitive).
 *
 *   char * strstrci(char const * needle, char const * haystack);
 ******************************************************************************/

char *strstrci(char const *needle, char const *haystack) {
  size_t needle_len = strlen(needle), haystack_len = strlen(haystack);
  if (needle_len == 0 || needle_len > haystack_len) {
    return NULL;
  }

  size_t i = 0;
  while (i < haystack_len) {
    bool is_hit = true;
    for (size_t j = 0; j < needle_len; j++) {
      if (i + j >= haystack_len || tolower(haystack[i + j]) != tolower(needle[j])) {
        is_hit = false;
        break;
      }
    }
    if (is_hit) {
      return (char *)(haystack + i);
    }
    i++;
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
    res = scanf("%c", text + i);
    assert(res == 1);
  }

  return text;
}

int main() {
  int nlen, hlen;
  char *needle, *haystack, *sres;

  haystack = read_word(&hlen);
  needle   = read_word(&nlen);

  sres = strstrci(needle, haystack);

  printf("%s\n", sres);

  free(haystack);
  free(needle);

  return EXIT_SUCCESS;
}