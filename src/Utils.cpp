#include "Utils.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

string convertWindowsToWSLPath(const string &windowsPath)
{
    if (windowsPath.length() < 3 || windowsPath[1] != ':' || windowsPath[2] != '\\')
    {
        return "";
    }

    string wslPath = "/mnt/";
    wslPath += tolower(windowsPath[0]);
    wslPath += windowsPath.substr(2);

    replace(wslPath.begin(), wslPath.end(), '\\', '/');

    return wslPath;
}

tuple<double, double, int> validateInputConstraints()
{
    int varianceMethod;
    double varianceThreshold = 0.0;
    int minBlockSize = 0;

    // Input varianceMethod
    while (true)
    {
        cout << "Pilih metode variance sesuai angkanya (1-4): ";
        cin >> varianceMethod;
        if (varianceMethod == 1 || varianceMethod == 2 || varianceMethod == 3 || varianceMethod == 4)
        {
            break;
        }
        cout << "Input tidak valid. Coba lagi.\n";
    }

    // Input varianceThreshold
    while (true)
    {
        cout << "Masukkan threshold variance: ";
        cin >> varianceThreshold;
        if (varianceThreshold > 0)
        {
            break;
        }
        cout << "Threshold harus lebih besar dari 0. Coba lagi.\n";
    }

    // Input minBlockSize
    while (true)
    {
        cout << "Masukkan ukuran blok minimum: ";
        cin >> minBlockSize;
        if (minBlockSize > 0)
        {
            break;
        }
        cout << "Ukuran blok harus lebih besar dari 0. Coba lagi.\n";
    }

    return make_tuple(varianceMethod - 1, varianceThreshold, minBlockSize);
}