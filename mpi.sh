#!/bin/bash

#SBATCH -N 4
#SBATCH -n 32
#SBATCH -t 2-12:00:00
#SBATCH -o mpi.%N.%j.out
#SBATCH -e mpi.%N.%j.err
prun ./mpi.exe
