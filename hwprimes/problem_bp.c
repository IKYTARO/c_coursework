#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem BP -- старший и младший бит
 *
 * ОПИСАНИЕ:
 *   Вам предлагается найти позицию старшего и младшего установленных битов в числе N.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - N: unsigned число
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - minor_bit: unsigned младший бит числа N
 *   - major_bit: unsigned старший бит числа N
 *
 * ПРИМЕРЫ:
 *   Вход: 2
 *   Выход: 1 1
 *
 *   Вход: 269971274
 *   Выход: 28 1
 *
 *   Вход: 0
 *   Выход: NO
 ******************************************************************************/

typedef struct {
  int minor_bit;
  int major_bit;
} bit_pair;

bit_pair get_bit_pair(unsigned N);

int main() {
  unsigned num;
  int res = scanf("%u", &num);
  if (res == -1) {
    printf("Wrong input!\n");
    abort();
  }

  if (num == 0) {
    printf("NO");
  } else {
    bit_pair result = get_bit_pair(num);
    printf("%i %i", result.major_bit, result.minor_bit);
  }

  return 0;
}

bit_pair get_bit_pair(unsigned N) {
  bit_pair answer = {-1, 0};

  for (unsigned long i = 0; i < sizeof(unsigned) * CHAR_BIT; i++) {
    if (((N >> i) & 1u) == 1) {
      answer.major_bit = i;
      if (answer.minor_bit == -1)
        answer.minor_bit = i;
    }
  }
  return answer;
}