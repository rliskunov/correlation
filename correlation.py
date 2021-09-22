import numpy as np


def get_elements(path):
    a: list = []
    b: list = []
    with open(path) as file:
        while line := file.readline().rstrip():
            delimiter = line.split()
            a.append(int(delimiter[0]))
            b.append(int(delimiter[1]))
    return a, b


if __name__ == '__main__':
    filename: str = "sample.txt"
    xarray, yarray = get_elements(path=filename)
    correlation: float = round(np.corrcoef(xarray, yarray)[0][1], 7)
    print(f"Correlation coefficient: {correlation}")
