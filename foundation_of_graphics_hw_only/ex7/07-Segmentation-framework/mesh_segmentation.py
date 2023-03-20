
# Cüneyt Erem, Md Emtazul Haque, Sheik Mastura Farzana

import numpy as np
import scipy, scipy.spatial, scipy.io
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

from my_kmeans import my_kmeans

# Check for the right python version
import sys
if sys.version_info.major != 3:
    print("Wrong Python version\n")
    print("You are using Python ",
          sys.version_info.major, ".", sys.version_info.minor,
          ". To ensure portability, please use Python 3 for this exercise",
          sep='')
    sys.exit()
    
    

# load data:
#   points:     (Nx3) vector of vertex coordinates
#   normals:     (Nx3) vector of vertex normals
data = scipy.io.loadmat("data.mat")
points, normals = data['points'], data['normals']

# display the vertices & normals
ax = plt.figure().gca(projection='3d')
ax.quiver(points[:, 0], points[:, 1], points[:, 2], normals[:, 0], normals[:, 1], normals[:, 2], length=0.01)
ax.set_title("Data")

# select the number of clusters
k = 5


print("Starting computation...")
# TODO:
# Use your k-means implementation to find 'k' cluster centers in
# the normal vectors.  The array 'map_to_centers' should contain the index of the
# center of each normal in 'VN'.
#
map_to_centers, center_colors = my_kmeans(normals,k)

# Cluster Visualization
colors=["r", "g", "b", "c", "m", "y", "k"]
ax = plt.figure().gca(projection='3d')
ax.set_title("Clustering")
# TODO:
# create a plot as above, where each cluster has a different color
ax.quiver(points[:, 0], points[:, 1], points[:, 2], normals[:, 0], normals[:, 1], normals[:, 2], length=0.01, colors=colors)

print("done")
plt.show()
