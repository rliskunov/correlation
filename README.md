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

### Parallel calculate correlation (c++)

    $ make parallel

### Non-parallel Calculate correlation (c++)

    $ make non_parallel

### Usage

    $ make

### Example of working

    $ cat result.txt

