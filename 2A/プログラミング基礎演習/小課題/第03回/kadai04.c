#include <stdio.h>
#include <math.h>

int mod_apm(int a, int p, int m);

int main() {
    int result = mod_apm(541, 1234, 1299709);
    printf("%d\n", result);
    return 0;
}

int mod_apm(int a, int p, int m) {
    if (p == 0) {
        return 1;
    } else {
        return (a * mod_apm(a, p - 1, m)) % m;
    }
}
