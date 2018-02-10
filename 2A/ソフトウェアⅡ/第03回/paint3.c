#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define WIDTH 70
#define HEIGHT 40
#define BUFSIZE 1000
#define COLOR_BIT 24	/* 色ビット */
#define PIXEL_RATE 10
#define PIXEL_NUM_X WIDTH * PIXEL_RATE
#define PIXEL_NUM_Y HEIGHT * PIXEL_RATE
#define PIC_DATA_SIZE PIXEL_NUM_X * PIXEL_NUM_Y * 3	/* bitmapのサイズ */

typedef struct node
{
    char *str;
    struct node *next;
} Node;

Node *history = NULL;
int canvas[WIDTH][HEIGHT];
int clipboard[WIDTH][HEIGHT];
int clipboard_width = 0;
int clipboard_height = 0;
const char *default_history_file = "history.txt";
const char *default_bitmap_file = "canvas.bmp";
char c = '#';

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
void fill_rectangle(int x0, int y0, int x1, int y1);
void draw_circle(const int x, const int y, const int r);
void fill_circle(const int x, const int y, const int r);
void copy(int x0, int y0, int x1, int y1);
void paste(const int x, const int y);
void save_history(const char *filename);
void load_history(const char *filename, int *hsize);
int interpret_command(const char *command, int *hsize);
int createBitmap(const char *filename);
void createPic(unsigned char *b, int x, int y);
int putBmpHeader(FILE *s, int x, int y, int c);
int fputc2LowHigh(unsigned short d, FILE *s);
int fputc4LowHigh(unsigned long d, FILE *s);

int main()
{
    const char *filename = "canvas.txt";
    FILE *fp;
    char buf[BUFSIZE];

    if ((fp = fopen(filename, "a")) == NULL) {
        fprintf(stderr, "Error: cannot open %s.\n", filename);
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
    if (begin == NULL) return begin;
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
            if (canvas[x][y] == 1) {
                fputc(c, fp);
            } else {
                fputc(' ', fp);
            }
        }
        fputc('\n', fp);
    }
    fflush(fp);
}

void init_canvas()
{
    memset(canvas, 0, sizeof(canvas));
}

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}

void draw_line(const int x0, const int y0, const int x1, const int y1)
{
    int i;
    const int n = max(abs(x1 - x0), abs(y1 - y0));
    if (n == 0) {
        canvas[x0][y0] = 1;
    } else {
        for (i = 0; i <= n; i++) {
            const int x = x0 + i * (x1 - x0) / n;
            const int y = y0 + i * (y1 - y0) / n;
            canvas[x][y] = 1;
        }
    }
}

void draw_rectangle(const int x0, const int y0, const int x1, const int y1)
{
    draw_line(x0, y0, x1, y0);
    draw_line(x0, y0, x0, y1);
    draw_line(x1, y1, x1, y0);
    draw_line(x1, y1, x0, y1);
}

void fill_rectangle(int x0, int y0, int x1, int y1)
{
    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1) {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    for (int i = x0; i <= x1; i ++) {
        for (int j = y0; j <= y1; j ++) {
            canvas[i][j] = 1;
        }
    }
}

void draw_circle(const int x, const int y, const int r)
{
    for (int i = 0; i < WIDTH; i ++) {
        for (int j = 0; j < HEIGHT; j ++) {
            int distance = (pow(x - i, 2) + pow(y - j, 2));
            if (distance > (r - 1) * r && distance < r * (r + 1)) {
                canvas[i][j] = 1;
            }
        }
    }
}

void fill_circle(const int x, const int y, const int r)
{
    for (int i = 0; i < WIDTH; i ++) {
        for (int j = 0; j < HEIGHT; j ++) {
            int distance = (pow(x - i, 2) + pow(y - j, 2));
            if (distance <= pow(r, 2)) {
                canvas[i][j] = 1;
            }
        }
    }
}

void copy(int x0, int y0, int x1, int y1)
{
    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1) {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    for (int i = x0; i < x1; i ++) {
        for (int j = y0; j < y1; j ++) {
            clipboard[i - x0][j - y0] = canvas[i][j];
        }
    }
    clipboard_width = x1 - x0;
    clipboard_height = y1 - y0;
    printf("copied (%d, %d) -> (%d, %d) to clipboard\n", x0, y0, x1, y1);
}

