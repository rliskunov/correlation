#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

int get_number_elements(const char *path);

using namespace std;

int main(int argc, char **argv) {
    int rank, numprocs, elements_per_process, n_elements_recieved;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    auto path = "sample.txt";

    const auto numberArray = get_number_elements(path);
    const auto numberArrayPart = numberArray / 2;
    auto *arrX = new double[numberArray];
    auto *arrX_part1 = new double[numberArrayPart];
    auto *arrX_part2 = new double[numberArrayPart];
    auto *arrY = new double[numberArray];
    auto *arrY_part1 = new double[numberArrayPart];
    auto *arrY_part2 = new double[numberArrayPart];

    ifstream file(path);
    if (!file.is_open()) {
        cout << "Error reading!" << endl;
    } else {
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
    }
    file.close();

    int amountX = 0;
    for (int i = 0; i < numberArray; i++) amountX += arrX[i];
    double averageX = amountX / numberArray;
    for (int i = 0; i < numberArray; i++)
        if (arrX[i] < averageX) arrX_part1[i] = arrX[i];
        else arrX_part2[i] = arrX[i];

    int amountY = 0;
    for (int i = 0; i < numberArray; i++) amountY += arrY[i];
    double averageY = amountY / numberArray;
    for (int i = 0; i < numberArray; i++)
        if (arrY[i] < averageY) arrY_part1[i] = arrY[i];
        else arrY_part2[i] = arrY[i];

    wtime = MPI_Wtime();

    double partial_coefficient = 0;
    double x_amount = 0, y_amount = 0, xy_amount = 0;
    double x_square_amount = 0, y_square_amount = 0;
    if (rank == 0) {
        for (int i = 0; i < numberArrayPart; i++) {
            // sum of elements of array arrX.
            x_amount += arrX_part1[i];

            // sum of elements of array arrY.
            y_amount += arrY_part1[i];

            // sum of arrX[i] * arrY[i].
            xy_amount += arrX_part1[i] * arrY_part1[i];

            // sum of square of array elements.
            x_square_amount += arrX_part1[i] * arrX_part1[i];
            y_square_amount += arrY_part1[i] * arrY_part1[i];

            partial_coefficient += (numberArray * xy_amount - x_amount * y_amount)
                              / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                                     * (numberArray * y_square_amount - y_amount * y_amount));
        }
    } else if (rank == 1) {
        for (int i = 0; i < numberArrayPart; i++) {
            // sum of elements of array arrX.
            x_amount += arrX_part2[i];

            // sum of elements of array arrY.
            y_amount += arrY_part2[i];

            // sum of arrX[i] * arrY[i].
            xy_amount += arrX_part2[i] * arrY_part2[i];

            // sum of square of array elements.
            x_square_amount += arrX_part2[i] * arrX_part2[i];
            y_square_amount += arrY_part2[i] * arrY_part2[i];

            partial_coefficient += (numberArray * xy_amount - x_amount * y_amount)
                              / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                                     * (numberArray * y_square_amount - y_amount * y_amount));
        }
    }
    double coefficient;
    MPI_Reduce(&partial_coefficient, &coefficient, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    wtime = MPI_Wtime() - wtime;

    if (rank == 0) {
        cout << "Correlation coefficient: " << coefficient << endl;
        cout << "The parallel time: " << wtime << " seconds\n";
    }

    delete[] arrX;
    delete[] arrX_part1;
    delete[] arrX_part2;
    delete[] arrY;
    delete[] arrY_part1;
    delete[] arrY_part2;

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