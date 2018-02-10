#include <stdio.h>
#include <math.h>

int main() {
    double t = pow(log(2), 2);
    for (int n = 1; n < 10000; n++) {
        t += 1 / (pow(n, 2) * pow(2, n - 1));
    }
    printf("%15.14lf\n", sqrt(t * 6));
    return 0;
}
