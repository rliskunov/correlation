from random import randint
from typing import Union

import numpy as np
from numpy import array

if __name__ == '__main__':
    min_number: int = 3
    max_number: int = 12
    coefficient: int = 1_000_000
    size: int = randint(
        min_number * coefficient,
        max_number * coefficient
    )

    coefficient: int = 1_000
    xarray: Union = np.random.randint(
        min_number * coefficient,
        max_number * coefficient,
        size=size
    )
    yarray: Union = np.random.randint(
        min_number * coefficient,
        max_number * coefficient,
        size=size
    )

    data: array = np.column_stack([xarray, yarray])
    datafile_path: str = "sample.txt"
    np.savetxt(datafile_path, data, fmt=['%d', '%d'])
