#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BUFSIZE 1000
#define HEIGHT 40
#define WIDTH 70
#define RATE 10

int cell[HEIGHT][WIDTH];

void print_cells(FILE *fp)
{
    int i, j;

    fprintf(fp, "----------\n");

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            const char c = (cell[i][j] == 1) ? '#' : ' ';
            fputc(c, fp);
        }
        fputc('\n', fp);
    }
    fflush(fp);

    sleep(1);
}

void init_cells_randomly()
{
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell[i][j] = 0;
        }
    }

    srand(time(NULL));
    for (int i = 0; i < HEIGHT; i ++) {
        for (int j = 0; j < WIDTH; j ++) {
            if (rand() % 100 < RATE) {
                cell[i][j] = 1;
            }
        }
    }
}

void init_cells_from_file(char *filename) {
    char buf[BUFSIZE];
    FILE *fp;
    int i = 0, j;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("error: can't open %s\n", filename);
        exit(1);
    }

    while (fgets(buf, BUFSIZE, fp) != NULL) {
        for (j = 0; j < WIDTH; j ++) {
            if (buf[j] == '#') {
                cell[i][j] = 1;
            } else {
                cell[i][j] = 0;
            }
        }
        i ++;
    }

    fclose(fp);
}


int count_adjacent_cells(int i, int j)
{
    int n = 0;
    int k, l;
    for (k = i - 1; k <= i + 1; k++) {
        if (k < 0 || k >= HEIGHT) continue;
        for (l = j - 1; l <= j + 1; l++) {
            if (k == i && l == j) continue;
            if (l < 0 || l >= WIDTH) continue;
            n += cell[k][l];
        }
    }
    return n;
}

void update_cells()
{
    int i, j;
    int cell_next[HEIGHT][WIDTH];

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell_next[i][j] = 0;
            const int n = count_adjacent_cells(i, j);
            if (cell[i][j] == 1) {
                if (n == (2 | 3)) {
                    cell_next[i][j] = 1;
                }
            } else {
                if (n == 3) {
                    cell_next[i][j] = 1;
                }
            }
        }
    }

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            cell[i][j] = cell_next[i][j];
        }
    }
}


int main(int argc, char *argv[])
{
    int gen;
    FILE *fp;

    if ((fp = fopen("cells.txt", "a")) == NULL) {
        fprintf(stderr, "error: cannot open a file.\n");
        return 1;
    }

    if (argc == 1) {
        printf("Initialize cells randomly.\n");
        init_cells_randomly();
    } else {
        printf("Initialize cells from input file: %s\n", argv[1]);
        init_cells_from_file(argv[1]);
    }

    print_cells(fp);

    for (gen = 1;; gen++) {
        int count = 0;
        for (int i = 0; i < HEIGHT; i ++) {
            for (int j = 0; j < WIDTH; j ++) {
                count += cell[i][j];
            }
        }
        printf("generation = %d, cell rate: %lf\n", gen, (double)count / (HEIGHT * WIDTH));
        update_cells();
        print_cells(fp);
    }

    fclose(fp);

    return 0;
}
