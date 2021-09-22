#include <iostream>
#include <fstream>
#include "/usr/local/opt/libomp/include/omp.h"
#include <string>
#include <sstream>
#include <cmath>

int get_number_elements(const char *path);

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

    double avgX = 0;
    double localAvgX = 0;
    for (int i = 0; i < numberArray; ++i) {
        localAvgX += arrX[i];
    }
    avgX += localAvgX;

    double avgY = 0;
    double localAvgY = 0;

    for (int i = 0; i < numberArray; ++i) {
        localAvgY += arrY[i];
    }

    avgY += localAvgY;

    double amount1 = 0;
    double amount2 = 0;
    double amount3 = 0;

    for (int i = 0; i < numberArray; ++i) {
        amount1 += (arrX[i] - avgX / numberArray) * (arrY[i] - avgY / numberArray);
        amount2 += pow((arrX[i] - avgX / numberArray), 2);
        amount3 += pow((arrY[i] - avgY / numberArray), 2);
    }

    double correlation = amount1 / (sqrt(amount2 * amount3));
    cout << "Correlation coefficient: " << correlation << endl;

    auto end = chrono::steady_clock::now();

    auto elapsed_ms = chrono::duration_cast<chrono::nanoseconds>(end - begin);
    cout << "The nonparallel time: " << elapsed_ms.count() << " ns\n";

    delete[] arrX;
    delete[] arrY;
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
    } else {
        cout << "Error reading" << endl;
    }
    in.close();
    return temp;
}
