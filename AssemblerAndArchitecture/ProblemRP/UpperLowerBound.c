#include <fenv.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem RP — верхняя и нижняя границы
 *
 * ОПИСАНИЕ:
 *   Для этой задачи, установите округление вниз (FE_DOWNWARD)
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - numerator:   int числитель дроби
 *   - denominator: int знаменатель дроби
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   Нижняя (по направлению к нулю) и верхняя (к бесконечности или минус бесконечности)
 *аппроксимации при представлении в формате float как два шестнадцатиричных числа: экспонента и
 *дробная часть мантиссы. Если возможно точное представление, на выходе только оно
 *
 * ПРИМЕРЫ:
 *   Вход: 1 3
 *   Выход: 0x7d 0x2aaaaa 0x7d 0x2aaaab
 *
 *   Вход: 1 2
 *   Выход: 0x7e 0x0
 ******************************************************************************/

unsigned float_to_uint(float value);
bool is_exat_float_division(int numerator, int denominator);
void copy_bits(unsigned src, unsigned *dst, unsigned start, unsigned stop);
void print_float_parts(float value);

int main() {
  int numerator, denominator;
  int input_res = scanf("%d%d", &numerator, &denominator);
  if (input_res != 2) {
    perror("Invalid input.");
    abort();
  }

  bool is_negative = numerator * denominator < 0;
  int LOWER_BOUND  = (is_negative) ? FE_UPWARD : FE_DOWNWARD;
  int UPPER_BOUND  = (is_negative) ? FE_DOWNWARD : FE_UPWARD;

  fesetround(LOWER_BOUND);
  float lower_bound = (float)numerator / denominator;
  print_float_parts(lower_bound);

  if (!is_exat_float_division(numerator, denominator)) {
    printf(" ");

    fesetround(UPPER_BOUND);
    float upper_bound = (float)numerator / denominator;
    print_float_parts(upper_bound);
  }

  return EXIT_SUCCESS;
}

unsigned float_to_uint(float value) {
  unsigned result;
  memcpy(&result, &value, sizeof(unsigned));
  return result;
}

bool is_exat_float_division(int numerator, int denominator) {
  int n = numerator, d = denominator;
  while (d != 0) {
    int tmp = d;
    d       = n % d;
    n       = tmp;
  }
  int gcd = n;

  int reduced_numerator   = numerator / gcd;
  int reduced_denominator = denominator / gcd;

  bool denominator_ok = (reduced_denominator & (reduced_denominator - 1)) == 0;
  bool numerator_ok   = (abs(reduced_numerator) < (1 << 24));

  return denominator_ok && numerator_ok;
}

void copy_bits(unsigned src, unsigned *dst, unsigned start, unsigned stop) {
  unsigned char bit = 0;
  for (unsigned i = start; i < stop; i++) {
    bit = (src >> i) & 1u;
    if (bit == 1) {
      *dst = (*dst) | (1u << (i - start));
    } else {
      *dst = (*dst) & ~(1u << (i - start));
    }
  }
}

void print_float_parts(float value) {
  unsigned uvalue = float_to_uint(value);

  unsigned mantissa = 0, exponent = 0;
  copy_bits(uvalue, &mantissa, 0, 23);
  copy_bits(uvalue, &exponent, 23, 31);

  printf("0x%x 0x%x", exponent, mantissa);
}
