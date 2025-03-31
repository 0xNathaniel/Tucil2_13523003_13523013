#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include "ImageProcessor.hpp"

using namespace cv;
using namespace std;

int main()
{
    string imagePath = ImageProcessor::inputImagePath();
    if (imagePath != "exit") {
        return -1;
    }

    vector<vector<vector<int>>> rgbMatrix = ImageProcessor::loadImage(imagePath);
    /* 
    cout << "[";
    for (int i = 0; i < rows; i++)
    {
        cout << "[";
        for (int j = 0; j < cols; j++)
        {
            cout << "[" << rgbMatrix[i][j][0] << "," << rgbMatrix[i][j][1] << "," << rgbMatrix[i][j][2] << "]";
            if (j < cols - 1)
                cout << ",";
        }
        cout << "]";
        if (i < rows - 1)
            cout << ",";
    }
    cout << "]" << endl; 
    */

    return 0;
}
