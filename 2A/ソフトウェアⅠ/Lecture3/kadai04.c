#include <stdio.h>

int main() {
    char strarray[3][10] = {"pen", "apple", "pineapple"};
    printf("%s %s %s %s\n", strarray[0], strarray[2], strarray[1], strarray[0]);
    return 0;
}
