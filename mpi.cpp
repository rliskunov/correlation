#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

using namespace std;

int comm_sz;
int rank;
clock_t begin_init, begin_calc, end_calc;
double time_serial_from_init, time_serial_from_calc, time_parallel_from_init, time_parallel_from_calc;

int get_number_elements(const char *path);

void parallelPCC(const int numberArray, double *arrX, double *arrY);

int main(void) {
    auto path = "sample.txt";

    const auto numberArray = get_number_elements(path);
    auto *arrX = new double[numberArray];
    auto *arrY = new double[numberArray];

    ifstream file(path);
    if (!file.is_open()) {
        cout << "Error reading!" << endl;
    } else {
        string line;
        int count = 0;
        while (getline(file, line)) {
            double x, y;
            istringstream iss(line);
            iss >> x >> y;
            arrX[count] = x;
            arrY[count] = y;
            count++;
        }
    }
    file.close();

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (comm_sz > 1) {
        parallelPCC(numberArray, arrX, arrY);
    } else {
        cout << "\nERROR" << endl;
    }

    MPI_Finalize();
    return 0;
}

void parallelPCC(const int numberArray, double *arrX, double *arrY) {
    if (rank == 0) {
        begin_init = clock();
    }

    double *a = arrX;
    double *b = arrY;
    double tempSumA = 0;
    double tempSumB = 0;
    double totalA = 0;
    double totalB = 0;
    double meanA = 0;
    double meanB = 0;
    double lastB = 0;

    int localSize = 0;
    int remainder = numberArray % comm_sz;
    if (remainder == 0) {
        localSize = numberArray / comm_sz;
    } else {
        localSize = numberArray / comm_sz;
        if (rank < remainder) {
            localSize++;
        }
    }

    double *local_a = arrX;
    double *local_b = arrY;

    double offset = 0;
    if (rank < remainder) {
        offset = rank * localSize;
    } else {
        offset = (rank * localSize) + remainder;
    }

    if (rank == comm_sz - 1) {
        lastB = local_b[localSize - 1];
        MPI_Send(&lastB, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Reduce(&tempSumA, &totalA, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    begin_calc = clock();

    if (rank == 0) {
        MPI_Recv(&lastB, 1, MPI_DOUBLE, comm_sz - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        meanA = totalA / numberArray;
        totalB = totalA + lastB;
        meanB = totalB / numberArray;
    }

    MPI_Bcast(&meanA, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&meanB, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double tempA = 0;
    double tempB = 0;
    double tempSum = 0;
    double totalA2 = 0;
    double totalB2 = 0;
    double totalTempSum = 0;

    for (int j = 0; j < localSize; j++) {

        tempA += (local_a[j] - meanA) * (local_a[j] - meanA);
        tempB += (local_b[j] - meanB) * (local_b[j] - meanB);

        tempSum += (local_a[j] - meanA) * (local_b[j] - meanB);
    }

    MPI_Reduce(&tempA, &totalA2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tempB, &totalB2, 1, MPI_DOUBLE, MPI_SUM, 1, MPI_COMM_WORLD);
    MPI_Reduce(&tempSum, &totalTempSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    free(local_a);
    free(local_b);
    free(a);
    free(b);

    double sdB = 0;
    if (rank == 1) {
        sdB = sqrt(totalB2 / numberArray);
        MPI_Send(&sdB, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        double sdA = sqrt(totalA2 / numberArray);

        MPI_Recv(&sdB, 1, MPI_DOUBLE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        totalTempSum = totalTempSum / numberArray;
        double pcc = totalTempSum / (sdA * sdB);
        cout << "\nPearson Correlation Coefficient:" << pcc << endl;

        end_calc = clock();
        time_parallel_from_calc = ((double) (end_calc - begin_calc) / CLOCKS_PER_SEC) * 1000;
        cout << "\nTime Taken (Calculation Only) " << time_parallel_from_calc << endl;
        time_parallel_from_init = ((double) (end_calc - begin_init) / CLOCKS_PER_SEC) * 1000;
        cout << "\nTime Taken (Including Array Initialization):" << time_parallel_from_init << endl;
    }
}

int get_number_elements(const char *path) {
    ifstream in(path);
    string str;
    auto temp = 0;
    if (in.is_open()) {
        while (getline(in, str)) {
            temp++;
        }
    } else cout << "Error reading" << endl;
    in.close();
    return temp;
}