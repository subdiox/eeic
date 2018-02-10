#include <stdio.h>
#include <math.h>
#define NALPHA 26

int main() {
    int count[NALPHA][NALPHA];
    double prob[NALPHA][NALPHA], entropy[NALPHA];
    int c, i, j, sum, first, second, minidx, maxidx;
    double min, max;

    for (i = 0; i < NALPHA; i ++) {
        for (j = 0; j < NALPHA; j ++) {
            count[i][j] = 0;
            prob[i][j] = 0;
        }
    }

    for (i = 0; i < NALPHA; i ++) {
        entropy[i] = 0;
    }

    second = ' ';
    while ((c = getchar()) != EOF) {
        first = second;
        second = c;
        if (first >= 'a' && first <= 'z' && second >= 'a' && second <= 'z') {
            count[first - 'a'][second - 'a'] ++;
        }
    }

    for (i = 0; i < NALPHA; i ++) { // 確率を計算するループ
        sum = 0;

        for (j = 0; j < NALPHA; j ++) {
            sum += count[i][j];
        }

        for (j = 0; j < NALPHA; j ++) {
            prob[i][j] = (double)count[i][j] / (double)sum;
            if (count[i][j] != 0) {
                entropy[i] -= log(prob[i][j]) / log(2) * (prob[i][j]);
            }
        }
        printf("[%c]: ent = %e\n", 'a' + i, entropy[i]);
    }

    return 0;
 }
