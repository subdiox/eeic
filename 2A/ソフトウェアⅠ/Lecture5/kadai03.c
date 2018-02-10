#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 10
#define WIDTH 10

int main() {
    int needs_scanf = 1;
    char matrix[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            matrix[i][j] = '-';
        }
    }
    FILE *fpr;
    if ((fpr = fopen("data.txt", "r")) == NULL) {
        printf("data.txtが存在しないため，matrixを初期化します．\n");
    } else {
        char str[HEIGHT * WIDTH];
        fgets(str, HEIGHT * WIDTH + 1, fpr);
        for (int i = 0; i < HEIGHT; i ++) {
            for (int j = 0; j < WIDTH; j ++) {
                matrix[i][j] = str[i * WIDTH + j];
            }
        }
        needs_scanf = 0;
    }
    fclose(fpr);

    while(1) {
        if (needs_scanf) {
            char *input, c;
            int x, y;
            scanf("%d", &x);
            scanf("%d", &y);
            scanf("%s", &c);
            matrix[x][y] = c;
        }

        FILE *fpw;
        fpw = fopen("data.txt", "w");
        for (int i = 0; i < HEIGHT; i ++) {
            for (int j = 0; j < WIDTH; j ++) {
                fputc(matrix[i][j], fpw);
            }
        }
        fclose(fpw);
        system("clear");
        for (int i = 0; i < HEIGHT; i ++) {
            for (int j = 0; j < WIDTH; j ++) {
                printf("%c ",matrix[i][j]);
            }
            printf("\n");
        }
        needs_scanf = 1;
    }

    return 0;
}
