#include <stdio.h>
#include <math.h>

int main() {
    double pi = 3.14; // 本当はM_PIを使うべき
    for (double x = -5 * pi; x <= 5 * pi; x += 0.01 * pi) {
        double y = sin(3.14 * x) / (3.14 * x);
        printf("%f %f\n", x, y);
    }

    return 0;
}
