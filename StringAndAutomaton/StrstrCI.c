#include <ctype.h>
#include <stdbool.h>
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
      return haystack + i;
    }
    i++;
  }

  return NULL;
}