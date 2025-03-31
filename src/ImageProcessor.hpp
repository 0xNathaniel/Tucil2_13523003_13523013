#ifndef __IMAGE_PROCESSOR_HPP__
#define __IMAGE_PROCESSOR_HPP__

#include <iostream>
#include <vector>
using namespace std;

class ImageProcessor {
    public:
        static string inputImagePath();
        static vector<vector<vector<int>>> loadImage(const string& imagePath);
};

#endif