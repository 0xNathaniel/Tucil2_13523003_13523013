#include "GIF.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

GIF::GIF(int width, int height, int frameDelay)
    : width(width), height(height), frameDelay(frameDelay)
{
    Magick::InitializeMagick(nullptr);
}

void GIF::addFrame(const Mat &frame)
{
    if (frame.cols == width && frame.rows == height) {
        frames.push_back(frame.clone());
    } else {
        Mat resizedFrame;
        resize(frame, resizedFrame, Size(width, height));
        frames.push_back(resizedFrame);
    }
}

void GIF::generateFramesFromQuadtree(Quadtree &quadtree)
{
    frames.clear();
    
    int maxDepth = quadtree.getTreeDepth();
    
    for (int depth = 1; depth <= maxDepth; depth++) {
        Mat frame(height, width, CV_8UC3, Scalar(255, 255, 255));
        quadtree.renderAtDepth(frame, depth);
        addFrame(frame);
    }
}

Magick::Image GIF::convertMatToMagickImage(const Mat& mat)
{
    Mat bgr;
    if (mat.channels() == 3) {
        bgr = mat;
    } else if (mat.channels() == 4) {
        cvtColor(mat, bgr, COLOR_BGRA2BGR);
    } else if (mat.channels() == 1) {
        cvtColor(mat, bgr, COLOR_GRAY2BGR);
    } else {
        throw runtime_error("Unsupported number of channels");
    }
    
    Magick::Image image(bgr.cols, bgr.rows, "BGR", Magick::CharPixel, bgr.data);
    return image;
}

bool GIF::saveGif(const string &outputPath)
{
    if (frames.empty()) {
        cout << "No frames to save." << endl;
        return false;
    }
    
    try {
        std::vector<Magick::Image> magickImages;
        
        for (const auto& frame : frames) {
            Magick::Image image = convertMatToMagickImage(frame);
            image.animationDelay(frameDelay / 10);
            magickImages.push_back(image);
        }
        
        for (auto& image : magickImages) {
            image.animationIterations(0); 
        }
        
        Magick::writeImages(magickImages.begin(), magickImages.end(), outputPath);
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving GIF: " << e.what() << std::endl;
        return false;
    }
}