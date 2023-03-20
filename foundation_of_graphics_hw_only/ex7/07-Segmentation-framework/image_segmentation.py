# -*- coding: utf-8 -*-

# Cüneyt Erem, Md Emtazul Haque, Sheik Mastura Farzana

import numpy as np
import matplotlib.pyplot as plt
import skimage, skimage.io # These are part of the 'scikit-image' package

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



# load image
img = skimage.io.imread("lenna.png")
colors = np.reshape(img, (-1, 3)).astype(np.double)

# the number of clusters
# CAREFUL: you need at least two clusters, or the indexing will break!
clusterAmounts = [2, 5, 15]

f, axes = plt.subplots(len(clusterAmounts), 3, sharex=True, sharey=True)

def plot_image(i1, i2, data):
    data = data.reshape(img.shape[:2] + (-1,))
    if data.shape[2] == 1:
        data = data.reshape(img.shape[:2])
    axes[i1, i2].imshow(data.astype('uint8'))
    axes[i1, i2].set(xticks=[], yticks=[])

print("Starting computation...")

for i in range(len(clusterAmounts)):
    # TODO:
    # Use your k-means implementation to find 'k' cluster centers in
    # the colors 'colors'.  Assign the variable 'center_colors' with the
    # representative color of each center (dimension: k x 3).  The array
    # 'map_to_centers' should contain the center of each color in 'colors'.
	
    # map_to_centers, center_colors = ...
    map_to_centers, center_colors = my_kmeans(colors,clusterAmounts[i])

    # display image
    plot_image(i, 0, colors) # original image
    axes[i, 0].set_title("Original")
    
    plot_image(i, 1, map_to_centers) # clusters
    axes[i, 1].set_title("Clusters k={}".format(clusterAmounts[i]))
    
    # TODO:
    # Plot a reconstruction of the image, using only the cluster centers as colors
    # plot_image(...)

    cluster = np.copy(colors)

    for j in range(clusterAmounts[i]):
        cluster[map_to_centers == j] = center_colors[j]
        
    plot_image(i, 2, cluster)

    axes[i, 2].set_title("Reconstruction k={}".format(clusterAmounts[i]))

plt.tight_layout(pad=0.1, w_pad=0.1)
print("done")
plt.show()