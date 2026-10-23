#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * ЗАДАЧА: Problem EX — работа с битами плавающих чисел
 *
 * ОПИСАНИЕ:
 *   Вам дано число с плавающей точкой типа float (мантисса 23 нижних бита).
 *   Вы должны получить из него другое число с плавающей точкой , инвертировав все нечётные биты
 *фракции (нулевой бит считается чётным) и напечатать с точностью до пятого знака после запятой.
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - input: float число с плавающей точкой
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - output: float чило, полученное после инвертирования нечетных бит
 *
 * ПРИМЕРЫ:
 *   Вход:  1.0
 *   Выход: 1.33333
 *
 *   Вход:  1.23456
 *   Выход: 1.41152
 ******************************************************************************/

unsigned float_to_uint(float value);
float uint_to_float(unsigned value);
void reverse_bits(unsigned *value);

int main() {
  float input;
  int input_res = scanf("%f", &input);

  if (input_res != 1) {
    abort();
  }

  unsigned uinput = float_to_uint(input);
  reverse_bits(&uinput);
  float output = uint_to_float(uinput);
  printf("%.5f", output);

  return EXIT_SUCCESS;
}

unsigned float_to_uint(float value) {
  unsigned result;
  memcpy(&result, &value, sizeof(unsigned));
  return result;
}

float uint_to_float(unsigned value) {
  float result;
  memcpy(&result, &value, sizeof(float));
  return result;
}

void reverse_bits(unsigned *value) {
  for (int i = 1; i < 23; i += 2) {
    (*value) = (*value) ^ (1u << i);
  }
}