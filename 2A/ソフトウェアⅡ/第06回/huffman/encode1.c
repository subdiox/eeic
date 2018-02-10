#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "encode1.h"

#define NSYMBOLS 256

static int count;
static int symbol_count[NSYMBOLS];
static char symbol_list[NSYMBOLS][NSYMBOLS];
static char code[NSYMBOLS];
//static char tree[NSYMBOLS];

typedef struct node
{
    int symbol;
    int count;
    struct node *parent;
    struct node *left;
    struct node *right;
} Node;

static void count_symbols(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "error: cannot open %s\n", filename);
        exit(1);
    }

    int i;
    for (i = 0; i < NSYMBOLS; i++) {
        symbol_count[i] = 0;
    }

    unsigned char byte;
    while((fread(&byte, 1, 1, fp)) != 0) {
        symbol_count[(int)byte] ++;
    }

    //symbol_count[NSYMBOLS-1] ++; // End of File

    fclose(fp);
}

static Node *pop_min(int *n, Node *nodep[])
{
    // Find the node with the smallest count
    int i, j = 0;
    for (i = 0; i < *n; i++) {
        if (nodep[i]->count < nodep[j]->count) {
            j = i;
        }
    }

    Node *node_min = nodep[j];

    // Remove the node pointer from nodep[]
    for (i = j; i < (*n) - 1; i++) {
        nodep[i] = nodep[i + 1];
    }
    (*n)--;

    return node_min;
}

static Node *build_tree()
{
    int i, n = 0;
    Node *nodep[NSYMBOLS];

    for (i = 0; i < NSYMBOLS; i++) {
        if (symbol_count[i] == 0) continue;
        nodep[n] = malloc(sizeof(Node));
        nodep[n]->symbol = i;
        nodep[n]->count  = symbol_count[i];
        nodep[n]->parent = NULL;
        nodep[n]->left   = NULL;
        nodep[n]->right  = NULL;
        n++;
    }

    count = n;

    while (n >= 2) {
        Node *node1 = pop_min(&n, nodep);
        Node *node2 = pop_min(&n, nodep);

        // Create a new node
        nodep[n] = malloc(sizeof(Node));
        nodep[n]->symbol = '\0';
        nodep[n]->count  = node1->count + node2->count;
        nodep[n]->left   = node1;
        nodep[n]->right  = node2;

        node1->parent = nodep[n];
        node2->parent = nodep[n];
        n++;
    }

    return nodep[0];
}

// Perform depth-first traversal of the tree
static void traverse_tree(const int depth, const Node *np)
{
    assert(depth < NSYMBOLS);

    if (np->left == NULL) {
        code[depth] = '\0';
        strcpy(symbol_list[np->symbol], code);
        printf("symbol: %c, code: %s\n", np->symbol, code);
        return;
    }

    code[depth] = '0';
    traverse_tree(depth + 1, np->left);
    code[depth] = '1';
    traverse_tree(depth + 1, np->right);
}

// intを0と1のビット列に変換
char* itob(int i, int size) {
    assert ((0x1 << size) > i);

    char *b = malloc(sizeof(char) * size);
    int j = 0;
    for (int mask = (0x1 << (size - 1)); mask > 0; mask >>= 1){
        b[j] = mask & i ? '1' : '0';
        j ++;
    }
    b[size] = '\0';
    return b;
}

void encode_tree(FILE *fp)
{
    /*
    static int pos = 0;
    if (np->left == NULL) {
        tree[pos] = '1';
        pos ++;
        char *value = itob(np->symbol);
        printf("value: %s\n", value);
        strncpy(tree + pos, value, 8);
        pos += 8;
        //printf("tree: %s\n", tree);
    } else {
        tree[pos] = '0';
        pos ++;
        encode_tree(np->left);

        tree[pos] = '0';
        pos ++;
        encode_tree(np->right);
    }
    */
    for (int i = 0; i < NSYMBOLS; i ++) {
        if (symbol_count[i] == 0) continue;
        char *symbol = symbol_list[i];
        int size = strlen(symbol);
        unsigned int value = strtol(symbol, NULL, 2);
        fwrite(&i, 2, 1, fp);
        fwrite(&size, 2, 1, fp);
        fwrite(&value, 4, 1, fp);
    }
}

