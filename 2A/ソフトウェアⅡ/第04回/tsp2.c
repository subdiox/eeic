#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <float.h>

#define WIDTH 70
#define HEIGHT 40
#define MAX_CITIES 1000

// for hill_climbing
#define MAX_ITERATIONS 1000
#define MAX_TRIAL 5

// for simulated_annealing
#define T 100000
#define R 0.9999
#define EPSILON 0.00001

// for genetic_algorithm
#define GENERATION 10000
#define POP 100
#define ELITE 20
#define MUTATE_PROB 0.2

// for tabu_search
#define TABU_ITERATIONS 10000
#define TABU_SIZE 7
#define M_RATE 0.5

typedef struct
{
    int x;
    int y;
} City;

City city[MAX_CITIES];
char map[WIDTH][HEIGHT];

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}

int min(const int a, const int b)
{
    return (a < b) ? a : b;
}

int exists(const int n, const int count, int array[])
{
    int exists = 0;
    for (int i = 0; i < count; i ++) {
        if (array[i] == n) exists = 1;
    }
    return exists;
}

void print_array(const int n, int array[]) {
    printf("(");
    for (int i = 0; i < n - 1; i ++) {
        printf("%d, ", array[i]);
    }
    printf("%d)\n", array[n - 1]);
}


void draw_line(const int x0, const int y0, const int x1, const int y1)
{
    int i;
    const int n = max(abs(x1 - x0), abs(y1 - y0)) + 1;
    for (i = 0; i <= n; i++) {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        if (map[x][y] == ' ') map[x][y] = '*';
    }
}

void draw_route(const int n, const int *route)
{
    if (route == NULL) return;

    int i;
    for (i = 0; i < n; i++) {
        const int c0 = route[i];
        const int c1 = (i < n - 1) ? route[i+1] : route[0];
        draw_line(city[c0].x, city[c0].y, city[c1].x, city[c1].y);
    }
}

void plot_cities(FILE *fp, const int n, const int *route)
{
    int i, j, x, y;

    fprintf(fp, "----------\n");

    memset(map, ' ', sizeof(map));

    for (i = 0; i < n; i++) {
        char buf[100];
        sprintf(buf, "C_%d", i);
        for (j = 0; j < strlen(buf); j++) {
            const int x = city[i].x + j;
            const int y = city[i].y;
            map[x][y] = buf[j];
        }
    }

    draw_route(n, route);

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            const char c = map[x][y];
            fputc(c, fp);
        }
        fputc('\n', fp);
    }

    fflush(fp);
}

double distance(const int i, const int j)
{
    const double dx = city[i].x - city[j].x;
    const double dy = city[i].y - city[j].y;
    return sqrt(dx * dx + dy * dy);
}

double total_distance(const int n, int route[])
{
    double sum_d = 0;
    for (int i = 0; i < n; i++) {
        const int c0 = route[i];
        const int c1 = route[(i + 1) % n];
        sum_d += distance(c0, c1);
    }
    return sum_d;
}

double hill_climbing(const int n, int route[])
{
    double shortest_distance = DBL_MAX;
    int shortest_route[MAX_CITIES];
    for (int t = 0; t < MAX_TRIAL; t ++) {
        for (int i = 0; i < n; i ++) {
            route[i] = i;
        }
        for (int i = 0; i < n; i ++) {
            int j = rand() % n;
            int tmp = route[i];
            route[i] = route[j];
            route[j] = tmp;
        }
        for (int i = 0; i < MAX_ITERATIONS; i ++) {
            int random1 = rand() % n;
            int random2 = rand() % n;
            double cur_total = total_distance(n, route);
            int tmp = route[random1];
            route[random1] = route[random2];
            route[random2] = tmp;
            double new_total = total_distance(n, route);
            if (new_total >= cur_total) {
                int tmp = route[random1];
                route[random1] = route[random2];
                route[random2] = tmp;
            }
        }
        if (shortest_distance > total_distance(n, route)) {
            memcpy(shortest_route, route, sizeof(int) * MAX_CITIES);
        }
    }
    memcpy(route, shortest_route, sizeof(int) * MAX_CITIES);
    return total_distance(n, route);
}

