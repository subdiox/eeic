#include <stdio.h>

void strcpy1(char *s, char *t);

int main() {
    char t[] = "kadai07";
    char s[100];
    strcpy1(s, t);

    printf("%s\n", s);
    return 0;
}

void strcpy1(char *s, char *t) {
    while((*s++ = *t++) != '\0')
      ;
}
