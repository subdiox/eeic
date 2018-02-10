#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define PROBABILITY 20 // 初期状態で誕生している確率(%)
#define HEIGHT 25 // 縦幅
#define WIDTH 25 // 横幅

int main() {
    srand(time(NULL));
    char matrix[HEIGHT + 2][WIDTH + 2];
    for (int i = 0; i < HEIGHT + 2; i ++) {
        for (int j = 0; j < WIDTH + 2; j ++) {
            if (i > 0 && i < HEIGHT + 1 && j > 0 && j < WIDTH + 1 && rand() % 100 < PROBABILITY) {
                matrix[i][j] = '*';
            } else {
                matrix[i][j] = ' ';
            }
        }
    }
    while(1) {
        system("clear");
        for (int i = 1; i < HEIGHT + 1; i ++) {
            for (int j = 1; j < WIDTH + 1; j ++) {
                printf("%c ",matrix[i][j]);
            }
            printf("\n");
        }
        for (int i = 1; i < HEIGHT + 1; i ++) {
            for (int j = 1; j < WIDTH + 1; j ++) {
                int count = 0;
                count += (matrix[i - 1][j - 1] == '*');
                count += (matrix[i - 1][j] == '*');
                count += (matrix[i - 1][j + 1] == '*');
                count += (matrix[i][j - 1] == '*');
                count += (matrix[i][j + 1] == '*');
                count += (matrix[i + 1][j - 1] == '*');
                count += (matrix[i + 1][j] == '*');
                count += (matrix[i + 1][j + 1] == '*');
                if (matrix[i][j] == '*') {
                    if (count != 2 && count != 3) {
                        matrix[i][j] = ' ';
                    }
                } else {
                    if (count == 3) {
                        matrix[i][j] = '*';
                    }
                }
            }
        }
        usleep(100000);
    }

    return 0;
}
