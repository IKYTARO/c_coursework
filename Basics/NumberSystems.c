#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * ЗАДАЧА: Problem NS -- системы счисления
 *
 * ОПИСАНИЕ:
 *   Необходимо написать программу, которая печатает на стандартный вывод
 *   число x в системе счисления по основанию y
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [x]: [unsigned] [целое положительное число]
 *   - [y]: [unsigned] [основание системы счисления]
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [output]: [unsigned] [число x в системе счисления по основанию y]
 *
 * ОГРАНИЧЕНИЯ:
 *   - [1 < y < 11]
 *   - [x > 0]
 *
 * ПРИМЕРЫ:
 *   Вход: 12 10
 *   Выход: 12
 *
 *   Вход: 12 6
 *   Выход: 20
 ******************************************************************************/

void print_to_base(unsigned value, unsigned base);

int main() {
    unsigned value, base;
    if (scanf("%u%u", &value, &base) != 2 || base < 2 || base > 10 ||
        value <= 0) {
        perror("Invalid input");
        abort();
    }

    print_to_base(value, base);

    return EXIT_SUCCESS;
}

void print_to_base(unsigned value, unsigned base) {
    if (value == 0) {
        return;
    }

    print_to_base(value / base, base);
    printf("%u", value % base);
}