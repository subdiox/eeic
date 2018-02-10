#include <stdio.h>
#include <string.h>

int main() {
    for (int i = 1; i <= 1000; i ++) {
        char buf[5];
        sprintf(buf, "%d", i);
        int c = '3';
        if (i % 3 == 0 || strchr(buf, c) != NULL) {
            printf("%d!!!\n", i);
        } else {
            printf("%d\n", i);
        }
    }
    return 0;
}
