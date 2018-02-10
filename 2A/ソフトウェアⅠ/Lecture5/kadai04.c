#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 128

char* archive_rle(char* raw, int length);

int main() {
    FILE *fpr;
    if ((fpr = fopen("data.txt", "r")) == NULL) {
        printf("data.txtが存在しません．\n");
        return 1;
    } else {
        char raw[LENGTH];
        fgets(raw, LENGTH, fpr);
        fclose(fpr);
        char *archived = archive_rle(raw, LENGTH);
        FILE *fpw;
        fpw = fopen("data.rle", "w");
        fputs(archived, fpw);
        fclose(fpw);
    }

    return 0;
}

char* archive_rle(char* raw, int length) {
    char *archived;
    archived = malloc(length);

    int index = 0;
    int count = 1;
    char current = '\0';
    for (int i = 0; i < strlen(raw); i ++) {
        if (raw[i] == current) {
            if (i == strlen(raw) - 1) {
                if (count == 8) {
                    archived[index] = current;
                    archived[index + 1] = count + '0';
                    archived[index + 2] = current;
                    archived[index + 3] = '1';
                    archived[index + 4] = '\0';
                } else {
                    count ++;
                    archived[index] = current;
                    archived[index + 1] = count + '0';
                    archived[index + 2] = '\0';
                }
            } else {
                if (count == 8) {
                    archived[index] = current;
                    archived[index + 1] = count + '0';
                    count = 1;
                    index += 2;
                } else {
                    count ++;
                }
            }
        } else if (current != '\0') {
            if (i == strlen(raw) - 1) {
                archived[index] = current;
                archived[index + 1] = count + '0';
                current = raw[i];
                count = 1;
                index += 2;
                archived[index] = current;
                archived[index + 1] = count + '0';
            } else {
                archived[index] = current;
                archived[index + 1] = count + '0';
                current = raw[i];
                count = 1;
                index += 2;
            }
        } else {
            current = raw[i];
        }
    }

    return archived;
}
