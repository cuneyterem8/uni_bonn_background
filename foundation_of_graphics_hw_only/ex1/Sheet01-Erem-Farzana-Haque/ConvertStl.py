# -*- coding: utf-8 -*-
"""
Created on Fri Oct 28 18:38:11 2016

@author: kleinj
"""

#this script requires numpy-stl to be installed!
import numpy as np
from stl import mesh

m = mesh.Mesh.from_file("Monkey.stl")
np.save('Monkey.npy', m.vectors)
print("done")
