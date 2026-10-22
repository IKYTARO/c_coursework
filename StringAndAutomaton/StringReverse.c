#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem SR — переворот подстрок
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать программу которая переворачивает в некоей строке все
 *   вхождения определённой подстроки. Вы можете предполагать что текст состоит
 *   из слов, разделённых пробелами и пунктуацией, а слово состоит из алфавитных
 *   символов.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: int Длина искомой подстроки
 *   - [параметр2]: char [] Cама подстрока
 *   - [параметр3]: int Длина текста
 *   - [параметр4]: char [] Cам текст, в котором нужно сделать поиск и замену
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: char []  Текст с перевёрнутыми подстроками, либо исходный
 *                            текст, если ни одной подстроки не найдено
 *
 * ПРИМЕРЫ:
 *   Вход: 5 world 13 Hello, world!
 *   Выход: Hello, dlrow!
 *
 *   Вход: 3 abc 51 qffUt g abcykvabcb x Lv BRabc g KUrabcN wQabcylz R
 *   Выход: qffUt g cbaykvcbab x Lv BRcba g KUrcbaN wQcbaylz R
 ******************************************************************************/

#define ensure(cond) ensure_impl((cond), __FILE__, __LINE__, #cond)

static void ensure_impl(bool cond, const char *file, int line, const char *cond_str) {
  if (!cond) {
    fprintf(stderr, "%s:%d: Condition failed: %s\n", file, line, cond_str);
    exit(1);
  }
}

void read_string(int *str_len, char **str) {
  ensure(scanf("%d", str_len) == 1 && str_len > 0);

  ensure(isspace(getc(stdin)));

  *str = (char *)calloc(*str_len + 1, sizeof(char));
  ensure(*str != NULL);
  char *res = fgets(*str, *str_len + 1, stdin);
  ensure(res != NULL);
}

void reverse_string(char *src, int start, int len) {
  int left = start, right = start + len - 1;
  char tmp;

  while (left < right) {
    tmp        = src[left];
    src[left]  = src[right];
    src[right] = tmp;

    left++;
    right--;
  }
}

int main() {
  int needle_len, haystack_len;
  char *needle, *haystack;
  read_string(&needle_len, &needle);
  read_string(&haystack_len, &haystack);

  char *needle_pos = haystack;
  while ((needle_pos = strstr(needle_pos, needle)) != NULL) {
    reverse_string(haystack, (int)(needle_pos - haystack), needle_len);
    needle_pos += needle_len;
  }

  printf("%s", haystack);

  free(needle);
  free(haystack);

  return 0;
}