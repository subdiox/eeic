#include <stdio.h>

int main() {
    for (int x = 1; x < 1000; x ++) {
        for (int y = 1; y < 1000; y ++) {
            for (int z = 1; z < 1000; z ++) {
                if (x * x + y * y == z * z) {
                    printf("(x, y, z) = (%d, %d, %d)\n", x, y, z);
                }
            }
        }
    }
}
