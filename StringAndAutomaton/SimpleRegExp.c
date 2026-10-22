#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem RXB — простые регулярные выражения
 *
 * ОПИСАНИЕ:
 *   Ваша задача написать программу, распознающую корректные адреса электронной
 *   почты. Адрес электронной почты имеет вид something@domain.extension Где
 *   'something' может включать разделение точками и цифры, а 'domain' и
 *   'extension' буквенные. Ваша задача отличить правильные адреса от неправильных.
 *   Для правильных выводите на stdout число 1, для неправильных 0.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [параметр1]: char [] Адрес электронной почты
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [результат1]: int Число
 *      1 - адрес корректный
 *      0 - адрес некорректный
 *
 * ПРИМЕРЫ:
 *   Вход: vasya1976@mail.ru
 *   Выход: 1
 *
 *   Вход: vasya1976@mail@ru
 *   Выход: 0
 ******************************************************************************/

char *read_email() {
  char *input_email  = NULL;
  size_t email_len   = 0;
  ssize_t read_chars = 0;

  if ((read_chars = getline(&input_email, &email_len, stdin)) != -1) {
    if (read_chars > 0 && input_email[read_chars - 1] == '\n') {
      input_email[read_chars - 1] = '\0';
    }
  }
  return input_email;
}

int compile_regex(regex_t *regex) {
  char const *pattern = "^[a-zA-Z0-9.]+@[a-zA-Z]+[.][a-zA-Z]+$";

  if (regcomp(regex, pattern, REG_EXTENDED) != 0) {
    return 0;
  }
  return 1;
}

int main() {
  char *email = read_email();
  if (!email) {
    perror("Reading from stdin error");
    abort();
  }

  regex_t regex;
  if (compile_regex(&regex) != 1) {
    free(email);
    perror("Regex compile failed");
    abort();
  }

  int res = regexec(&regex, email, 0, NULL, 0);
  if (res == 0) {
    printf("1\n");
  } else if (res == REG_NOMATCH) {
    printf("0\n");
  } else {
    perror("Regex match failed");
  }

  free(email);
  regfree(&regex);

  return 0;
}