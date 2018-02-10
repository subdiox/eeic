#include <stdio.h>
#include <string.h>

#define ROMAN_COUNT 7

int roman2num(char*);

int main() {
    // Examples
    printf("XIV = %d\n", roman2num("XIV"));
    //14
    printf("CDXCV = %d\n", roman2num("CDXCV"));
    //495
    printf("MCMXLV = %d\n", roman2num("MCMXLV"));
    //1945
    printf("MMMCMXCIX = %d\n", roman2num("MMMCMXCIX"));
    //3999
    return 0;
}

int roman2num(char *s){
    char roman[ROMAN_COUNT] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};
    int num[ROMAN_COUNT] = {1, 5, 10, 50, 100, 500, 1000};
    int sum = 0;
    for (int i = 0; i < strlen(s) - 1; i ++) {
        int ci; // current index
        for (ci = 0; ci < ROMAN_COUNT; ci ++) {
            if (s[i] == roman[ci]) {
                break;
            }
        }
        int ni; // next index
        for (ni = 0; ni < ROMAN_COUNT; ni ++) {
            if (s[i + 1] == roman[ni]) {
                break;
            }
        }
        if (ci >= ni) {
            sum += num[ci];
        } else {
            sum -= num[ci];
        }
    }
    int ci; // current index
    for (ci = 0; ci < ROMAN_COUNT; ci ++) {
        if (s[strlen(s) - 1] == roman[ci]) {
            break;
        }
    }
    sum += num[ci];
    return sum;
}
