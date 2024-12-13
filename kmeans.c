#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

struct centroid {
    struct coord *centroid_coords;
    double sum;
    int count;
};

struct coord
{
    double coord;
    struct coord *next;
};

struct datapoint
{
    struct coord *coords;
    struct datapoint *next;
};

/* internal funcs for read_args */
void init_coords(struct coord **first_coord, struct coord **curr_coord) {
    *first_coord = malloc(sizeof(struct coord));
    *curr_coord = *first_coord;
    (*curr_coord)->next = NULL;
}

void init_datapoints(struct datapoint **first_datapoint, struct datapoint **curr_datapoint) {
    *first_datapoint = malloc(sizeof(struct datapoint));
    *curr_datapoint = *first_datapoint;
    (*curr_datapoint)->next = NULL;
}

void progress_coord(struct coord **curr_coord, double n) {
    (*curr_coord)->coord = n;
    (*curr_coord)->next = malloc(sizeof(struct coord));
    (*curr_coord) = (*curr_coord)->next;
    (*curr_coord)->next = NULL;
}

void progress_datapoint(struct datapoint **curr_datapoint, struct coord **first_coord) {
    (*curr_datapoint)->coords = *first_coord;
    (*curr_datapoint)->next = malloc(sizeof(struct datapoint));
    (*curr_datapoint) = (*curr_datapoint)->next;
    (*curr_datapoint)->next = NULL;
}

/* parse the args (K, iter and the datapoints) */
int read_args(int argc, char *argv[], int *K, int *iter, int *d, int *N, struct datapoint **datapoints) {
    /* init vars for reading the file */
    double n; /* for the double values */
    char delim; /* commas/\n/... */
    struct datapoint *curr_datapoint;
    struct coord *first_coord, *curr_coord;

    /* Read arguments - argc should be 2 if there is not iter arg, 3 if there is */
    if (argc == 3) {
        sscanf(argv[2], "%d", iter);
    }
    sscanf(argv[1], "%d", K);

    /* validate K from below & iter */
    if (*K <= 1) {
        printf("Invalid number of clusters!\n");
        return 1;
    }
    if (*iter <= 1 || *iter >= 1000) {
        printf("Invalid maximum iteration!\n");
        return 1;
    }

    /* init the first datapoint and its first coordinate */
    init_datapoints(datapoints, &curr_datapoint);
    init_coords(&first_coord, &curr_coord);

    /* go over first line to get d */
    do {
        scanf("%lf%c", &n, &delim);
        progress_coord(&curr_coord, n);
        (*d)++;
    }
    while (delim != '\n');
    progress_datapoint(&curr_datapoint, &first_coord);
    init_coords(&first_coord, &curr_coord);
    (*N)++;

    /* go over the rest of the lines to get datapoints and N */
    while (scanf("%lf%c", &n, &delim) == 2) {
        progress_coord(&curr_coord, n);
        if (delim == '\n') { /* if at the end of the line */
            progress_datapoint(&curr_datapoint, &first_coord);
            init_coords(&first_coord, &curr_coord);
            (*N)++;
        }
    }

    /* validate K from above */
    if (*K >= *N) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    /* TODO: printing the info extracted, can delete later */
    printf("K = %d, N = %d, d = %d, iter = %d\n", *K, *N, *d, *iter);

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
        /* TODO: we probably want to create copies of the coords so we can free them later */
        (cent + i)->centroid_coords = points->coords;
        points = points->next;
    }

    *centroids = cent;
    return 0;
}

int run_kmeans(int K, int iter, int d, struct datapoint *points, struct centroid *centroids) {
    int is_not_converged = 1;
    int j = 0;
    struct datapoint *point = NULL;
    struct centroid *cent = NULL;

    int i = 0;
    for (; i < iter && is_not_converged; i++) {
        point = points;
        do {
            /*
            TODO: Go over all centroids - find closest (euclidean distance function), add to sum and counter of centroid.
            */
            point = point->next;
        } while (point != NULL);
        
        cent = centroids;
        for (j = 0; j < K; j++, cent++) {
            /*
            * TODO: Go over all centroids:
            * update their value to their sum divided by their counter, and set sum and counter as 0.
            * If their delta is greater than eps, set “all less than eps” to false
            */
        }
    }
    return 0;
}

float calc_euclidean_distance(float *point1, float *point2, int *d){
    float sum = 0;
    for (int i = 0; i<*d; i++){
        sum += powf((point1[i] - point2[i]), 2);
    }
    return sqrt(sum);

}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;
    int d = 0;
    int N = 0;
    struct datapoint *datapoints = NULL;
    struct centroid *centroids = NULL;

    /* TODO: stuff for printing the data, can delete later */
    struct datapoint *curr_datapoint;
    struct coord *curr_coord;

    if (0 != read_args(argc, argv, &K, &iter, &d, &N, &datapoints)) {
        return 1;
    }

    /* TODO: printing the data, can delete later */
    curr_datapoint = datapoints;
    do {
        curr_coord = curr_datapoint->coords;
        do {
            printf("%f,", curr_coord->coord);
            curr_coord = curr_coord->next;
        } while (curr_coord->next != NULL);
        printf("\n");
        curr_datapoint = curr_datapoint->next;
    } while (curr_datapoint->next != NULL);

    if (0 != init_centroids(K, datapoints, &centroids)) {
        /* TODO: later delete indicative error */
        printf("Error initializing centroids\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (0 != run_kmeans(K, iter, d, datapoints, centroids)) {
        /* TODO: later delete indicative error */
        printf("Error in kmeans algorithm\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    /* TODO: print result */

    /* TODO: free memory */
    free(centroids);

    printf("Done\n");
    return 0;
}
