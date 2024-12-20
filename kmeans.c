#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const double EPSILON = 0.001;
const int DEFAULT_ITER = 200;

struct centroid {
    struct coord *centroid_coords;
    struct coord *sum;
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

int init_coord(struct coord **coord, double n) {
    struct coord *new_coord = malloc(sizeof(struct coord));
    if (new_coord == NULL) {
        return 1;
    }
    new_coord->coord = n;
    new_coord->next = NULL;

    *coord = new_coord;
    return 0;
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
    struct coord *first_coord;
    struct coord ** curr_coord;

    if (argc < 2 || argc > 3) {
        printf("An Error Has Occurred\n");
        return 1;
    }

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

    /* go over first line to get d */
    first_coord = NULL;
    curr_coord = &first_coord;
    do {
        scanf("%lf%c", &n, &delim);
        if (0 != init_coord(curr_coord, n)) {
            return 1;
        }
        curr_coord = &(*curr_coord)->next;
        (*d)++;
    } while (delim != '\n');

    progress_datapoint(&curr_datapoint, &first_coord);
    (*N)++;

    /* go over the rest of the lines to get datapoints and N */
    first_coord = NULL;
    curr_coord = &first_coord;
    while (scanf("%lf%c", &n, &delim) == 2) {
        if (0 != init_coord(curr_coord, n)) {
            return 1;
        }
        curr_coord = &(*curr_coord)->next;

        if (delim == '\n') { /* if at the end of the line */
            progress_datapoint(&curr_datapoint, &first_coord);
            first_coord = NULL;
            curr_coord = &first_coord;
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


int init_centroids(int d, int K, struct datapoint *points, struct centroid **centroids) {
    int i = 0;
    int j = 0;
    struct datapoint *curr_datapoint = NULL;
    struct coord **curr_coord = NULL;
    struct coord *point_coord = NULL;

    /* memory initialized as zeroes. */
    struct centroid *cent = calloc(K, sizeof(struct centroid));
    if (cent == NULL) {
        return 1;
    }

    /* set first K centroids to first K datapoints. */
    for (curr_datapoint = points; i < K; i++, curr_datapoint = curr_datapoint->next) {
        /* set centroid coords to datapoint coordinates */
        curr_coord = &(cent + i)->centroid_coords;
        for (point_coord = curr_datapoint->coords;
             point_coord != NULL;
             point_coord = point_coord->next, curr_coord = &(*curr_coord)->next) {
            if (0 != init_coord(curr_coord, point_coord->coord)) {
                return 1;
            }
        }
        
        /* set sums to zeroes on all dimensions */
        curr_coord = &(cent + i)->sum;
        for (j = 0; j < d; j++, curr_coord = &(*curr_coord)->next) {
            if (0 != init_coord(curr_coord, 0)) {
                return 1;
            }
        }
    }

    *centroids = cent;
    return 0;
}

int add_coord_to_centroid(struct centroid *cent, struct datapoint *point, int d){
    int i;
    struct coord *curr_sum_coord = cent->sum;
    struct coord *curr_datapoint_coord = point->coords;

    for (i = 0; i < d; i++){
        curr_sum_coord->coord += curr_datapoint_coord->coord;
        curr_sum_coord = curr_sum_coord->next;
        curr_datapoint_coord = curr_datapoint_coord->next;
    }
    return 0;
}

double calc_euclidean_distance(struct centroid *cent, struct datapoint *point2, int d){
    double sum = 0;
    struct coord *coord1 = cent->centroid_coords;
    struct coord *coord2 = point2->coords;
    int i;

    for (i = 0; i < d; i++){
        sum += pow((coord1->coord - coord2->coord), 2);
        coord1 = coord1->next;
        coord2 = coord2->next;
    }
    return sqrt(sum);
}


int run_kmeans(int d, int K, int iter, struct datapoint *points, struct centroid *centroids) {
    int is_not_converged = 1;
    int j = 0;
    int k = 0;
    struct datapoint *point = NULL;
    struct centroid *cent = NULL;
    int index;
    int i = 0;
    
    for (i = 0; i < iter && is_not_converged; i++) {
        /* for each point, find closest centroid */
        for (point = points; point != NULL; point = point->next) {
            struct centroid *min_cent = centroids;
            double min_distance = HUGE_VAL;
            double curr_distance = 0;

            /*
            TODO: Go over all centroids - find closest (euclidean distance function), add to sum and counter of centroid.
            */
            for (index = 0; index < K; index++){
                curr_distance = calc_euclidean_distance(centroids + index, point, d);
                if (curr_distance < min_distance){
                    min_distance = curr_distance;
                    min_cent = centroids + index;
                }
            }
            min_cent->count++;
            add_coord_to_centroid(min_cent, point, d);
        }
 
        cent = centroids;
        for (j = 0; j < K; j++, cent++) {
            struct coord *curr_coord = cent->centroid_coords;
            struct coord *sum_coord = cent->sum;
            int curr_sum = 0;

            is_not_converged = 0;
            for(k = 0; k < d; k++){
                if ((cent + j)->count != 0){
                    if (fabs(curr_coord->coord-sum_coord->coord/(cent + j)->count) > EPSILON){
                        is_not_converged = 1;
                    }
                    curr_coord->coord = sum_coord->coord/(cent + j)->count;
                    sum_coord->coord = 0;
                    curr_coord = curr_coord->next;
                    sum_coord = sum_coord->next;
                }
            }
            cent->count = 0;

            /*
            * TODO: Go over all centroids:
            * update their value to their sum divided by their counter, and set sum and counter as 0.
            * If their delta is greater than eps, set “all less than eps” to false
            */
        }
    }
    return 0;
}

void free_all(int K, struct datapoint *datapoints, struct centroid *centroids) {
    int i;
    struct centroid *centroid;
    struct coord *curr_coord;

    centroid = centroids;
    for (i = 0; i < K; i++, centroid++) {
        /* TODO free centroid coords */
    }
    free(centroids);
    
}

int main(int argc, char *argv[]) {
    int K = 0;
    int iter = DEFAULT_ITER;
    int d = 0;
    int N = 0;
    struct datapoint *datapoints = NULL;
    struct centroid *centroids = NULL;
    int i;

    /* TODO: stuff for printing the data, can delete later */
    struct datapoint *curr_datapoint;
    struct coord *curr_coord;

    if (0 != read_args(argc, argv, &K, &iter, &d, &N, &datapoints)) {
        return 1;
    }

    /* TODO: printing the data, can delete later */
    for (curr_datapoint = datapoints; curr_datapoint != NULL; curr_datapoint = curr_datapoint->next) {
        for (curr_coord = curr_datapoint->coords; curr_coord != NULL; curr_coord = curr_coord->next) {
            printf("%f,", curr_coord->coord);
        }
        printf("\n");
    }

    if (0 != init_centroids(d, K, datapoints, &centroids)) {
        /* TODO: later delete indicative error */
        printf("Error initializing centroids\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    if (0 != run_kmeans(d, K, iter, datapoints, centroids)) {
        /* TODO: later delete indicative error */
        printf("Error in kmeans algorithm\n");
        printf("An Error Has Occurred\n");
        return 1;
    }

    /* TODO: print result */
    printf("\n");
    for (i = 0;i<K;i++){
        struct coord *curr_coord = (centroids + i)->centroid_coords;
        do {
            printf("%f,", curr_coord->coord);
            curr_coord = curr_coord->next;
        } while (curr_coord->next != NULL);
        printf("\n");
    }

    /* TODO: free memory */
    free_all(K, datapoints, centroids);

    printf("Done\n");
    return 0;
}
