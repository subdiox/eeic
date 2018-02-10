#include <stdio.h>
#include <math.h>

int prime_list[10000];
int prime_list_size = 0;
int is_prime(int); /*引数が素数場合1，それ以外場合0を返す*/

int main() {
    int i;
    double t = 1;
    for (i = 2; i <= 10000; i ++) {
        if (is_prime(i) == 1) {
            t *= (1 - 1 / pow((double)i, 2));
        }
    }
    printf("pi = %6.5lf\n", sqrt(6 / t));
}

int is_prime(int n) {
    for (int i = 0; i < prime_list_size; i ++) {
        if (n % prime_list[i] == 0) {
            return 0;
        }
    }
    prime_list[prime_list_size] = n;
    prime_list_size += 1;
    return 1;
}
