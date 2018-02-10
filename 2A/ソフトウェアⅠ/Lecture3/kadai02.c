#include <stdio.h>
#include <stdlib.h>

#define winrate 80

char* get_hand(int);
char* get_win(int);

int main() {
    srand(1);
    while (1) {
        char command[100];
        scanf("%s", command);

        int win = -1;
        if (rand() % 100 < winrate) {
            win = 1;
        }

        int you;
        if (command[0] == 'g' || command[0] == 'G') {
            you = 0;
        } else if (command[0] == 'c' || command[0] == 'C') {
            you = 1;
        } else if (command[0] == 'p' || command[0] == 'P') {
            you = 2;
        } else if (command[0] == 'q' || command[0] == 'Q') {
            printf("プログラムを終了します．\n");
            exit(0);
        }
        int me = you + win;
        
        printf("あなたは%sを出しました．私は%sを出しました．あなたの%s！\n", get_hand(you), get_hand(me), get_win(win));
    }

    return 0;
}

char *get_hand(int i) {
    if (i % 3 == 0) {
        return "グー";
    } else if (i % 3 == 1) {
        return "チョキ";
    } else {
        return "パー";
    }
}

char *get_win(int i) {
    if (i == -1) {
        return "負け";
    } else {
        return "勝ち";
    }
}
