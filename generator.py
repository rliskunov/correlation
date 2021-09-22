import numpy as np
from random import randint

from numpy import array

size: int = randint(25, 100)
xarray: list = np.random.randint(25, 100, size=size)
yarray: list = np.random.randint(25, 100, size=size)

data: array = np.column_stack([xarray, yarray])
datafile_path: str = "sample.txt"
np.savetxt(datafile_path, data, fmt=['%d', '%d'])
