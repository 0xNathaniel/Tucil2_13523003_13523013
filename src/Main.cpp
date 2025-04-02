#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

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

    string imagePath;
    int varianceMethod;
    double varianceThreshold;
    int minBlockSize;
    vector<vector<vector<int>>> rgbMatrix;

    imagePath = ImageProcessor::inputImagePath();
    if (imagePath == "exit")
    {
        return -1;
    }
    rgbMatrix = ImageProcessor::loadImage(imagePath);

    // Constraints input
    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints();

    int width, height;
    width = rgbMatrix[0].size();
    height = rgbMatrix.size();

    Quadtree quadtree(width, height, &rgbMatrix, varianceMethod, varianceThreshold, minBlockSize);
    cout << "compressing" << endl;
    quadtree.compressImage();

    string outputPath = imagePath.substr(0, imagePath.find_last_of('.')) + "_compressed.jpg";
    if (quadtree.saveCompressedImage(outputPath))
    {
        cout << "Compressed image saved as: " << outputPath << endl;

        double originalSize = width * height * 3;
        double compressedSize = quadtree.getCompressedSize();
        double compressionRatio = originalSize / compressedSize;

        cout << "Original size: " << originalSize << " bytes" << endl;
        cout << "Compressed size: " << compressedSize << " bytes" << endl;
        cout << "Compression ratio: " << compressionRatio << ":1" << endl;
    }
    else
    {
        cout << "Failed to save compressed image." << endl;
    }

    return 0;
}
