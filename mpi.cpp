#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#define send_data_tag 2001
#define return_data_tag 2002

int getNumberElements(const char *path);

void getMask(int numberToMask, int numberArray, int *powerOfTwo);

using namespace std;

int main(int argc, char **argv) {
    auto path = "sample.txt";

    int numberArray;

    int *arrX;
    int *arrY;

    int rank, ierr, numprocs, an_id, avg_rows_per_process, start_row, end_row;


    double x_amount = 0, y_amount = 0, xy_amount = 0;
    double x_square_amount = 0, y_square_amount = 0;

    double total_x_amount = 0, total_y_amount = 0, total_xy_amount = 0;
    double total_x_square_amount = 0, total_y_square_amount = 0;

    int root_process = 0;
    MPI_Comm Comm = MPI_COMM_WORLD;
    MPI_Status status;
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(Comm, &rank);
    ierr = MPI_Comm_size(Comm, &numprocs);

//    if (rank == 0) {
//        int t;
//        cout << "Please input number: ";
//        cin >> t;
//    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == root_process) {

        numberArray = getNumberElements(path);
        int variants = pow(2, numberArray);
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

        //Вычисляем и отправляем каждому процессу свой кусочек массива
        for (int an_id = 1; an_id < numprocs; an_id++) {
            start_row = an_id * avg_rows_per_process + 1;
            end_row = (an_id + 1) * avg_rows_per_process;

            if ((numberArray - end_row) < avg_rows_per_process)
                end_row = numberArray - 1;

            int num_rows_to_send = end_row - start_row + 1;

            ierr = MPI_Send(&num_rows_to_send, 1, MPI_INT,
                            an_id, send_data_tag, MPI_COMM_WORLD);

            ierr = MPI_Send(&arrX[start_row], num_rows_to_send, MPI_INT,
                            an_id, send_data_tag, MPI_COMM_WORLD);

            ierr = MPI_Send(&arrY[start_row], num_rows_to_send, MPI_INT,
                            an_id, send_data_tag, MPI_COMM_WORLD);
        }

        for (int i = 0; i < avg_rows_per_process; i++) {
            x_amount += arrX[i];

            y_amount += arrY[i];

            xy_amount += arrX[i] * arrY[i];

            x_square_amount += arrX[i] * arrX[i];
            y_square_amount += arrY[i] * arrY[i];
        }

        MPI_Reduce(&x_amount, &total_x_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&y_amount, &total_y_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&xy_amount, &total_xy_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&x_square_amount, &total_x_square_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&y_square_amount, &total_y_square_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        double result = (numberArray * xy_amount - x_amount * y_amount)
                        / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                               * (numberArray * y_square_amount - y_amount * y_amount));

        cout << result;

        delete[] arrX;
        delete[] arrY;
    } else {
        int numRowToRecieve;

        ierr = MPI_Recv(&numRowToRecieve, 1, MPI_INT,
                        root_process, send_data_tag, MPI_COMM_WORLD, &status);

        int *arrX2 = new int[numRowToRecieve];
        int *arrY2 = new int[numRowToRecieve];

        ierr = MPI_Recv(&arrX2[0], numRowToRecieve, MPI_INT,
                        root_process, send_data_tag, MPI_COMM_WORLD, &status);
        ierr = MPI_Recv(&arrY2[0], numRowToRecieve, MPI_INT,
                        root_process, send_data_tag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < numRowToRecieve; i++) {
            x_amount += arrX2[i];

            y_amount += arrY2[i];

            xy_amount += arrX2[i] * arrY2[i];

            x_square_amount += arrX2[i] * arrX2[i];
            y_square_amount += arrY2[i] * arrY2[i];
        }

        MPI_Reduce(&x_amount, &total_x_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&y_amount, &total_y_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&xy_amount, &total_xy_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&x_square_amount, &total_x_square_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&y_square_amount, &total_y_square_amount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    ierr = MPI_Finalize();
    return 0;
}

//double calculate(int start_row, int end_row, const int numberArray, const double* arrX, const double* arrY) {
//	double x_amount = 0, y_amount = 0, xy_amount = 0;
//	double x_square_amount = 0, y_square_amount = 0;
//	for (int i = start_row; i < end_row + 1; i++) {
//		for (int i = 0; i < numberArray; i++) { // вот ето лишнее
//			// sum of elements of array arrX.
//			x_amount += arrX[i];
//
//			// sum of elements of array arrY.
//			y_amount += arrY[i];
//
//			// sum of arrX[i] * arrY[i].
//			xy_amount += arrX[i] * arrY[i];
//
//			// sum of square of array elements.
//			x_square_amount += arrX[i] * arrX[i];
//			y_square_amount += arrY[i] * arrY[i];
//		}
//	}
//	return result;
//}


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