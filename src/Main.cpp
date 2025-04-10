#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "ImageProcessor.hpp"
#include "Quadtree.hpp"
#include "Utils.hpp"
#include "GIF.hpp"

using namespace cv;
using namespace std;

int main()
{
    string inputImagePath;
    string outputImagePath;
    string outputGIFPath;
    int varianceMethod;
    double varianceThreshold;
    int minBlockSize;
    vector<vector<vector<int>>> rgbMatrix;
    int gifFrameDelay = 1000;

    // Input image path and load image
    inputImagePath = ImageProcessor::inputImagePath();
    if (inputImagePath == "exit")
    {
        return -1;
    }
    rgbMatrix = ImageProcessor::loadImage(inputImagePath);

    int width = rgbMatrix[0].size();
    int height = rgbMatrix.size();

    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints(inputImagePath, rgbMatrix, width, height);

    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints(inputImagePath, rgbMatrix, width, height);

    // Input image path (output image)
    cout << "Masukkan alamat absolut gambar output: ";
    outputImagePath = askValidPath("image output");

    // Input gif output path
    cout << "Masukkan alamat absolut GIF output: ";
    outputGIFPath = askValidPath("GIF output");

    long long originalFileSize = getFileSize(inputImagePath);

    auto start = chrono::high_resolution_clock::now();

    Quadtree quadtree(width, height, &rgbMatrix, varianceMethod, varianceThreshold, minBlockSize);
    cout << "Compressing . . . " << endl;
    quadtree.compressImage();

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    // Compression process
    if (quadtree.saveCompressedImage(outputImagePath))
    {
        long long compressedFileSize = getFileSize(outputImagePath);

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

        // Create and save GIF visualization
        cout << "Generating GIF visualization..." << endl;
        GIF gifGenerator(width, height, gifFrameDelay);
        gifGenerator.generateFramesFromQuadtree(quadtree);

        if (gifGenerator.saveGif(outputGIFPath))
        {
            cout << "GIF Disimpan Ke: " << outputGIFPath << endl;
        }
        else
        {
            cout << "Gagal Menyimpan GIF." << endl;
        }
    }
    else
    {
        cout << "Gagal Menyimpan Gambar." << endl;
    }

    return 0;
}