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

    printTitle();
    

    inputImagePath = ImageProcessor::inputImagePath();
    if (inputImagePath == "exit")
    {
        return -1;
    }
    rgbMatrix = ImageProcessor::loadImage(inputImagePath);

    int width = rgbMatrix[0].size();
    int height = rgbMatrix.size();

    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints(inputImagePath, rgbMatrix, width, height);
;
    outputImagePath = askValidPath("image output", {".jpg", ".jpeg", ".png"});
    
    outputGIFPath = askValidPath("GIF output", {".gif"});


    long long originalFileSize = getFileSize(inputImagePath);

    auto start = chrono::high_resolution_clock::now();

    Quadtree quadtree(width, height, &rgbMatrix, varianceMethod, varianceThreshold, minBlockSize);
    cout << "Compressing . . . " << endl << endl;
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

        cout << "Waktu eksekusi: " << duration.count() << " ms" << endl;
        cout << "Ukuran original: " << originalFileSize << " bytes" << endl;
        cout << "Ukuran compressed: " << compressedFileSize << " bytes" << endl;
        cout << "Persentase kompresi: " << compressionPercentage << "%" << endl;
        cout << "Kedalaman pohon: " << quadtree.getTreeDepth() << endl;
        cout << "Banyak simpul: " << quadtree.getTotalNodes() << endl << endl;

        cout << "Gambar berhasil disimpan di: " << outputImagePath << endl << endl;

        cout << "Membuat output GIF . . ." << endl;
        GIF gifGenerator(width, height, gifFrameDelay);
        gifGenerator.generateFramesFromQuadtree(quadtree);

        if (gifGenerator.saveGif(outputGIFPath))
        {
            cout << "GIF berhasil disimpan di: " << outputGIFPath << endl; 
        }
        else
        {
            cout << "Gagal menyimpan GIF!" << endl;
        }
    }
    else
    {
        cout << "Gagal menyimpan gambar!" << endl;
    }

    return 0;
}