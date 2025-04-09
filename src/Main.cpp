#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "ImageProcessor.hpp"
#include "Quadtree.hpp"
#include "Utils.hpp"

using namespace cv;
using namespace std;

int main()
{
    // Declarations

    // Variance method encodings:
    // 0: Variance
    // 1: Mean Absolute Deviation (MAD)
    // 2: Max Pixel Difference
    // 3: Entropy

    string inputImagePath;
    string outputImagePath;
    int varianceMethod;
    double varianceThreshold;
    int minBlockSize;
    vector<vector<vector<int>>> rgbMatrix;

    inputImagePath = ImageProcessor::inputImagePath();
    if (inputImagePath == "exit")
    {
        return -1;
    }
    rgbMatrix = ImageProcessor::loadImage(inputImagePath);

    // Constraints input
    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints();

    int width = rgbMatrix[0].size();
    int height = rgbMatrix.size();

    cout << "Masukkan alamat absolut gambar output: ";
    cin >> outputImagePath;

    long long originalFileSize = getFileSize(inputImagePath);

    auto start = std::chrono::high_resolution_clock::now();

    Quadtree quadtree(width, height, &rgbMatrix, varianceMethod, varianceThreshold, minBlockSize);
    cout << "Compressing . . . " << endl;
    quadtree.compressImage();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (quadtree.saveCompressedImage(outputImagePath))
    {
        long long compressedFileSize = getFileSize(convertWindowsToWSLPath(outputImagePath));

        double compressionPercentage = 0.0;
        if (originalFileSize > 0 && compressedFileSize > 0)
        {
            compressionPercentage = (1.0 - (static_cast<double>(compressedFileSize) / static_cast<double>(originalFileSize))) * 100.0;
        }

        cout << "Waktu Eksekusi: " << duration.count() << " ms" << endl;
        cout << "Ukuran Original: " << originalFileSize << " bytes" << endl;
        cout << "Ukuran Compressed: " << compressedFileSize << " bytes" << endl;
        cout << "% kompresi: " << compressionPercentage << "%" << endl;
        cout << "Kedalaman Pohon: " << quadtree.getTreeDepth() << endl;
        cout << "Banyak Simpul: " << quadtree.getTotalNodes() << endl;
        cout << "Gambar Disimpan Ke: " << outputImagePath << endl;
    }
    else
    {
        cout << "Gagal Menyimpan Gambar." << endl;
    }

    return 0;
}
