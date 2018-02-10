#include <stdio.h>
#include <string.h> //strlen()に使う．
int check_kaibun(char str[]);

int main(void){
    char str[] = "kayak";
    if (check_kaibun(str)) {
        printf("%s is palindrome.\n", str);
    } else {
        printf("%s is NOT palindrome.\n", str);
    }
    return 0;
}

int check_kaibun(char str[]) {
    int n = strlen(str);
    for (int i = 0; i <= n / 2; i ++) {
        if (str[i] != str[n - i - 1]) {
            return 0;
        }
    }
    return 1;
}
