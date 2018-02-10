#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define K 5 // スロットマシンの数
#define N 100 // コインの数．採点時には1,000,000などの大きな数にする．
int bet(int k); //変更不可

int main() { //自由に変更して良い
    srand((unsigned) time(NULL));
    int credit = N, reward = 0, sum = 0;

    double uk[K];

    int try = N / K / 2;

    for (int i = 0; i < K; i ++) {
        double try_sum = 0;
        for (int j = 0; j < try; j ++) {
            credit --;
            int try_reward = bet(i);
            try_sum += try_reward;
            sum += try_reward;
        }
        uk[i] = try_sum / K;
    }

    int max_index = 0;
    for (int i = 0; i < K; i ++) {
        if (uk[max_index] < uk[i]) {
            max_index = i;
        }
    }

    while (credit-- > 0) {
        reward = bet(max_index);
        sum += reward;
    }

    printf("sum = %f\n", (double)sum / N);

    return 0;
}

// 以下の関数は変更不可
int bet(int k){ // 入力：スロットマシンの番号　出力：当選金
    // これは例であり，採点時には当選金と確率が変化する
    switch(k){
        case 0:
            if(rand() % 2 == 0){ // 2/2
                return 2;
            }else{
                return 0;
            }
            break;
        case 1:
            if(rand() % 7 == 0){ // 5/7
                return 5;
            }else{
                return 0;
            }
            break;
        case 2:
            if(rand() % 8 == 0){ // 10/8
                return 10;
            }else{
                return 0;
            }
            break;
        case 3:
            if(rand() % 40 == 0){ // 50/45
                return 50;
            }else{
                return 0;
            }
            break;
        case 4:
            if(rand() % 200 == 0){  // 100/200
                return 100;
            }else{
                return 0;
            }
            break;
        default:
            return 0;
    }
}