void paste(const int x, const int y)
{
    for (int i = x; i < x + clipboard_width; i ++) {
        for (int j = y; j < y + clipboard_height; j ++) {
            if (clipboard[i - x][j - y] == 1) {
                canvas[i][j] = 1;
            }
        }
    }
    printf("pasted clipboard\n");
}

void save_history(const char *filename)
{
    if (filename == NULL)
        filename = default_history_file;

    FILE *fp;
    if ((fp = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "Error: cannot open %s.\n", filename);
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
        fprintf(stderr, "Error: cannot open %s.\n", filename);
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

int* split(char* delimiter, int count)
{
    int* list = malloc(sizeof(int) * (count + 1));
    for (int i = 0; i <= count; i ++) {
        list[i] = 0;
    }
    int offset = 0;
    for (int i = 0; i < count + offset; i ++) {
        char* c = strtok(NULL, delimiter);
        if (c == NULL) {
            list[i - offset] = -1;
        } else {
            if (strcmp(c, "-f") == 0) {
                list[count] = 1;
                offset ++;
                continue;
            }
            char *endp;
            list[i - offset] = strtol(c, &endp, 10);
            if (strcmp(endp, "") != 0) {
                list[i - offset] = -2;
            } else if (list[i] < 0) {
                list[i - offset] = -3;
            }
        }
    }
    return list;
}

int check_list(int* list, int count)
{
    for (int i = 0; i < count; i ++) {
        if (list[i] == -1) return -1;
        if (list[i] == -2) return -2;
        if (list[i] == -3) return -3;
    }
    return 1;
}

void print_error(int code) {
    switch(code) {
        case -1:
            printf("Error: not enough arguments.\n");
            break;
        case -2:
            printf("Error: non-number is included in arguments.\n");
            break;
        case -3:
            printf("Error: minus values are not allowed.\n");
            break;
        default:
            printf("Error: unknown error occurred.\n");
            break;
    }
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

    if (s == NULL) {
        return 1;
    }

    if (strcmp(s, "help") == 0) {
        printf("\
Paint Software Ver 1.0\n\
Copyright (c) 2017, Yuta Ooka. All rights reserved.\n\
\n\
Usage: [COMMAND] [OPTION] [ARGUMENTS]\n\
  help                        Show this help.\n\
  line x1 y1 x2 y2            Draw a line from (x1, y1) to (x2, y2).\n\
  rectangle (-f) x1 y1 x2 y2  Draw a rectangle, whose upper-left coordinate is (x1, y1),\n\
                              and whose lower-right coordinate is (x2, y2).\n\
                              If you put -f option, the rectangle will be filled.\n\
  circle (-f) x y r           Draw a circle whose center coordinate is (x, y),\n\
                              and whose radius is r.\n\
                              If you put -f option, the circle will be filled.\n\
  effect c                    Add effect to the canvas with character c.\n\
  copy x1 y1 x2 y2            Copy an area whose upper-left coordinate is (x1, y1),\n\
                              and whose lower-right coordinate is (x2, y2) to clipboard.\n\
  paste x y                   Paste clipboard to the upper-left position of (x, y).\n\
  undo                        Undo one step you did before.\n\
  save (filename)             Save the history as a file with specified filename.\n\
                              If filename is not specified, save it as \"history.txt\".\n\
  load (filename)             Load a history file with specified filename.\n\
                              If filename is not specified, load \"history.txt\".\n\
  export (filename)           Create a bitmap file from canvas with specified filename.\n\
                              If filename is not specified, export it as \"canvas.bmp\".\n\
  quit                        Quit this program.\n\
");
        return 1;
    }

    if (strcmp(s, "line") == 0) {
        int x0, y0, x1, y1;
        int* list = split(" ", 4);
        int ok = check_list(list, 4);
        if (ok == 1) {
            x0 = list[0];
            y0 = list[1];
            x1 = list[2];
            y1 = list[3];
            draw_line(x0, y0, x1, y1);
            return 0;
        } else {
            print_error(ok);
            return 1;
        }
    }

    if (strcmp(s, "rectangle") == 0) {
        int x0, y0, x1, y1;
        int* list = split(" ", 4);
        int ok = check_list(list, 4);
        if (ok == 1) {
            x0 = list[0];
            y0 = list[1];
            x1 = list[2];
            y1 = list[3];
            if (list[4] == 1) {
                fill_rectangle(x0, y0, x1, y1);
            } else {
                draw_rectangle(x0, y0, x1, y1);
            }
            return 0;
        } else {
            print_error(ok);
            return 1;
        }
    }

    if (strcmp(s, "circle") == 0) {
        int x, y, r;
        int* list = split(" ", 3);
        int ok = check_list(list, 3);
        if (ok == 1) {
            x = list[0];
            y = list[1];
            r = list[2];
            if (list[3] == 1) {
                fill_circle(x, y, r);
            } else {
                draw_circle(x, y, r);
            }
            return 0;
        } else {
            print_error(ok);
            return 1;
        }
    }

    if (strcmp(s, "effect") == 0) {
        c = *strtok(NULL, " ");
        init_canvas();
        Node *begin = history;
        while (begin != NULL) {
            printf("%s", begin->str);
            interpret_command(begin->str, NULL);
            begin = begin->next;
        }
        return 1;
    }

    if (strcmp(s, "copy") == 0) {
        int x0, y0, x1, y1;
        int* list = split(" ", 4);
        int ok = check_list(list, 4);
        if (ok == 1) {
            x0 = list[0];
            y0 = list[1];
            x1 = list[2];
            y1 = list[3];
            copy(x0, y0, x1, y1);
            return 0;
        } else {
            print_error(ok);
            return 1;
        }
    }

    if (strcmp(s, "paste") == 0) {
        int x, y;
        int* list = split(" ", 2);
        int ok = check_list(list, 2);
        if (ok == 1) {
            x = list[0];
            y = list[1];
            paste(x, y);
            return 0;
        } else {
            print_error(ok);
            return 1;
        }
    }

    if (strcmp(s, "undo") == 0) {
        if (*hsize == 0) {
            printf("Error: cannot undo anymore.\n");
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

    if (strcmp(s, "export") == 0) {
        s = strtok(NULL, " ");
        createBitmap(s);
        return 1;
    }

    if (strcmp(s, "quit") == 0) {
        return 2;
    }

    printf("Error: unknown command.\n");
    return 1;
}

int createBitmap(const char *filename) {
    if (filename == NULL)
        filename = default_bitmap_file;

    FILE *f;
	int r;
	unsigned char *b;

	/* bitmap用メモリを確保 */
	b = malloc(PIC_DATA_SIZE);
	if (b == NULL) {
		return EXIT_FAILURE;
	}

	/* 画像を生成 */
	createPic(b, PIXEL_NUM_X, PIXEL_NUM_Y);

	/* ファイルをオープン */
	f = fopen(filename, "wb");
	if (f == NULL) {
		return EXIT_FAILURE;
	}

	/* ヘッダの書出 */
	r = putBmpHeader(f, PIXEL_NUM_X, PIXEL_NUM_Y, COLOR_BIT);
	if (!r) {
		fclose(f);
		return EXIT_FAILURE;
	}

	/* bitmapの書出 */
	r = fwrite(b, sizeof(unsigned char), PIC_DATA_SIZE, f);
	if (r != PIC_DATA_SIZE) {
		fclose(f);
		return EXIT_FAILURE;
	}

	/* ファイルをクローズし,処理終了 */
	fclose(f);

    printf("exported as \"%s\"\n", filename);
    return EXIT_SUCCESS;
}

/* 画像生成関数 */
void createPic(unsigned char *b, int x, int y)
{
	unsigned char black = 0;
    unsigned char white = 255;

	/* データを生成 */
	for (int j = y - 1; j >= 0; j --) {
		for (int i = 0; i < x; i ++) {
            if (canvas[i / PIXEL_RATE][j / PIXEL_RATE] == 1) {
                for (int k = 0; k < 3; k ++) {
                    *b = black;
                    b ++;
                }
            } else {
                for (int k = 0; k < 3; k ++) {
                    *b = white;
                    b ++;
                }
            }
		}
	}
}

/*
	putBmpHeader BMPヘッダ書出

	BMPファイルのヘッダを書き出す

	●戻り値
		int:0…失敗, 1…成功
	●引数
		FILE *s:[i] 出力ストリーム
		int x:[i] 画像Xサイズ(dot, 1〜)
		int y:[i] 画像Yサイズ(dot, 1〜)
		int c:[i] 色ビット数(bit/dot, 1 or 4 or 8 or 24)
*/
int putBmpHeader(FILE *s, int x, int y, int c)
{
	int i;
	int color; /* 色数 */
	unsigned long int bfOffBits; /* ヘッダサイズ(byte) */

	/* 画像サイズが異常の場合,エラーでリターン */
	if (x <= 0 || y <= 0) {
		return 0;
	}

	/* 出力ストリーム異常の場合,エラーでリターン */
	if (s == NULL || ferror(s)) {
		return 0;
	}

	/* 色数を計算 */
	if (c == 24) {
		color = 0;
	} else {
		color = 1;
		for (i=1;i<=c;i++) {
			color *= 2;
		}
	}

	/* ヘッダサイズ(byte)を計算 */
	/* ヘッダサイズはビットマップファイルヘッダ(14) + ビットマップ情報ヘッダ(40) + 色数 */
	bfOffBits = 14 + 40 + 4 * color;

	/* ビットマップファイルヘッダ(計14byte)を書出 */
	/* 識別文字列 */
	fputs("BM", s);

	/* bfSize ファイルサイズ(byte) */
	fputc4LowHigh(bfOffBits + (unsigned long)x * y, s);

	/* bfReserved1 予約領域1(byte) */
	fputc2LowHigh(0, s);

	/* bfReserved2 予約領域2(byte) */
	fputc2LowHigh(0, s);

	/* bfOffBits ヘッダサイズ(byte) */
	fputc4LowHigh(bfOffBits, s);

	/* ビットマップ情報ヘッダ(計40byte) */
	/* biSize 情報サイズ(byte) */
	fputc4LowHigh(40, s);

	/* biWidth 画像Xサイズ(dot) */
	fputc4LowHigh(x, s);

	/* biHeight 画像Yサイズ(dot) */
	fputc4LowHigh(y, s);

	/* biPlanes 面数 */
	fputc2LowHigh(1, s);

	/* biBitCount 色ビット数(bit/dot) */
	fputc2LowHigh(c, s);

	/* biCompression 圧縮方式 */
	fputc4LowHigh(0, s);

	/* biSizeImage 圧縮サイズ(byte) */
	fputc4LowHigh(0, s);

	/* biXPelsPerMeter 水平解像度(dot/m) */
	fputc4LowHigh(0, s);

	/* biYPelsPerMeter 垂直解像度(dot/m) */
	fputc4LowHigh(0, s);

	/* biClrUsed 色数 */
	fputc4LowHigh(0, s);

	/* biClrImportant 重要色数 */
	fputc4LowHigh(0, s);

	/* 書出失敗ならエラーでリターン */
	if (ferror(s)) {
		return 0;
	}

	/* 成功でリターン */
	return 1;
}

/*
	fputc2LowHigh 2バイトデータ書出(下位〜上位)

	2バイトのデータを下位〜上位の順で書き出す

	●戻り値
		int:EOF…失敗, EOF以外…成功
	●引数
		unsigned short d:[i] データ
		FILE *s:[i] 出力ストリーム
*/
int fputc2LowHigh(unsigned short d, FILE *s)
{
	putc(d & 0xFF, s);
	return putc(d >> CHAR_BIT, s);
}

/*
	fputc4LowHigh 4バイトデータ書出(下位〜上位)

	4バイトのデータを下位〜上位の順で書き出す

	●戻り値
		int:EOF…失敗, EOF以外…成功
	●引数
		unsigned long d:[i] データ
		FILE *s:[i] 出力ストリーム
*/
int fputc4LowHigh(unsigned long d, FILE *s)
{
	putc(d & 0xFF, s);
	putc((d >> CHAR_BIT) & 0xFF, s);
	putc((d >> CHAR_BIT * 2) & 0xFF, s);
	return putc((d >> CHAR_BIT * 3) & 0xFF, s);
}
