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
    int varianceMethod; // Encodings
    double varianceThreshold;
    int minBlockSize;
    vector<vector<vector<int>>> rgbMatrix;

    // Image processing (loading)
    imagePath = ImageProcessor::inputImagePath();
    if (imagePath == "exit")
    {
        return -1;
    }
    rgbMatrix = ImageProcessor::loadImage(imagePath);
    // for (int i = 0; i < rgbMatrix.size(); ++i)
    // {
    //     for (int j = 0; j < rgbMatrix[i].size(); ++j)
    //     {
    //         for (int k = 0; k < rgbMatrix[i][j].size(); ++k)
    //         {
    //             cout << "[" << i << "," << j << "," << k << "]="
    //                  << rgbMatrix[i][j][k] << endl;
    //         }
    //     }
    // }

    // Constraints input
    tie(varianceMethod, varianceThreshold, minBlockSize) = validateInputConstraints();

    return 0;
}
