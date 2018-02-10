#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1000
#define MAX_T 1000

double x[MAX_LEN][MAX_LEN], c[MAX_LEN][MAX_LEN], v[MAX_LEN], component[MAX_LEN][MAX_LEN], k;
int dimension, row, column;

void init(); // 行列を初期化
void load_data(char *filename, char *delimiter); // ファイルから行列Xを生成
void calculate_covariance(); // 共分散行列Cを計算
void calculate_power(); // べき乗法を計算
void calculate_next(); // 行列Aを行列Bに置換
void calculate_component(); // 主成分を計算
void print_answer(int n); // 解答を出力

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [input file]\n", argv[0]);
        return 1;
    }
    init();
    load_data(argv[1], ",");
    calculate_covariance();
    for (int n = 0; n < column; n ++) {
        calculate_power();
        calculate_component();
        print_answer(n);
        calculate_next();
    }

    return 0;
}

void init() {
    for (int i = 0; i < MAX_LEN; i ++) {
        for (int j = 0; j < MAX_LEN; j ++) {
            x[i][j] = -1;
            c[i][j] = -1;
        }
    }
    for (int i = 0; i < MAX_LEN; i ++) {
        v[i] = 1;
    }
}

void load_data(char *filename, char *delimiter) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        exit(1);
    }
    int i, j;
    char buf[MAX_LEN];
    while (fgets(buf, MAX_LEN, fp) != NULL) {
        char *ptr1 = strtok(buf, delimiter);
        if (ptr1 == NULL) {
            break;
        }
        char *end1;
        double d1 = strtod(ptr1, &end1);
        if (strlen(end1) > 0) {
            break;
        }
        x[i][j] = d1;
        while (1) {
            j ++;
            char *ptr2 = strtok(NULL, delimiter);
            if (ptr2 == NULL) {
                if (column == 0) {
                    column = j;
                }
                break;
            }
            char *end2;
            double d2 = strtod(ptr2, &end2);
            if (strlen(end2) > 0) {
                if (column == 0) {
                    column = j;
                }
                break;
            }
            x[i][j] = d2;
        }
        i ++;
        j = 0;
    }
    fclose(fp);
    row = i;
    dimension = column;
}

void calculate_covariance() {
    for (int a = 0; a < column; a ++) {
        for (int b = 0; b < column; b ++) {
            double xa_bar = 0;
            for (int l = 0; l < row; l ++) {
                xa_bar += x[l][a] / row;
            }
            double xb_bar = 0;
            for (int l = 0; l < row; l ++) {
                xb_bar += x[l][b] / row;
            }
            for (int l = 0; l < row; l ++) {
                c[a][b] += (x[l][a] - xa_bar) * (x[l][b] - xb_bar) / (row - 1);
            }
        }
    }
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
                v[i] += c[i][j] * v_old[j];
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
    double cv[MAX_LEN];
    for (int i = 0; i < dimension; i ++) {
        cv[i] = 0;
    }
    for (int i = 0; i < dimension; i ++) {
        for (int j = 0; j < dimension; j ++) {
            cv[i] += c[i][j] * v[j];
        }
    }
    k = 0;
    for (int i = 0; i < dimension; i ++) {
        k += v[i] * cv[i];
    }
}

void calculate_next() {
    for (int i = 0; i < MAX_LEN; i ++) {
        for (int j = 0; j < MAX_LEN; j ++) {
            c[i][j] = c[i][j] - k * v[i] * v[j];
        }
    }
}

void calculate_component() {
    for (int i = 0; i < row; i ++) {
        double scalar = 0;
        for (int j = 0; j < column; j ++) {
            scalar += v[j] * x[i][j];
        }
        for (int j = 0; j < column; j ++) {
            component[i][j] = scalar * v[j];
        }
    }
}

void print_answer(int n) {
    printf("Eigen Vector %d: (", n + 1);
    for (int j = 0; j < column - 1; j ++) {
        printf("%f, ", v[j]);
    }
    printf("%f)\n", v[column - 1]);
    printf("Principal Component %d: (\n", n + 1);
    for (int i = 0; i < row; i ++) {
        printf("    ");
        for (int j = 0; j < column - 1; j ++) {
            printf("%f, ", component[i][j]);
        }
        printf("%f\n", component[i][column - 1]);
    }
    printf(")\n");
}
