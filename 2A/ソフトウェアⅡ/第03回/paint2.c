#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#define WIDTH 70
#define HEIGHT 40

#define BUFSIZE 1000

typedef struct node
{
    char *str;
    struct node *next;
} Node;

Node *history = NULL;
char canvas[WIDTH][HEIGHT];
const char *default_history_file = "history.txt";

Node *push_front(Node *begin, const char *str);
Node *pop_front(Node *begin);
Node *push_back(Node *begin, const char *str);
Node *pop_back(Node *begin);
Node *remove_all(Node *begin);
void print_canvas(FILE *fp);
void init_canvas();
int max(const int a, const int b);
void print_canvas(FILE *fp);
void draw_line(const int x0, const int y0, const int x1, const int y1);
void draw_rectangle(const int x0, const int y0, const int x1, const int y1);
void draw_circle(const int x, const int y, const int r);
void save_history(const char *filename);
void load_history(const char *filename, int *hsize);
int interpret_command(const char *command, int *hsize);

int main()
{
    const char *filename = "canvas.txt";
    FILE *fp;
    char buf[BUFSIZE];

    if ((fp = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return 1;
    }

    init_canvas();
    print_canvas(fp);

    int index = 0;

    while (1) {
        printf("%d > ", index);
        fgets(buf, BUFSIZE, stdin);

        const int r = interpret_command(buf, &index);
        if (r == 2) break;
        if (r == 0) {
            history = push_back(history, buf);
            index ++;
        }

        print_canvas(fp);
    }

    fclose(fp);

    return 0;
}

Node *push_front(Node *begin, const char *str)
{
    // Create a new element
    Node *p = (Node *)malloc(sizeof(Node));
    char *s = (char *)malloc(strlen(str) + 1);
    strcpy(s, str);
    p->str = s;
    p->next = begin;

    return p; // Now the new element is the first element in the list
}

Node *pop_front(Node *begin)
{
    assert(begin != NULL); // Don't call pop_front() when the list is empty
    Node *p = begin->next;

    free(begin->str);
    free(begin);

    return p;
}

Node *push_back(Node *begin, const char *str)
{
    if (begin == NULL) { // If the list is empty
        return push_front(begin, str);
    }

    // Find the last element
    Node *p = begin;
    while (p->next != NULL) {
        p = p->next;
    }

    // Create a new element
    Node *q = (Node *)malloc(sizeof(Node));
    char *s = (char *)malloc(strlen(str) + 1);
    strcpy(s, str);
    q->str = s;
    q->next = NULL;

    // The new element should be linked from the previous last element
    p->next = q;

    return begin;
}

Node *pop_back(Node *begin)
{
    assert(begin != NULL); // Don't call pop_back() when the list is empty

    if (begin->next == NULL) {
        return NULL;
    }
    Node *p = begin;
    while (p->next != NULL) {
        if (p->next->next != NULL) {
            p = p->next;
        } else {
            p->next = NULL;
            break;
        }
    }

    return begin;
}

Node *remove_all(Node *begin)
{
    while ((begin = pop_front(begin)))
        ; // Repeat pop_front() until the list becomes empty
    return begin;    // Now, begin is NULL
}

void print_canvas(FILE *fp)
{
    int x, y;

    fprintf(fp, "----------\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            const char c = canvas[x][y];
            fputc(c, fp);
        }
        fputc('\n', fp);
    }
    fflush(fp);
}

void init_canvas()
{
    memset(canvas, ' ', sizeof(canvas));
}

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}

void draw_line(const int x0, const int y0, const int x1, const int y1)
{
    int i;
    const int n = max(abs(x1 - x0), abs(y1 - y0));
    for (i = 0; i <= n; i++) {
        const int x = x0 + i * (x1 - x0) / n;
        const int y = y0 + i * (y1 - y0) / n;
        canvas[x][y] = '#';
    }
}

void draw_rectangle(const int x0, const int y0, const int x1, const int y1)
{
    draw_line(x0, y0, x1, y0);
    draw_line(x0, y0, x0, y1);
    draw_line(x1, y1, x1, y0);
    draw_line(x1, y1, x0, y1);
}

void draw_circle(const int x, const int y, const int r)
{
    for (int i = 0; i < WIDTH; i ++) {
        for (int j = 0; j < HEIGHT; j ++) {
            int distance = (pow(x - i, 2) + pow(y - j, 2));
            if (distance > (r - 1) * r && distance < r * (r + 1)) {
                canvas[i][j] = '#';
            }
        }
    }
}

void save_history(const char *filename)
{
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return;
    }

    Node *begin = history;
    while (begin != NULL) {
        fprintf(fp, "%s", begin->str);
        begin = begin->next;
    }

    printf("saved as \"%s\"\n", filename);

    fclose(fp);
}

void load_history(const char *filename, int *hsize)
{
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "error: cannot open %s.\n", filename);
        return;
    }

    char *command = malloc(sizeof(BUFSIZE));
    init_canvas();
    history = remove_all(history);
    while (fgets(command, BUFSIZE, fp) != NULL) {
        interpret_command(command, hsize);
        history = push_front(history, command);
    }

    printf("loaded \"%s\"\n", filename);

    fclose(fp);
}

// Interpret and execute a command
//     return value:
//         0, normal commands such as "line"
//         1, unknown or special commands (not recorded in history[])
//         2, quit
int interpret_command(const char *command, int *hsize)
{
    char buf[BUFSIZE];
    strcpy(buf, command);
    buf[strlen(buf) - 1] = 0; // remove the newline character at the end

    const char *s = strtok(buf, " ");

    if (strcmp(s, "line") == 0) {
        int x0, y0, x1, y1;
        x0 = atoi(strtok(NULL, " "));
        y0 = atoi(strtok(NULL, " "));
        x1 = atoi(strtok(NULL, " "));
        y1 = atoi(strtok(NULL, " "));
        draw_line(x0, y0, x1, y1);
        return 0;
    }

    if (strcmp(s, "rectangle") == 0) {
        int x0, y0, x1, y1;
        x0 = atoi(strtok(NULL, " "));
        y0 = atoi(strtok(NULL, " "));
        x1 = atoi(strtok(NULL, " "));
        y1 = atoi(strtok(NULL, " "));
        draw_rectangle(x0, y0, x1, y1);
        return 0;
    }

    if (strcmp(s, "circle") == 0) {
        int x, y, r;
        x = atoi(strtok(NULL, " "));
        y = atoi(strtok(NULL, " "));
        r = atoi(strtok(NULL, " "));
        draw_circle(x, y, r);
        return 0;
    }

    if (strcmp(s, "save") == 0) {
        s = strtok(NULL, " ");
        save_history(s);
        return 1;
    }

    if (strcmp(s, "load") == 0) {
        s = strtok(NULL, " ");
        load_history(s, hsize);
        return 1;
    }

    if (strcmp(s, "undo") == 0) {
        if (*hsize == 0) {
            printf("error: cannot undo anymore.\n");
            return 1;
        }
        init_canvas();

        history = pop_back(history);
        Node *begin = history;
        while (begin != NULL) {
            printf("%s", begin->str);
            interpret_command(begin->str, NULL);
            begin = begin->next;
        }
        (*hsize) --;
        return 1;
    }

    if (strcmp(s, "quit") == 0) {
        return 2;
    }

    printf("error: unknown command.\n");

    return 1;
}
