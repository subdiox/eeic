#include <stdio.h>

int comp_int(const void *x0, const void *x1)
{
    const int y0 = *(int*)x0;
    const int y1 = *(int*)x1;

    if (y0 > y1) return -1;
    if (y0 < y1) return 1;

    return 0;
}
