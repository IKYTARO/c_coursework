#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 * ЗАДАЧА: Problem EE -- расширенный алгоритм Eвклида
 *
 * ОПИСАНИЕ:  
 *   Даны два целых числа x и y. Необходимо найти их наибольший общий делитель d и целые, возможно отрицательные числа a и b, такие, что:
 *   ax + by = d (тождество Безу)
 *
 * ВХОДНЫЕ ДАННЫЕ:
 *   - [x]: [long long] [целое число]
 *   - [y]: [long long] [цело число]
 *
 * ВЫХОДНЫЕ ДАННЫЕ:
 *   - [a]: [long long] [коэффициент a]
 *   - [b]: [long long] [коэффициент b]
 *   - [d]: [long long] [НОД(x, y)]
 *
 * ПРИМЕРЫ:
 *   Вход: 1769 551
 *   Выход: 5 -16 29
 ******************************************************************************/

typedef struct {
    long long d;
    long long a;
    long long b;
} ExtendedGCDResult;

ExtendedGCDResult extended_gcd(long long x, long long y) {
    long long c = x, ac = 1, bc = 0;
    long long d = y, ad = 0, bd = 1;
    
    while (d != 0) {
        long long q = c / d;
        long long r = c % d;

        if (r == 0) {
            break;
        }

        long long ar = ac - q * ad;
        long long br = bc - q * bd;

        c = d; ac = ad; bc = bd;
        d = r; ad = ar; bd = br;
    }

    if (d < 0) {
        d = -d; ad = -ad; bd = -bd;
    }
    
    ExtendedGCDResult result = {d, ad, bd};
    return result;
}

int main() {
    long long x, y;
    if (scanf("%lld%lld", &x, &y) != 2) {
        fprintf(stderr, "Error reading input");
        abort();
    }

    ExtendedGCDResult res = extended_gcd(x, y);
    printf("%lld %lld %lld\n", res.a, res.b, res.d);

    return 0;
}