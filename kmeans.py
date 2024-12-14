import sys

def read_args():
    iter = int(sys.argv[2]) if len(sys.argv) == 4 else 200

    if not 1 < iter < 1000:
        print("Invalid maximum iteration!")
        return 1

    K = int(sys.argv[1])
    
    with open(sys.argv[3]) as input_file:
        datapoints = [[float(coord) for coord in line.rstrip().split(",")] for line in input_file]
    N = len(datapoints)

    if not 1 < K < N:
        print("Invalid number of clusters!")
        return 1
    
    d = len(datapoints[0])
    
    return iter, K, datapoints, N, d

def main():
    iter, K, datapoints, N, d = read_args()
    
    print(f"iter = {iter}, K = {K}, d = {d}, N = {N}")
    # print(datapoints)

    print("Done")

if __name__ == "__main__":
    main()
