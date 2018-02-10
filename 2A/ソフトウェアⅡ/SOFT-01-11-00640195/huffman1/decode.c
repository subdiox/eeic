#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

#define NSYMBOLS 256

char symbol_list[NSYMBOLS][NSYMBOLS];
extern char* itob(int i, int size);

void parse_tree(FILE *fp, int count) {
    for (int i = 0; i < count; i ++) {
        int value, size, symbol;
        fread(&value, 2, 1, fp);
        fread(&size, 2, 1, fp);
        fread(&symbol, 4, 1, fp);
        strcpy(symbol_list[value], itob(symbol, size));
    }
}

void parse_metadata(FILE *fp) {
    char sign[5];
    fread(&sign, 4, 1, fp);
    sign[4] = '\0';
    if (!strcmp(sign, "huff")) {
        int version;
        fread(&version, 2, 1, fp);
        if (version >= 100) {
            int count;
            fread(&count, 2, 1, fp);
            parse_tree(fp, count);
        }
    } else {
        fprintf(stderr, "error: this is not HUF format file\n");
        exit(1);
    }
}

char *pop_left(char *bytes, int length) {
    char *new_bytes = malloc(sizeof(char) * (length + 1));
    strncpy(new_bytes, bytes + length, strlen(bytes) - length);
    return new_bytes;
}
/*
Node *parse_tree(FILE *fp, int size) {
    char *bytes = malloc(sizeof(char) * NSYMBOLS);
    int byte;
    Node *root = malloc(sizeof(Node));
    Node *current = root;
    for (int i = 0; i < size - 1; i ++) {
        fread(&byte, 1, 1, fp);
        char *binary = itob(byte);
        strncpy(bytes + strlen(bytes), binary, 8);
        while (strlen(bytes) >= 8) {
            if (bytes[0] == '0') {
                Node *new_node = malloc(sizeof(Node));
                if (current->right != NULL) {
                    current = current->right;
                } else if (current->left != NULL) {
                    current = current->left;
                } else {
                    current = current->left;
                }

                bytes = pop_left(bytes, 1);
            } else if (bytes[0] == '1') {
                int value;
                fread(&value, 1, 1, fp);
                Node *new_node = malloc(sizeof(Node));
                new_node->symbol = value;
                new_node->left = NULL;
                new_node->right = NULL;
                if (current->left == NULL) {
                    current->left = new_node;
                } else if (cur->right == NULL) {
                    current->right = new_node;
                } else {
                    fprintf(stderr, "error: huffman tree is broken\n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "error: internal error occurred\n");
                exit(1);
            }
        }
    }
}*/

void decode(const char *input_file, const char *output_file) {
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

    parse_metadata(ifp);

    char buff;
    char bytes[NSYMBOLS];
    while((fread(&buff, 1, 1, ifp)) != 0) {
        char *value = itob(buff, 8);
        strcat(bytes, value);
        printf("bytes: %s\n", bytes);
        for (int i = 1; i < strlen(bytes); i ++) {
            char *string = malloc(sizeof(char) * strlen(bytes));
            strncpy(string, bytes, i);
            printf("string: %s\n", string);
            for (int j = 0; j < NSYMBOLS; j ++) {
                if (!strcmp(string, symbol_list[j])) {
                    fwrite(&j, 1, 1, ofp);
                    pop_left(bytes, 8);
                    break;
                }
            }
        }
    }
}