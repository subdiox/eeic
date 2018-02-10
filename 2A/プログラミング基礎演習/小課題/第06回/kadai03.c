#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000

int main(int argc, char *argv[]) {
    int matrix[MAX_LEN][MAX_LEN], x, y, row, column;

    if (argc != 2) {
        printf("Usage: %s [input file]\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    char buf[MAX_LEN];
    while (fgets(buf, MAX_LEN, fp) != NULL) {
        matrix[x][y] = atoi(strtok(buf, "\t"));
        while (1) {
            y ++;
            char *ptr = strtok(NULL, "\t");
            if (ptr == NULL) {
                column = y;
                break;
            } else {
                matrix[x][y] = atoi(ptr);
            }
        }
        x ++;
        y = 0;
    }
    fclose(fp);
    row = x;

    for (int i = 0; i < row; i ++) {
        for (int j = 0; j < column; j ++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
