
import numpy as np
import scipy, scipy.spatial

# Cüneyt Erem, Md Emtazul Haque, Sheik Mastura Farzana

def my_kmeans(data, k):
    # TODO:
    # The variable 'data' contains data points in its rows.
    # Initilize 'k' centers randomized.  Afterwards apply Floyd's algorithm
    # until convergence to get a solution of the k-means problem.  Utilize
    # 'scipy.spatial.cKDTree' for nearest neighbor computations.

    random_choice = np.random.choice(range(data.shape[0]), size=k)
    centers = data[random_choice]

    prev_label = None
    while True:
        _, curr_label = scipy.spatial.cKDTree(centers).query(data)
        if np.all(prev_label == curr_label):
            break
        else:
            prev_label = curr_label

        for i in range(k):
            center_i = data[i == prev_label]
            centers[i] = np.mean(center_i, axis=0)
    index = prev_label

    return index, centers
