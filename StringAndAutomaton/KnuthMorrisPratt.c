#include <ctype.h>
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