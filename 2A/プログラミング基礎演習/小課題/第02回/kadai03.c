#include <stdio.h>

int prime_list[1000];
int prime_list_size = 0;
int is_prime(int); /*引数が素数場合1，それ以外場合0を返す*/

int main() {
    int i;
    for (i = 2; i <= 1000; i ++) {
        if (is_prime(i) == 1) {
            printf("%d is prime\n", i);
        }
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
