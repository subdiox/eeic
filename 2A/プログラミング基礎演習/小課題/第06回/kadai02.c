#include <stdio.h>
#include <string.h>
#define MAX_LEN 1000

int main(int argc, char *argv[]) {
    char buffer[MAX_LEN];
    int end = 1;

    if (argc != 3) {
        printf("Usage: %s [input file] [output file]\n", argv[0]);
        return 1;
    }

    FILE *ifp = fopen(argv[1], "r");
    if (ifp == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    FILE *ofp = fopen(argv[2], "w");

    while (fscanf(ifp, "%s ", buffer) != EOF) {
        if (end == 1) {
            if (buffer[0] >= 'a' && buffer[0] <= 'z') {
                buffer[0] -= 32;
            } else {
                buffer[1] -= 32;
            }
        }

        if (buffer[strlen(buffer) - 1] == ('.' | '!' | '?')) {
            end = 1;
        } else {
            end = 0;
        }

        if (strcmp(buffer, "i") == 0) {
            buffer[0] = 'I';
        }
        if (buffer[0] == 'i' && buffer[1] == '\'') {
            buffer[0] = 'I';
        }
        
        if (0 > fprintf(ofp, "%s ", buffer)) {
            printf("Failed to output file\n");
            return 1;
        }
    }
    fclose(ifp);
    fclose(ofp);

    return 0;
}
