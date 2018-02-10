#include <stdio.h>

int strcmp1(char *s, char *t);

int main() {
    char s[] = "kadai08";
    char t[] = "kadai09";
    int i = strcmp1(s, t);

    printf("%d\n", i);
    return 0;
}

int strcmp1(char *s, char *t) {
    for (int i = 0; i < sizeof(s); i++) {
        if (s[i] > t[i]) {
            return 1;
        } else if (s[i] < t[i]) {
            return -1;
        }
    }
    return 0;
}
