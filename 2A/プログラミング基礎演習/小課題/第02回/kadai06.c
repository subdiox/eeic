#include <stdio.h>

int gcd(int, int);

int main() {
    for (int x = 1; x < 1000; x ++) {
        for (int y = 1; y < 1000; y ++) {
            for (int z = 1; z < 1000; z ++) {
                if (x * x + y * y == z * z) {
                    if (gcd(gcd(x, y), gcd(y, z)) == 1) {
                        printf("(x, y, z) = (%d, %d, %d)\n", x, y, z);
                    }
                }
            }
        }
    }
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}
