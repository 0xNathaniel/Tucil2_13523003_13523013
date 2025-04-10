#ifndef GIF_HPP
#define GIF_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <Magick++.h>
#include "Quadtree.hpp"

using namespace std;
using namespace cv;

class GIF {
private:
    vector<Mat> frames;
    int width;
    int height;
    int frameDelay;
    
    Magick::Image convertMatToMagickImage(const Mat& mat);

public:
    GIF(int width, int height, int frameDelay = 500);
    
    void addFrame(const Mat &frame);
    void generateFramesFromQuadtree(Quadtree &quadtree);
    bool saveGif(const string &outputPath);
};

#endif