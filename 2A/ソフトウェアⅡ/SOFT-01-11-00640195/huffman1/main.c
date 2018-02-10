#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char **argv)
{
    if (argc == 2) {
        char *input_file = argv[1];
        char *output_file = malloc(sizeof(input_file) + sizeof(char) * 4);
        sprintf(output_file, "%s.huf", input_file);
        encode(input_file, output_file);
    } else if (argc == 3) {
        if (!strcmp(argv[1], "-d")) {
            char *input_file = argv[2];
            char *output_file = malloc(sizeof(input_file) + sizeof(char) * 4);
            sprintf(output_file, "%s.dec", input_file);
            decode(input_file, output_file);
        }
    } else {
        fprintf(stderr, "error: please specify the file.\n");
        exit(1);
    }

    return 0;
}
