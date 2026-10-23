#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem SA — конкатенировать с реаллокацией
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать функцию с наперёд заданным прототипом, осуществляющую
 *   конкатенацию строк с реаллокацией.
 *
 *   char *strcat_r(char *dest, const char *src, int *bufsz);
 *
 *   Функция должна дописывать src в конец dest, при необходимости реаллоцируя
 *   dest. Параметр bufsz содержит фактический размер массива для dest, который
 *   может быть куда больше, чем её null-terminated длина. Это in-out параметр, так
 *   как туда записывается новый размер. Функция должна, если буфер недостаточен,
 *   реаллоцировать его точно до размера сконкатенированной строки, делать запас
 *   для амортизации не надо.
 *
 *   Возвращаемое значение: указатель на начало сконкатенированной строки. Из-за
 *                          реаллокаций он может и не совпадать с dest.
 *
 * ПРИМЕРЫ:
 *   Вход: "Hello", 6, ", world!"
 *   Выход: "Hello, world!"
 ******************************************************************************/

char *strcat_r(char *dest, const char *src, int *bufsz) {
  int dest_len = strlen(dest), src_len = strlen(src);
  int concat_len = dest_len + src_len + 1;
  if (concat_len <= *bufsz) {
    return strcat(dest, src);
  }

  char *new_dest = realloc(dest, concat_len);
  if (!new_dest) {
    return NULL;
  }

  *bufsz = concat_len;
  dest   = new_dest;
  return strcat(dest, src);
}

int main() {
  int buflen, res, catlen;
  char *buf, *catbuf;

  res = scanf("%d", &buflen);
  assert(res == 1);

  buf = (char *)calloc(buflen + 1, sizeof(char));
  assert(buf != NULL);

  catlen = 10;
  catbuf = (char *)calloc(catlen, sizeof(char));
  assert(catbuf != NULL);

  for (;;) {
    res = scanf("%s", buf);
    if (res != 1)
      break;
    catbuf = strcat_r(catbuf, buf, &catlen);
  }

  printf("%s\n", catbuf);
  free(buf);
  free(catbuf);

  return EXIT_SUCCESS;
}