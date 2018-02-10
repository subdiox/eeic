#include <stdio.h>

int strlen1(char *s);

int main() {
    char *s = "kadai06";
    printf("%d\n", strlen1(s));

    return 0;
}

int strlen1(char *s) {
    return sizeof(s) - 1;
}
