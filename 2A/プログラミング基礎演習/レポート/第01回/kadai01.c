#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1000
#define MAX_T 1000

double a[MAX_LEN][MAX_LEN], v[MAX_LEN], k;
int dimension;

void init(); // 行列を初期化
void load_matrix(char *filename); // ファイルから行列Aを生成
void calculate_power(); // べき乗法を計算
void print_answer(); // 解答を出力

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [input file]\n", argv[0]);
        return 1;
    }
    init();
    load_matrix(argv[1]);
    calculate_power();
    print_answer();

    return 0;
}

void init() {
    for (int i = 0; i < MAX_LEN; i ++) {
        for (int j = 0; j < MAX_LEN; j ++) {
            a[i][j] = -1;
        }
    }
    for (int i = 0; i < MAX_LEN; i ++) {
        v[i] = 1;
    }
}

void load_matrix(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        exit(1);
    }
    int i, j, row, column;
    char buf[MAX_LEN];
    while (fgets(buf, MAX_LEN, fp) != NULL) {
        a[i][j] = atof(strtok(buf, "\t"));
        while (1) {
            j ++;
            char *ptr = strtok(NULL, "\t");
            if (ptr == NULL) {
                column = j;
                break;
            } else {
                a[i][j] = atof(ptr);
            }
        }
        i ++;
        j = 0;
    }
    fclose(fp);
    row = i;
    if (row != column) {
        printf("Difference between row and column will cause a bug. Please check the file again.\n");
        exit(1);
    }
    dimension = row;
}

void calculate_power() {
    for (int t = 0; t <= MAX_T; t ++) {
        double v_old[MAX_LEN];
        for (int i = 0; i < MAX_LEN; i ++) {
            v_old[i] = v[i];
            v[i] = 0;
        }
        for (int i = 0; i < dimension; i ++) {
            for (int j = 0; j < dimension; j ++) {
                v[i] += a[i][j] * v_old[j];
            }
        }
        double sum = 0;
        for (int i = 0; i < dimension; i ++) {
            sum += pow(v[i], 2);
        }
        double size = sqrt(sum);
        for (int i = 0; i < dimension; i ++) {
            v[i] = v[i] / size;
        }
    }
    if (v[0] < 0) {
        for (int i = 0; i < dimension; i ++) {
            v[i] *= -1;
        }
    }
    double av[MAX_LEN];
    for (int i = 0; i < dimension; i ++) {
        av[i] = 0;
    }
    for (int i = 0; i < dimension; i ++) {
        for (int j = 0; j < dimension; j ++) {
            av[i] += a[i][j] * v[j];
        }
    }
    k = 0;
    for (int i = 0; i < dimension; i ++) {
        k += v[i] * av[i];
    }
}

void print_answer() {
    printf("Max Eigen Vector: (");
    for (int i = 0; i < dimension - 1; i ++) {
        printf("%f, ", v[i]);
    }
    printf("%f)\n", v[dimension - 1]);
    printf("Max Eigen Value: %f\n", k);
}
