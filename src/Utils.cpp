#include "Utils.hpp"
#include <iostream>



tuple<double, double, int> validateInputConstraints() {
    int varianceMethod;
    double varianceThreshold;
    int minBlockSize;

    // Input varianceMethod
    while (true) {
        cout << "Pilih metode variance sesuai angkanya (1-4)";
        cin >> varianceMethod;
        if (varianceMethod == 1 || varianceMethod == 2 || varianceMethod == 3|| varianceMethod == 4) {
            break;
        }
    }
    // Input varianceThreshold;
    while (true) {
        if (varianceThreshold > 0) {
            break;
        }
    }
    // Input minBlockSize
    while (true) {
        if (minBlockSize > 0) {
            break;
        }
    }

    return make_tuple(varianceMethod - 1, varianceThreshold, minBlockSize);
}