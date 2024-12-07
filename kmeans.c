#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

int read_args(int argc, char *argv[], int *K, int *iter) {
    /* Read arguments - argc should be 2 if there is not iter arg, 3 if there is */
    return 0;
}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;

    if (0 != read_args(argc, argv, &K, &iter)) {
        /* error */
        return 1;
    }

    printf("Done\n");
    return 0;
}
