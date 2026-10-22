#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem SP — замена в строке
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать функцию с наперёд заданным прототипом,
 *   осуществляющую замену всех подстрок в строке.
 *
 *   char *replace(char *str, char const *from, char const *to);
 *
 *   Функция должна аллоцировать и возвращать строку со всеми вхождениями from,
 *   замененными на to.
 *
 *   Возвращаемое значение: указатель на начало полученной строки.
 *
 * ПРИМЕРЫ:
 *   Вход: "Hello, %username, how are you, %username", "%username", "Eric, the Blood Axe"
 *   Выход: "Hello, Eric, the Blood Axe, how are you, Eric, the Blood Axe"
 ******************************************************************************/

char *replace(char *str, char const *from, char const *to) {
  int str_len = (int)strlen(str), from_len = (int)strlen(from), to_len = (int)strlen(to);
  if (from_len == 0) {
    return strdup(str);
  }

  int max_hits = str_len / from_len, hits_count = 0;
  char *hit_ptrs[max_hits];

  char *hit_pos = str;
  while ((hit_pos = strstr(hit_pos, from)) != NULL) {
    hit_ptrs[hits_count] = hit_pos;
    hits_count++;
    hit_pos += from_len;
  }

  if (hits_count == 0) {
    return strdup(str);
  }

  size_t new_len = str_len + hits_count * (to_len - from_len) + 1;
  char *new_str  = (char *)calloc(new_len, sizeof(char));
  if (!new_str) {
    return NULL;
  }

  int curr_hit_id = 0, i = 0, j = 0;
  while (i < str_len) {
    if (curr_hit_id < hits_count && str + i == hit_ptrs[curr_hit_id]) {
      memcpy(new_str + j, to, to_len);
      i += from_len;
      j += to_len;
      curr_hit_id++;
    } else {
      new_str[j] = str[i];
      i++;
      j++;
    }
  }

  return new_str;
}