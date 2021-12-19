#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

int getNumberElements(const char *path);

void getMask(int numberToMask, int numberArray, int *powerOfTwo);

void calculate(int start_row, int end_row, const int numberArray, int *arrX, int *arrY);

using namespace std;

int main(int argc, char **argv) {
    auto path = "sample.txt";

    int numberArray;

    int *arrX;
    int *arrY;

    int rank, ierr, numprocs, an_id, avg_rows_per_process, start_row, end_row;

    int root_process = 0;
    MPI_Comm Comm = MPI_COMM_WORLD;
    MPI_Status status;
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(Comm, &rank);
    ierr = MPI_Comm_size(Comm, &numprocs);

    MPI_Barrier(Comm);
    if (rank == root_process) {
        numberArray = getNumberElements(path);
        int variants = pow(2, numberArray);
        arrX = new int[numberArray];
        arrY = new int[numberArray];

        ifstream file(path);
        if (file.is_open()) {
            string line;
            int count = 0;
            while (getline(file, line)) {
                int x, y;
                istringstream iss(line);
                iss >> x >> y;
                arrX[count] = x;
                arrY[count] = y;
                count++;
            }
        } else cout << "Error reading!" << endl;
        file.close();

        avg_rows_per_process = numberArray / numprocs;

        MPI_Bcast(&avg_rows_per_process, 1, MPI_INT, 0, Comm);
        MPI_Bcast(&numberArray, 1, MPI_INT, 0, Comm);
        MPI_Bcast(arrX, numberArray, MPI_INT, 0, Comm);
        MPI_Bcast(arrY, numberArray, MPI_INT, 0, Comm);

        calculate(
                1,
                avg_rows_per_process,
                numberArray,
                arrX,
                arrY
        );

        delete[] arrX;
        delete[] arrY;
    } else {
        MPI_Bcast(&avg_rows_per_process, 1, MPI_INT, 0, Comm);
        MPI_Bcast(&numberArray, 1, MPI_INT, 0, Comm);

        arrX = new int[numberArray];
        arrY = new int[numberArray];

        MPI_Bcast(arrX, numberArray, MPI_INT, 0, Comm);
        MPI_Bcast(arrY, numberArray, MPI_INT, 0, Comm);

        start_row = avg_rows_per_process * rank;
        end_row = avg_rows_per_process * (rank + 1);

        calculate(
                start_row,
                end_row,
                numberArray,
                arrX,
                arrY
        );
    }

    ierr = MPI_Finalize();
    return 0;
}

void calculate(int start_row, int end_row, const int numberArray, int *arrX, int *arrY) {
    double x_amount = 0, y_amount = 0, xy_amount = 0;
    double x_square_amount = 0, y_square_amount = 0;
    for (int i = start_row; i < end_row + 1; i++) {
        // sum of elements of array arrX.
        x_amount += arrX[i];

        // sum of elements of array arrY.
        y_amount += arrY[i];

        // sum of arrX[i] * arrY[i].
        xy_amount += arrX[i] * arrY[i];

        // sum of square of array elements.
        x_square_amount += arrX[i] * arrX[i];
        y_square_amount += arrY[i] * arrY[i];
    }
    double result = (numberArray * xy_amount - x_amount * y_amount)
                    / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                           * (numberArray * y_square_amount - y_amount * y_amount));
    cout << "Correlation coefficient: " << result << endl;
}


int getNumberElements(const char *path) {
    ifstream in(path);
    string str;
    auto temp = 0;
    if (in.is_open()) {
        while (getline(in, str)) temp++;
    } else cout << "Error reading" << endl;
    in.close();
    return temp;
}

void getMask(int numberToMask, int numberArray, int *powerOfTwo) {
    cout << "Mask: ";
    for (int j = 0; j < numberArray; j++) {
        if ((numberToMask & powerOfTwo[j]) != 0) {
            cout << "1";
        } else {
            cout << "0";
        }
    }
    cout << "\n";
}