//
// Created by Roman Liskunov on 06.11.2021.
//


#include <mpi.h>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv) {
    int rank, world;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    printf("Hello: rank %d, world: %d\n",rank, world);
    MPI_Finalize();
}