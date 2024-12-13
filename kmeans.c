#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

struct centroid {
    double *centroid_coords;
    double sum;
    int count;
};

struct datapoint {
    double *coords;
    struct datapoint *next;
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

int init_centroids(int K, struct datapoint *points, struct centroid **centroids) {
    int i = 0;

    /* memory initialized as zeroes. */
    struct centroid *cent = calloc(K, sizeof(struct centroid));
    if (cent == NULL) {
        return 1;
    }

    /* set first K centroids to first K datapoints. */
    for (; i < K; i++) {
        (cent + i)->centroid_coords = points->coords;
        points = points->next;
    }

    *centroids = cent;
    return 0;
}

int run_kmeans(int K, int iter, int d, struct datapoint *points, struct centroid *centroids) {
    int is_not_converged = 1;
    int i = 0;
    int j = 0;
    struct datapoint *point = NULL;
    struct centroid *cent = NULL;

    for (; i < iter && is_not_converged; i++) {
        point = points;
        do {
            /*
            Go over all centroids - find closest (euclidean distance function), add to sum and counter of centroid.
            */
            point = points->next;
        } while (point != NULL);
        
        cent = centroids;
        for (j = 0; j < K; j++, cent++) {
            /*
            * Go over all centroids:
            * update their value to their sum divided by their counter, and set sum and counter as 0.
            * If their delta is greater than eps, set “all less than eps” to false
            */
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;
    int d = 0;
    int N = 0;
    struct datapoint *datapoints = NULL;
    struct centroid *centroids = NULL;

    if (0 != read_args(argc, argv, &K, &iter, &d, &N, &datapoints)) {
        /* error */
        printf("Error reading args\n");
        return 1;
    }

    if (0 != init_centroids(K, datapoints, &centroids)) {
        /* error */
        printf("Error initializing centroids\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (0 != run_kmeans(K, iter, d, datapoints, centroids)) {
        printf("Error in kmeans algorithm\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    /* TODO: print result */

    printf("Done\n");
    return 0;
}
