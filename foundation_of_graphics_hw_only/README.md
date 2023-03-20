# Foundation of Graphics


Most of the exercises include heavily practical coding and some theoretical math


## Content

Affine and projective transformations with applications to image formation (rigid body motion, cinematic chains); Parametric curves and surfaces with applications to 3D modelling; Ordinary differential equations with applications to physical based modelling


### Python

```python
import matplotlib.pyplot as plt
import matplotlib.image
import scipy.signal
import numpy as np

from mpl_toolkits.axes_grid1 import make_axes_locatable
```
```python
def MyConvolve2d(f, g):
	c = np.array([[0]])
	
	padding = 1
	kernel_flip = np.flipud(g)
	image_padded = np.pad(f, ((g.shape[0])), 'wrap')
	defzero = np.zeros((f.shape[0] + g.shape[0], f.shape[1] + g.shape[1]))

	for i in range(f.shape[0] + g.shape[0]):
		for j in range(f.shape[1] + g.shape[1]):
			try:
				mult = kernel_flip * image_padded[i:i + g.shape[0], j:j + g.shape[1]]
				defzero[i ,j] = mult.sum()
			except:
				break
	c = defzero[:-padding, :-padding]
	return c
```
```python
def MyFftConvolve2d(f, g):
	c = np.array([[0]])

	padding = 1
	image_padded = np.pad(f, ((g.shape[0])), 'wrap')
	F_f = np.fft.fft2(image_padded)
	F_g = np.fft.fft2(g, s=F_f.shape)
	F_h = F_f * F_g

	c = np.fft.ifft2(F_h)
	c = c[g.shape[0]:c.shape[0]-padding,g.shape[1]:c.shape[1]-padding]
	c = c.real

	return c
```


### Theory

<img src="https://github.com/cuneyterem8/uni_bonn_background/blob/main/foundation_of_graphics_hw_only/image.png?raw=true" width="60%" height="60%">

> Example from exercise
