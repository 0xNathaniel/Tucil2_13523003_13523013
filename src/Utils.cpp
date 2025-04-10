#include "Utils.hpp"
#include "Quadtree.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

string convertWindowsToWSLPath(const string &windowsPath)
{
    if (windowsPath.length() < 3 || windowsPath[1] != ':' || windowsPath[2] != '\\')
    {
        return windowsPath;
    }

    string wslPath = "/mnt/";
    wslPath += tolower(windowsPath[0]);
    wslPath += windowsPath.substr(2);

    replace(wslPath.begin(), wslPath.end(), '\\', '/');

    return wslPath;
}

tuple<int, double, int> validateInputConstraints(
    const string &inputImagePath,
    const vector<vector<vector<int>>> &rgbMatrix,
    int width,
    int height)
{
    int varianceMethod;
    double varianceThreshold = 0.0;
    int minBlockSize = 0;
    double targetCompression = 0.0;
    string tempOutputPath;

    while (true)
    {
        cout << "Masukan target kompresi (0.0-1.0): ";
        cin >> targetCompression;

        if (targetCompression >= 0.0 && targetCompression <= 1.0)
        {
            break;
        }
        cout << "Persentase kompresi harus diantara 0 dan 1, coba lagi!" << endl;
    }

    if (targetCompression != 0)
    {
        tie(varianceMethod, varianceThreshold, minBlockSize, tempOutputPath) =
            findOptimalSettingsBinarySearch(targetCompression, inputImagePath, rgbMatrix, width, height);

        if (remove(convertWindowsToWSLPath(tempOutputPath).c_str()) != 0)
        {
            cout << "Gagal menghapus.." << tempOutputPath << endl;
        }
        return make_tuple(varianceMethod, varianceThreshold, minBlockSize);
    }

    while (true)
    {
        cout << "Masuk mode manual" << endl;
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

    while (true)
    {
        cout << "Masukkan threshold variance: ";
        cin >> varianceThreshold;
        if (varianceMethod == 5)
        {
            if (varianceThreshold > 0 && varianceThreshold < 1)
            {
                break;
            }
            cout << "Threshold harus bernilai antara 0-1. Coba lagi." << endl;
        }
        else
        {
            if (varianceThreshold > 0)
            {
                break;
            }
            cout << "Threshold harus lebih besar dari 0. Coba lagi." << endl;
        }
    }

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
    ifstream file(convertWindowsToWSLPath(filePath), ifstream::ate | ifstream::binary);
    if (!file.is_open())
    {
        return -1;
    }
    return file.tellg();
}

tuple<int, double, int, string> findOptimalSettingsBinarySearch(
    double targetCompression,
    const string &inputImagePath,
    const vector<vector<vector<int>>> &rgbMatrix,
    int width,
    int height)
{
    const long long originalFileSize = getFileSize(inputImagePath);
    if (originalFileSize <= 0)
    {
        cout << "Size file original tidak dapat ditentukan." << endl;
        return make_tuple(1, 50.0, 4, inputImagePath);
    }

    int bestMethod = 1;
    double bestThreshold = 0.0;
    int bestMinBlockSize = 0;
    double bestCompressionDiff = 1.0;

    // Create a single temporary file path that will be reused
    string tempOutputPath = inputImagePath.substr(0, inputImagePath.find_last_of('.')) +
                            "_temp_compressed" +
                            inputImagePath.substr(inputImagePath.find_last_of('.'));

    for (int method = 1; method <= 5; method++)
    {
        double minThreshold, maxThreshold;
        if (method == 5)
        {
            minThreshold = 0.1;
            maxThreshold = 0.95;
        }
        else if (method == 4)
        {
            minThreshold = 0.1;
            maxThreshold = 8.0;
        }
        else
        {
            minThreshold = 1.0;
            maxThreshold = 255.0;
        }

        int minBlockSizes[] = {1, 2, 4, 8, 16};

        for (int blockSize : minBlockSizes)
        {
            double lowThreshold = minThreshold;
            double highThreshold = maxThreshold;

            for (int iteration = 0; iteration < 15; iteration++)
            {
                double midThreshold = lowThreshold + (highThreshold - lowThreshold) / 2.0;

                try
                {
                    Quadtree quadtree(width, height, &rgbMatrix, method, midThreshold, blockSize);
                    quadtree.compressImage();

                    if (!quadtree.saveCompressedImage(tempOutputPath))
                    {
                        cout << "Gagal menyimpan temp." << endl;
                        continue;
                    }

                    long long compressedSize = getFileSize(tempOutputPath);
                    if (compressedSize <= 0)
                    {
                        cout << "Gagal membaca." << endl;
                        continue;
                    }

                    double achievedCompression = 1.0 - (static_cast<double>(compressedSize) / originalFileSize);

                    double compressionDiff = abs(achievedCompression - targetCompression);
                    if (compressionDiff < bestCompressionDiff)
                    {
                        bestCompressionDiff = compressionDiff;
                        bestMethod = method;
                        bestThreshold = midThreshold;
                        bestMinBlockSize = blockSize;
                    }

                    if (achievedCompression < targetCompression)
                    {
                        if (method == 5)
                        {
                            lowThreshold = midThreshold;
                        }
                        else
                        {
                            highThreshold = midThreshold;
                        }
                    }
                    else
                    {
                        if (method == 5)
                        {
                            highThreshold = midThreshold;
                        }
                        else
                        {
                            lowThreshold = midThreshold;
                        }
                    }
                }
                catch (const exception &e)
                {
                    cout << "Error: " << e.what() << endl;
                }
            }
        }
    }

    return make_tuple(bestMethod, bestThreshold, bestMinBlockSize, tempOutputPath);
}

string askValidPath(const string &text)
{
    string path;

    while (true)
    {
        cout << "Masukkan path absolut " << text << " (misal: /home/user/output.png) atau exit: ";
        getline(cin, path);

        if (path == "exit")
        {
            return path;
        }

        path = convertWindowsToWSLPath(path);

        if (path.empty())
        {
            cout << "Path " << text << " tidak boleh kosong." << endl;
            continue;
        }

        fs::path outputPath(path);
        fs::path parentDir = outputPath.parent_path();

        if (!outputPath.is_absolute())
        {
            cout << "Path harus absolut. Coba lagi." << endl;
            continue;
        }

        if (!fs::exists(parentDir))
        {
            cout << "Direktori tidak ditemukan: " << parentDir << endl;
            continue;
        }

        break;
    }

    return path;
}