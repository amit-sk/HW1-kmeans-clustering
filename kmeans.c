#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

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
    free(first_coord);

    /* validate K from above */
    if (*K >= *N) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    /* printing the info extracted, can delete later */
    printf("K = %d, N = %d, d = %d, iter = %d\n", *K, *N, *d, *iter);

    return 0;
}

void free_datapoints_memory(struct datapoint **datapoints) {
    struct datapoint *curr_datapoint, *next_datapoint;
    struct coord *curr_coord, *next_coord;
    
    curr_datapoint = *datapoints;
    do {
        curr_coord = curr_datapoint->coords;
        do {
            next_coord = curr_coord->next;
            free(curr_coord);
            curr_coord = next_coord;
        }
        while (next_coord->next != NULL);
        free(next_coord);
        
        next_datapoint = curr_datapoint->next;
        free(curr_datapoint);
        curr_datapoint = next_datapoint;
    }
    while (next_datapoint->next != NULL);
    free(next_datapoint);
}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;
    int d = 0;
    int N = 0;
    struct datapoint *datapoints = NULL;
    /* stuff for printing the data, can delete later */
    struct datapoint *curr_datapoint;
    struct coord *curr_coord;


    if (0 != read_args(argc, argv, &K, &iter, &d, &N, &datapoints)) {
        /* error */
        return 1;
    }

    /* printing the data, can delete later */
    curr_datapoint = datapoints;
    do {
        curr_coord = curr_datapoint->coords;
        do {
            printf("%f,", curr_coord->coord);
            curr_coord = curr_coord->next;
        }
        while (curr_coord->next != NULL);
        printf("\n");
        curr_datapoint = curr_datapoint->next;
    }
    while (curr_datapoint->next != NULL);

    free_datapoints_memory(&datapoints);

    printf("Done\n");
    return 0;
}
