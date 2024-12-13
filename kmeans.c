#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

struct datapoint
{
    double *coords;
    double *next;
};

int read_args(int argc, char *argv[], int *K, int *iter, int *d, int *N, struct datapoint **datapoints) {
    /* Read arguments - argc should be 2 if there is not iter arg, 3 if there is */
    if (argc == 3) {
        sscanf(*argv[2], "%lf", *iter);
    }
    sscanf(*argv[1], "%lf", *K);

    if (*K <= 1) {
        printf("Invalid number of clusters!\n");
    }
    if (*iter <= 1) {
        printf("Invalid maximum iteration!\n");
    }

    /* go over first line to get d */

    
    return 0;
}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;
    int d = 0;
    int N = 0;
    struct datapoint *datapoints = NULL;


    if (0 != read_args(argc, argv, &K, &iter, &d, &N, &datapoints)) {
        /* error */
        return 1;
    }

    printf("Done\n");
    return 0;
}
