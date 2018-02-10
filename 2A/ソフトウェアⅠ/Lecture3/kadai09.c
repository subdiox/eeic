#include <stdio.h>
#include <math.h>
#define NALPHA 26 //アルファベットの種類

int main() {
    int count[NALPHA], prob[NALPHA];
    int c, i;
    double entropy;
    for(i = 0; i < NALPHA; i ++) { //カウント結果を記録する配列を初期化する
        count[i] = 0;
        prob[i] = 0;
    }

    while((c = getchar()) != EOF) {
        if(c >= 'a' && c <= 'z') { // cがaからzの間アルファベットであれば、、、
            count[c - 'a'] ++; // 該当するアルファベットのカウント結果を+1する。
        }
    }

    int sum = 0;
    for (i = 0; i < NALPHA; i ++) { // 合計を計算する
        sum += count[i];
    }

    for (i = 0; i < NALPHA; i ++) {
        entropy -= log((double)count[i] / sum) / log(2) * ((double)count[i] / sum);
    }

    printf("entropy = %e\n", entropy);

    return 0;
}
