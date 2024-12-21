import sys
import numpy as np

def read_args():
    iter = int(sys.argv[2]) if len(sys.argv) == 4 else 200

    if not 1 < iter < 1000:
        print("Invalid maximum iteration!")
        return 0, 0, [], 0, 0, False

    K = int(sys.argv[1])
    
    with open(sys.argv[-1]) as input_file:
        datapoints = [np.array([float(coord) for coord in line.rstrip().split(",")]) for line in input_file]
    N = len(datapoints)

    if not 1 < K < N:
        print("Invalid number of clusters!")
        return 0, 0, [], 0, 0, False
    
    d = len(datapoints[0])
    
    return iter, K, datapoints, N, d, True

def euclidean_distance(point1, point2):
    return (sum([(point1[i] - point2[i])**2 for i in range(len(point1))]))**0.5

def init_centroids(K, datapoints):
    return datapoints[:K]

def run_kmeans(K, datapoints, centroids, d, iter):
    eps = 0.001

    for i in range(iter):
        old_centroids = centroids
        centroids_sums = [np.zeros(d) for j in range(K)]
        centroids_counters = [0 for i in range(K)]
        
        for point in datapoints:
            min_dist_centroid = 0
            min_dist = euclidean_distance(point, centroids[0])

            for ind, centroid in enumerate(centroids):
                dist = euclidean_distance(point, centroid)

                # if the point is closest to this centroid so far:
                if dist < min_dist:
                    # update the info of the closest centroid
                    min_dist = dist
                    min_dist_centroid = ind
            
            # add the point to the closest centroid's sum and up its counter by 1
            centroids_sums[min_dist_centroid] += point
            centroids_counters[min_dist_centroid] += 1
        
        # set the new centroids to the mean of all of the points closest to them
        centroids = [centroids_sums[i] / centroids_counters[i] if centroids_counters[i] != 0 else old_centroids[i] \
                     for i in range(K)]

        # if the centroids barely moved since the last iteration, stop
        if max([euclidean_distance(centroids[i], old_centroids[i]) for i in range(K)]) < eps:
            break
    
    return centroids

def main():
    iter, K, datapoints, N, d, success = read_args()
    if not success:
        return
    
    centroids = run_kmeans(K, datapoints, init_centroids(K, datapoints), d, iter)

    for centroid in centroids:
        print(",".join(["%.4f" % coord for coord in centroid]))

if __name__ == "__main__":
    main()
