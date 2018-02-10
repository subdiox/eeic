#include <stdio.h>
#include <math.h>
#define MAX_LEN 1000

int main() {
    char buffer[MAX_LEN];
    int count, age, max[3], min[3];
    double height, weight, sum[3], sum2[3], mean[3], stdev[3];
	FILE *fp = fopen("namelist.txt", "r");
    if (fp == NULL) {
        printf("Failed to open namelist.txt\n");
        return 1;
    }

    while (fscanf(fp, "%[^,],%d,%lf,%lf\n", buffer, &age, &height, &weight) != EOF) {
        double array[3];
        array[0] = age;
        array[1] = height;
        array[2] = weight;
        for (int i = 0; i < 3; i ++) {
            sum[i] += array[i];
            sum2[i] += array[i] * array[i];
            if (max[i] < array[i]) {
                max[i] = array[i];
            }
            if (min[i] == 0 || min[i] > array[i]) {
                min[i] = array[i];
            }
        }
        count ++;
    }
    fclose(fp);

    for (int i = 0; i < 3; i ++) {
        mean[i] = sum[i] / count;
        stdev[i] = sqrt(sum2[i] / count - mean[i] * mean[i]);
    }

    printf("Age : (max,min,mean,stdev) = (%d, %d, %lf, %lf)\n", max[0], min[0], mean[0], stdev[0]);
    printf("Height : (max,min,mean,stdev) = (%d, %d, %lf, %lf)\n", max[1], min[1], mean[1], stdev[1]);
    printf("Weight : (max,min,mean,stdev) = (%d, %d, %lf, %lf)\n", max[2], min[2], mean[2], stdev[2]);

	return 0;
}