double simulated_annealing(const int n, int route[])
{
    for (int i = 0; i < n; i ++) {
        route[i] = i;
    }
    for (int i = 0; i < n; i ++) {
        int j = rand() % n;
        int tmp = route[i];
        route[i] = route[j];
        route[j] = tmp;
    }
    for (double t = T; t > EPSILON; t *= R) {
        int random1 = rand() % n;
        int random2 = rand() % n;
        double cur_total = total_distance(n, route);
        int tmp = route[random1];
        route[random1] = route[random2];
        route[random2] = tmp;
        double new_total = total_distance(n, route);
        double d = new_total - cur_total;
        if (d > 0 && (double)rand() / ((double)RAND_MAX) > exp(-d / t)) {
            int tmp = route[random1];
            route[random1] = route[random2];
            route[random2] = tmp;
        }
    }
    return total_distance(n, route);
}

double genetic_algorithm(const int n, int route[])
{
    int pop[POP][MAX_CITIES];
    for (int p = 0; p < POP; p ++) {
        for (int i = 0; i < n; i ++) {
            pop[p][i] = -1;
        }
    }

    int generated = 0;

    for (int iteration = 0; iteration < GENERATION; iteration ++) {

        // generate random route and choose elites
        for (int p = 0; p < POP; p ++) {
            if (generated == 0) {
                for (int i = 0; i < n; i ++) {
                    route[i] = i;
                }
                for (int i = 0; i < n; i ++) {
                    int j = rand() % n;
                    int tmp = route[i];
                    route[i] = route[j];
                    route[j] = tmp;
                }
            } else {
                for (int i = 0; i < n; i ++) {
                    route[i] = pop[p][i];
                }
            }
            double new_total = total_distance(n, route);
            for (int pp = 0; pp < ELITE; pp ++) {
                double cur_total = total_distance(n, pop[pp]);
                if (pop[pp][0] >= 0 && cur_total > new_total) {
                    for (int ppp = ELITE - 2; ppp >= pp; ppp --) {
                        for (int j = 0; j < n; j ++) {
                            pop[ppp + 1][j] = pop[ppp][j];
                        }
                    }
                    for (int j = 0; j < n; j ++) {
                        pop[pp][j] = route[j];
                    }
                    break;
                } else if (pop[pp][0] < 0) {
                    for (int j = 0; j < n; j ++) {
                        pop[pp][j] = route[j];
                    }
                    break;
                }
            }
        }
        generated = 1;

        // mutate and crossover elites
        for (int p = ELITE; p < POP; p ++) {
            if ((double)rand() / (double)RAND_MAX < MUTATE_PROB) {
                int random_elite = rand() % ELITE;
                int random1 = rand() % n;
                int random2 = rand() % n;

                for (int i = 0; i < n; i ++) {
                    pop[p][i] = pop[random_elite][i];
                }
                pop[p][random2] = pop[random_elite][random1];
                pop[p][random1] = pop[random_elite][random2];
            } else {
                int random_elite1 = rand() % ELITE;
                int random_elite2 = random_elite1;
                while (random_elite2 == random_elite1) {
                    random_elite2 = rand() % ELITE;
                }
                int random = rand() % n;
                for (int i = 0; i < random; i ++) {
                    pop[p][i] = pop[random_elite1][i];
                }
                int nonpop[MAX_CITIES];
                for (int i = 0; i < n - random; i ++) {
                    nonpop[i] = pop[random_elite1][i + random];
                }
                int k = 0;
                for (int i = 0; i < n; i ++) {
                    if (exists(pop[random_elite2][i], n - random, nonpop) == 1) {
                        pop[p][random + k] = pop[random_elite2][i];
                        k ++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i ++) {
        route[i] = pop[0][i];
    }
    return total_distance(n, route);
}

double tabu_search(const int n, int route[])
{
    int M = n * (n - 1) / 2 * M_RATE;
    int tabu_count = 0;
    int tabu_list[TABU_SIZE][2];
    int route_best[MAX_CITIES];

    for (int i = 0; i < n; i ++) {
        route[i] = i;
    }
    for (int i = 0; i < n; i ++) {
        int j = rand() % n;
        int tmp = route[i];
        route[i] = route[j];
        route[j] = tmp;
    }

    for (int i = 0; i < n; i ++) {
        route_best[i] = route[i];
    }

    for (int iteration = 0; iteration < TABU_ITERATIONS; iteration ++) {
        double shortest_total = total_distance(n, route);
        int route_candidate[MAX_CITIES];
        int r1, r2;
        for (int i = 0; i < M; i ++) {
            int route_backup[MAX_CITIES];
            for (int j = 0; j < n; j ++) {
                route_backup[j] = route[j];
            }
            int random1 = rand() % n;
            int random2 = random1;
            while (random2 == random1) {
                random2 = rand() % n;
            }
            int tmp = route_backup[random1];
            route_backup[random1] = route_backup[random2];
            route_backup[random2] = tmp;
            double new_total = total_distance(n, route_backup);
            if (new_total < shortest_total) {
                for (int j = 0; j < n; j ++) {
                    route_candidate[j] = route_backup[j];
                }
                shortest_total = new_total;
                r1 = min(random1, random2);
                r2 = max(random1, random2);
            }
        }
        double candidate_total = total_distance(n, route_candidate);
        double best_total = total_distance(n, route_best);
        int index = -1;
        for (int i = 0; i < tabu_count; i ++) {
            if (tabu_list[i][0] == r1 && tabu_list[i][1] == r2) {
                index = i;
                break;
            }
        }
        if (candidate_total < best_total) {
            if (index != -1) {
                for (int k = index; k < tabu_count - 1; k ++) {
                    tabu_list[k][0] = tabu_list[k + 1][0];
                    tabu_list[k][1] = tabu_list[k + 1][1];
                }
                tabu_list[tabu_count - 1][0] = r1;
                tabu_list[tabu_count - 1][1] = r2;
            }
            for (int i = 0; i < n; i ++) {
                route_best[i] = route_candidate[i];
                route[i] = route_candidate[i];
            }
        } else {
            if (index == -1) {
                if (tabu_count < TABU_SIZE) {
                    tabu_list[tabu_count][0] = r1;
                    tabu_list[tabu_count][1] = r2;
                    tabu_count ++;
                } else {
                    for (int k = 0; k < tabu_count - 1; k ++) {
                        tabu_list[k][0] = tabu_list[k + 1][0];
                        tabu_list[k][1] = tabu_list[k + 1][1];
                    }
                    tabu_list[tabu_count - 1][0] = r1;
                    tabu_list[tabu_count - 1][1] = r2;
                }
                for (int i = 0; i < n; i ++) {
                    route[i] = route_candidate[i];
                }
            }
        }
    }
    for (int i = 0; i < n; i ++) {
        route[i] = route_best[i];
    }
    return total_distance(n, route);
}

int main(const int argc, const char **argv)
{
    FILE *fp;
    const char *map_file = "map.txt";
    if ((fp = fopen(map_file, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", map_file);
        return 1;
    }

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "error: please specify the number of cities.\n");
        exit(1);
    }

    int input;
    if (argc == 3) {
        input = atoi(argv[2]);
    }

    if (input < 1 || input > 4) {
        printf("Choose algorithm to solve the TSP problem.\n");
        printf("1. Hill Climbing\n");
        printf("2. Simulated Annealing\n");
        printf("3. Genetic Algorithm\n");
        printf("4. Tabu Search\n");
        while (input < 1 || input > 4) {
            printf("Input(1-4): ");
            scanf("%d", &input);
        }
    }

    const int n = atoi(argv[1]);
    assert(n > 1 && n <= MAX_CITIES);

    int i;
    for (i = 0; i < n; i++) {
        city[i].x = rand() % (WIDTH - 5);
        city[i].y = rand() % HEIGHT;
    }

    plot_cities(fp, n, NULL);
    sleep(1);

    int route[MAX_CITIES];
    double d = 0;
    switch(input) {
        case 1:
            d = hill_climbing(n, route);
            break;
        case 2:
            d = simulated_annealing(n, route);
            break;
        case 3:
            d = genetic_algorithm(n, route);
            break;
        case 4:
            d = tabu_search(n, route);
            break;
        default:
            printf("Error: False input\n");
            break;
    }
    printf("total distance = %f\n", d);
    plot_cities(fp, n, route);

    return 0;
}