/*
    メタデータの仕様
    - 4 bytes : シグネチャ(0x68756666)
    - 2 bytes : 展開に必要なバージョン
    - 2 bytes : ハフマン木に含まれるデータ数(=x)
    - 6 * x bytes : ハフマン木をそのまま出力したデータ
*/
void add_metadata(FILE *fp, Node *root)
{
    char sign[] = "huff";
    fwrite(&sign, 1, 4, fp);
    int version = 100;
    fwrite(&version, 2, 1, fp);
    fwrite(&count, 2, 1, fp);
    encode_tree(fp);
/*
    int i;
    for (i = 0; i < pos; i += 8) {
        char *out_bytes = malloc(sizeof(char) * 8);
        strncpy(out_bytes, tree + i, 8);
        out_bytes[8] = '\0';
        int out = strtol(out_bytes, NULL, 2);
        fwrite(&out, 1, 1, fp);
    }
    // 常に最後は0で終わるように0埋めする
    char *out_bytes = malloc(sizeof(char) * 8);
    strncpy(out_bytes, tree + i, 8);
    int out = (strtol(out_bytes, NULL, 2) << (8 - strlen(out_bytes))) + (1 << (7 - strlen(out_bytes)));
    fwrite(&out, 1, 1, fp);
*/
}

void output_data(const char *input_file, const char *output_file, Node *root)
{
    FILE *ifp = fopen(input_file, "rb");
    FILE *ofp = fopen(output_file, "wb");
    if (ifp == NULL) {
        fprintf(stderr, "error: cannot open %s\n", input_file);
        exit(1);
    }
    if (ofp == NULL) {
        fprintf(stderr, "error: cannot open %s\n", output_file);
        exit(1);
    }

    add_metadata(ofp, root);

    /*
        - buff: 入力データから1バイト取得したものの領域
        - bytes: 0と1が8つたまるまで保存しておく領域
        - byte: buffに対応するシンボル列(0と1の列)
        - out_bytes: 出力するときに使うシンボル列
        - new_bytes: bytesからout_bytesを取り除いたシンボル列
    */
    char buff;
    char *bytes = malloc(sizeof(char) * NSYMBOLS);
    while((fread(&buff, 1, 1, ifp)) != 0) {
        char *byte = symbol_list[(int)buff];
        strcat(bytes, byte);
        //printf("buff: %c, byte: %s, bytes: %s\n", buff, byte, bytes);
        while (strlen(bytes) >= 8) {
            char *out_bytes = malloc(sizeof(char) * 8);
            strncpy(out_bytes, bytes, 8);
            out_bytes[8] = '\0';
            int out = strtol(out_bytes, NULL, 2);
            fwrite(&out, 1, 1, ofp);

            char *new_bytes = malloc(sizeof(char) * 16);
            strncpy(new_bytes, bytes + 8, strlen(bytes) - 8);
            new_bytes[strlen(bytes) - 8] = '\0';
            bytes = malloc(sizeof(char) * NSYMBOLS);
            strcpy(bytes, new_bytes);
        }
    }
    // 常に最後は0で終わるように0埋めする
    int out = strtol(bytes, NULL, 2) << (8 - strlen(bytes));
    fwrite(&out, 1, 1, ofp);

    fclose(ifp);
    fclose(ofp);
}

void encode(const char *input_file, const char *output_file)
{
    count_symbols(input_file);
    Node *root = build_tree();
    traverse_tree(0, root);
    //output_data(input_file, output_file, root);
}
