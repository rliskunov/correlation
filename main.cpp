#include <iostream>
#include <fstream>
#include "/usr/local/opt/libomp/include/omp.h"
#include <sstream>
#include <cmath>

int get_number_elements(const char *path);

double non_parallel(int numberArray, const double *arrX, const double *arrY);

double parallel(int numberArray, const double *arrX, const double *arrY);

using namespace std;

int main() {
    auto path = R"(/Users/roman/CLionProjects/correlation/sample.txt)";

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
            int x, y;
            istringstream iss(line);
            iss >> x >> y;
            arrX[count] = x;
            arrY[count] = y;
            count++;
        }
    }
    file.close();

    auto begin = chrono::steady_clock::now();
    auto p_coefficient = parallel(numberArray, arrX, arrY);
    auto end = chrono::steady_clock::now();
    cout << "Correlation coefficient: " << p_coefficient << endl;
    auto elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "The parallel time: " << elapsed_ms.count() << " microseconds\n";

    begin = chrono::steady_clock::now();
    auto n_coefficient = non_parallel(numberArray, arrX, arrY);
    end = chrono::steady_clock::now();
    cout << "Correlation coefficient: " << n_coefficient << endl;
    elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "The nonparallel time: " << elapsed_ms.count() << " microseconds\n\n";

    delete[] arrX;
    delete[] arrY;
    return 0;
}

double non_parallel(const int numberArray, const double *arrX, const double *arrY) {
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

    return static_cast<double> (numberArray * xy_amount - x_amount * y_amount)
           / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                  * (numberArray * y_square_amount - y_amount * y_amount));
}

double parallel(const int numberArray, const double *arrX, const double *arrY) {
    double result = 0;
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
    result = static_cast<double> (numberArray * xy_amount - x_amount * y_amount)
             / sqrt((numberArray * x_square_amount - x_amount * x_amount)
                    * (numberArray * y_square_amount - y_amount * y_amount));

    return result;
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
