#include <stdio.h>

double pie(double x0, int N);

int main() {
    for (int i = 1; i <= 10; i ++) {
        printf("N = %d => pie = %lf\n", i, pie(0.1, i));
    }
    printf("N = 100 => pie = %lf\n", pie(0.1, 100));
    printf("N = 1000 => pie = %lf\n", pie(0.1, 1000));
    return 0;
}

double pie(double x0, int N) {
    int x = x0 * 10;
    for (int i = 0; i < N; i ++) {
        if (x <= 5) {
            x = 2 * x;
        } else {
            x = 20 - 2 * x;
        }
    }
    return (double)x / 10;
}
