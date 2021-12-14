#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

int get_number_elements(const char *path);

using namespace std;

int main(int argc, char **argv) {
    auto path = "sample.txt";

    const auto numberArray = get_number_elements(path);
    double arrX[numberArray];
    double arrY[numberArray];

    int rank, numprocs, elements_per_process, n_elements_recieved;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

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

    double wtime = MPI_Wtime();
    double x_amount = 0, y_amount = 0, xy_amount = 0;
    double x_square_amount = 0, y_square_amount = 0;

    for (int i = 0; i < numberArray; i++) {
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
    double coefficient = (numberArray * xy_amount - x_amount * y_amount)
                    / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                           * (numberArray * y_square_amount - y_amount * y_amount));

    double wtime = MPI_Wtime() - wtime;

    if (rank == 0) {
        cout << "Correlation coefficient: " << coefficient << endl;
        cout << "The parallel time: " << wtime << " seconds\n";
    }

    MPI_Finalize();
    return 0;
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