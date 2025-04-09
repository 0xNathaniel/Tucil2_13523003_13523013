#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

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
        cout << "Metode variance:" << endl;
        cout << "(1) Variance" << endl;
        cout << "(2) Mean Absolute Deviation (MAD)" << endl;
        cout << "(3) Max Pixel Difference" << endl;
        cout << "(4) Entropy" << endl;
        cout << "(5) Structural Similarity Index (SSIM)" << endl;
        cout << "Pilih metode variance sesuai angkanya (1-5): ";
        cin >> varianceMethod;
        if (varianceMethod >= 1 && varianceMethod <= 5)
        {
            break;
        }
        cout << "Input tidak valid. Coba lagi." << endl
             << endl;
    }

    // Input varianceThreshold
    while (true)
    {
        cout << "Masukkan threshold variance: ";
        cin >> varianceThreshold;
        //  Validate SSIM variance threshold
        if (varianceMethod == 5)
        {
            if (varianceThreshold > 0 && varianceThreshold < 1)
            {
                break;
            }
            cout << "Threshold harus bernilai antara 0-1. Coba lagi." << endl;
        }
        // Validate other methods' variance threshold
        else
        {
            if (varianceThreshold > 0)
            {
                break;
            }
            cout << "Threshold harus lebih besar dari 0. Coba lagi." << endl;
        }
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

    return make_tuple(varianceMethod, varianceThreshold, minBlockSize);
}

long long getFileSize(const string &filePath)
{
    ifstream file(filePath, ifstream::ate | ifstream::binary);
    if (!file.is_open())
    {
        return -1;
    }
    return file.tellg();
}