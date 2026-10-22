#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem RXM — переворот подстрок с поиском регекспом
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать программу которая переворачивает в некоей строке все
 *   вхождения определённой подстроки. Например для подстроки задаваемой регулярным
 *   выражением "wo.*d" и строки "Hello, world!" результатом должно быть "Hello,
 *   dlrow!". Вы можете предполагать что текст состоит из слов, разделённых
 *   пробелами и пунктуацией, а слово состоит из алфавитных символов.
 *
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: int Длина искомой подстроки
 *   - [параметр2]: char [] Cама подстрока
 *   - [параметр3]: int Длина текста
 *   - [параметр4]: char [] Cам текст, в котором нужно сделать поиск и замену
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: char []  Текст с перевёрнутыми подстроками, либо исходный
 *                            текст, если ни одной подстроки не найдено.
 *
 * ПРИМЕРЫ:
 *   Вход: 5 wo.*d 13 Hello, world!
 *   Выход: Hello, dlrow!
 *
 *   Вход: 12 [ab][ab][bc] 51 qffUt g abcykvabcb x Lv BRabc g KUrabcN wQabcylz R
 *   Выход: qffUt g cbaykvcbab x Lv BRcba g KUrcbaN wQcbaylz R
 ******************************************************************************/

#define ensure(cond) ensure_impl((cond), __FILE__, __LINE__, #cond)

static void ensure_impl(bool cond, const char *file, int line, const char *cond_str) {
  if (!cond) {
    fprintf(stderr, "%s:%d: Condition failed: %s\n", file, line, cond_str);
    exit(EXIT_FAILURE);
  }
}

void read_string(int *str_len, char **str) {
  ensure(scanf("%d", str_len) == 1 && str_len > 0);

  ensure(isspace(getc(stdin)));

  *str = (char *)calloc(*str_len + 1, sizeof(char));
  ensure(*str != NULL);

  if (fgets(*str, *str_len + 1, stdin) == NULL) {
    fprintf(stderr, "%s:%d: Condition failed: %s\n", __FILE__, __LINE__, "fgets return not NULL");
    free(str);
    exit(EXIT_FAILURE);
  }
}

void reverse(char *src, int start, int stop) {
  char tmp;
  while (start < stop) {
    tmp        = src[start];
    src[start] = src[stop];
    src[stop]  = tmp;

    start++;
    stop--;
  }
}

void reverse_substrings(char *str, char *pattern) {
  char BUFFER[128];

  regmatch_t matches[1];
  regex_t regex;

  int res = regcomp(&regex, pattern, REG_EXTENDED);
  if (res != 0) {
    regerror(res, &regex, BUFFER, sizeof(BUFFER));
    fprintf(stderr, "Regex compilation failed: %s\n", BUFFER);
    free(str);
    free(pattern);
    exit(EXIT_FAILURE);
  }

  char *cursor = str;
  while (1) {
    res = regexec(&regex, cursor, 1, matches, 0);

    if (res == REG_NOMATCH) {
      break;
    }

    if (res != 0) {
      regerror(res, &regex, BUFFER, sizeof(BUFFER));
      fprintf(stderr, "Regex match failed: %s\n", BUFFER);
      free(str);
      free(pattern);
      regfree(&regex);
      exit(EXIT_FAILURE);
    }

    int global_start = (cursor - str) + matches[0].rm_so;
    int global_stop  = (cursor - str) + matches[0].rm_eo - 1;
    reverse(str, global_start, global_stop);

    cursor += matches[0].rm_eo;
  }

  regfree(&regex);
}

int main() {
  int pattern_len, str_len;
  char *pattern, *str;

  read_string(&pattern_len, &pattern);
  read_string(&str_len, &str);

  reverse_substrings(str, pattern);
  printf("%s\n", str);

  free(str);
  free(pattern);

  return EXIT_SUCCESS;
}