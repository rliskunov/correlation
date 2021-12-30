#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#define send_data_tag 2001
#define return_data_tag 2002

int getNumberElements(const char *path);

using namespace std;

int main(int argc, char **argv) {
    auto path = "sample.txt";

    int numberArray;
    double *arrX;
    double *arrY;

    int an_id, avg_rows_per_process, start_row, end_row;

    double x_amount = 0, y_amount = 0, xy_amount = 0;
    double x_square_amount = 0, y_square_amount = 0;

    double total_x_amount = 0, total_y_amount = 0, total_xy_amount = 0;
    double total_x_square_amount = 0, total_y_square_amount = 0;

    double start_time = MPI_Wtime();
    int root_process = 0;
    MPI_Comm Comm = MPI_COMM_WORLD;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(Comm, &rank);
    int num_procs;
    MPI_Comm_size(Comm, &num_procs);

    if (rank == root_process) {
        numberArray = getNumberElements(path);
        int variants = pow(2, numberArray);
        arrX = new double[numberArray];
        arrY = new double[numberArray];

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

        avg_rows_per_process = numberArray / num_procs;

        for (int an_id = 1; an_id < num_procs; an_id++) {
            start_row = an_id * avg_rows_per_process + 1;
            end_row = (an_id + 1) * avg_rows_per_process;

            if ((numberArray - end_row) < avg_rows_per_process)
                end_row = numberArray - 1;

            int num_rows_to_send = end_row - start_row + 1;

            MPI_Send(&num_rows_to_send, 1, MPI_INT, an_id, send_data_tag, MPI_COMM_WORLD);

            MPI_Send(&arrX[start_row], num_rows_to_send, MPI_INT, an_id, send_data_tag, MPI_COMM_WORLD);

            MPI_Send(&arrY[start_row], num_rows_to_send, MPI_INT, an_id, send_data_tag, MPI_COMM_WORLD);
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

        double result = (numberArray * total_xy_amount - total_x_amount * total_y_amount)
                        / sqrt((numberArray * total_x_square_amount - total_x_amount * total_x_amount)
                               * (numberArray * total_y_square_amount - total_y_amount * total_y_amount));

        cout << "Correlation coefficient: " << result << endl;

        double end_time = MPI_Wtime();
        cout << "The mpi time: " << end_time - start_time << " seconds\n";

        delete[] arrX;
        delete[] arrY;
    } else {
        int receive;

        MPI_Recv(&receive, 1, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);

        double *arrX2 = new double[receive];
        double *arrY2 = new double[receive];

        MPI_Recv(&arrX2[0], receive, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&arrY2[0], receive, MPI_INT, root_process, send_data_tag, MPI_COMM_WORLD, &status);

        for (int i = 0; i < receive; i++) {
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

    delete[] arrX;
    delete[] arrY;

    MPI_Finalize();
    return 0;
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