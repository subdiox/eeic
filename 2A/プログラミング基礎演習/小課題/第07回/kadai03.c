#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
    char name[20];
    int age;
    double weight;
    double height;
    struct person *previous;
    struct person *next;
};

struct person *first, *last;

void delete(struct person *p, char *s) {
    while (p != NULL) {
        if (strcmp(p -> name, s) == 0) {
            if (p -> previous == NULL) {
                p -> next -> previous = NULL;
                first = p -> next;
            } else if (p -> next == NULL) {
                p -> previous -> next = NULL;
                last = p -> previous;
            } else {
                p -> previous -> next = p -> next;
                p -> next -> previous = p -> previous;
            }
        }
        p = p -> next;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [input file]\n", argv[0]);
        return 1;
    }

	FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Failed to open %s\n", argv[1]);
        return 1;
    }

    struct person *p1;
    p1 = NULL;

    char name[20];
    int age;
    double weight;
    double height;

    while (fscanf(fp, "%[^,],%d,%lf,%lf\n", name, &age, &height, &weight) != EOF) {
        struct person *p2;
        p2 = (struct person *) malloc(sizeof(struct person));
        strcpy(p2 -> name, name);
        p2 -> age = age;
        p2 -> height = height;
        p2 -> weight = weight;
        p2 -> next = NULL;
        if (p1 == NULL) {
            p2 -> previous = NULL;
            first = p2;
        } else {
            p2 -> previous = p1;
            p1 -> next = p2;
        }
        p1 = p2;
    }
    last = p1;
    fclose(fp);

    char del_name[20];
    printf("誰のデータを消去しますか？\n");
    scanf("%s", del_name);
    delete(first, del_name);

    for (p1 = first; p1 != NULL; p1 = p1 -> next) {
        printf("name: %s, age: %d, weight: %f, height: %f\n", p1 -> name, p1 -> age, p1 -> weight, p1 -> height);
    }

	return 0;
}
