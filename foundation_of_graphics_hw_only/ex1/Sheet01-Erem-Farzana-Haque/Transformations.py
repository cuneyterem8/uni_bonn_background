# -*- coding: utf-8 -*-
"""
@author: 
Cüneyt Erem, 3277992
Sheikh Mastura Farzana, 3276883
Md Emtazul Haque, 3315998
"""
import math
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

def getRotationMatrixZaxis(angle):
    return np.array([
        [np.cos(angle), -1*np.sin(angle), 0],
        [np.sin(angle), np.cos(angle), 0],
        [0,0,1]
    ])

v = np.load('Monkey.npy')

figure=plt.figure()
ax = mplot3d.Axes3D(figure)
ax.add_collection3d(mplot3d.art3d.Poly3DCollection(v))

# a) limit the ranges of all three axes to [-2,...2]
# Assigning labels to axis
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Setting limit to all axes
axisLimit = (-2,2)
ax.set_xlim(axisLimit)
ax.set_ylim(axisLimit)
ax.set_zlim(axisLimit)

plt.show()

# b) translate and scale the model
# scaling according to scaleRatio
scaleRatio = 2
scaledImage = np.copy(v)*scaleRatio

# Translating
translationMatrix = np.array([1,0,0])
for i in range(0, scaledImage.shape[0]):
    for j in range(0, scaledImage[i].shape[0]):
        scaledImage[i][j] += translationMatrix

# show results in a new plot
scaleFigure=plt.figure()
scaleax = mplot3d.Axes3D(scaleFigure)
scaleax.add_collection3d(mplot3d.art3d.Poly3DCollection(scaledImage))
scaleax.set_xlabel('X')
scaleax.set_ylabel('Y')
scaleax.set_zlabel('Z')
scaleax.set_xlim(axisLimit)
scaleax.set_ylim(axisLimit)
scaleax.set_zlim(axisLimit)

plt.show()

# c) rotate model around z-axis and display both at the same time
# rotating 1(a)
image = np.copy(v)
rotationMatrixZaxisPIby2 = getRotationMatrixZaxis(np.pi/2)
for i in range(0, image.shape[0]):
    image[i] = np.dot(image[i], rotationMatrixZaxisPIby2)

# rotating 1(b)
rotationMatrixZaxisPIby4 = getRotationMatrixZaxis(np.pi/4)
for i in range(0, scaledImage.shape[0]):
  scaledImage[i] = np.dot(scaledImage[i], rotationMatrixZaxisPIby4)

# Translating 1(b)
translationMatrix = np.array([1,0,0])
for i in range(0, scaledImage.shape[0]):
    for j in range(0, scaledImage[i].shape[0]):
        scaledImage[i][j] += translationMatrix

# plotting both
rotateFigure = plt.figure()
rotateax = mplot3d.Axes3D(rotateFigure)
rotateax.set_xlabel('X')
rotateax.set_ylabel('Y')
rotateax.set_zlabel('Z')
rotateax.set_xlim(axisLimit)
rotateax.set_ylim(axisLimit)
rotateax.set_zlim(axisLimit)
rotateax.add_collection3d(mplot3d.art3d.Poly3DCollection(image))
rotateax.add_collection3d(mplot3d.art3d.Poly3DCollection(scaledImage))

plt.show()


