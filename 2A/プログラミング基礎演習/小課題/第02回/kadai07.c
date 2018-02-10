#include <stdio.h>
#include <math.h>

int prime_list[1000];
int prime_list_size = 0;
int is_prime(int); /*引数が素数場合1，それ以外場合0を返す*/
void check_goldbach(int);

int main() {
    int i;
    double t = 1;
    for (i = 2; i <= 1000; i ++) {
        is_prime(i);
    }
    for (int j = 4; j <= 1000; j += 2) {
        check_goldbach(j);
    }
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

void check_goldbach(int n) {
    for (int i = 0; i < prime_list_size; i ++) {
        for (int j = 0; j < prime_list_size; j ++) {
            if (n == prime_list[i] + prime_list[j]) {
                printf("%d = %d + %d\n", n, prime_list[i], prime_list[j]);
                return;
            }
        }
    }
}
