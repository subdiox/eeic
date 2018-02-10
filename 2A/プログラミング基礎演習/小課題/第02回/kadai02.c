#include <stdio.h>
#include <math.h>

int zeller(int, int, int);

int main() {
    int year = 2017, month = 10, day = 4;
    int w;
    w = zeller(year, month, day);
    printf("%d/%d/%d is ", month, day, year);
    switch(w) {
        case 0: printf("Sat.\n"); break;
        case 1: printf("Sun.\n"); break;
        case 2: printf("Mon.\n"); break;
        case 3: printf("Tue.\n"); break;
        case 4: printf("Wed.\n"); break;
        case 5: printf("Thurs.\n"); break;
        case 6: printf("Fri.\n"); break;
    }
    return 0;
}

int zeller(int year, int month, int day) {
    double h = (double)(year / 100);
    double y = (double)(year % 100);
    double m = (double)month;
    double d = (double)day;
    int w = y + floor(y / 4) + floor(h / 4) - 2 * h + floor(13 * (m + 1) / 5) + d;
    return w % 7;
}
