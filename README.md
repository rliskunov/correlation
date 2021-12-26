# Serial option

## Explanation

Coefficient of correlation of two samples (data is set in a text file)

### Requirements

The app build and run requires:

- python3.6+ (numpy)
- g++ (std=c++11)
- Linux or macOS

### Tested Requirements versions

- python3.9 (numpy~=1.21.2)
- g++ c++23
- macOS Big Sur 11.2

### Installation

git clone https://github.com/rliskunov/correlation.git

### Create a sample

    $ make generator

### Calculate correlation (python)

    $ make correlation

### Calculate sequential correlation (c++)

    $ make sequential

### Calculate openmp correlation (c++)

    $ make openmp

### Calculate mpi correlation (c++)

    $ make mpi

### Usage

    $ make

### Usage on cluster

    $ sbatch -w c4 runner.sh 

### Example of working

    $ cat result.txt

