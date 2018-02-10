#include <stdio.h>
#include <string.h>

void uppercase(char *s);

int main() {
    char s[] = "hello world";
    uppercase(s);
    printf("%s\n",s);
    // 出力䛿"HELLO WORLD"
    return 0;
}

void uppercase(char *s) {
    for (int i = 0; i < strlen(s); i ++) {
        if (s[i] != 32) {
            s[i] = s[i] - 32;
        }
    }
}
