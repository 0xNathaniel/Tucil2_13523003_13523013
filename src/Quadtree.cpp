#include "Quadtree.hpp"

using namespace std;

// Variance method encodings:
// 0: Variance
// 1: Mean Absolute Deviation (MAD)
// 2: Max Pixel Difference
// 3: Entropy

int Node::varianceMethod = -1;
double Node::varianceThreshold = -1;
int Node::minBlockSize = -1;

Node::Node(int x, int y, int width, int height, const vector<vector<vector<int>>>* rgbMatrix) 
    : block(x, y, width, height, rgbMatrix) {
    isLeaf = true;
}

bool Node::isValidConstraints() {
    if (varianceMethod == 0) {
        block.calculateVariance();
        return (block.getArea() > minBlockSize) && (block.getVariance() > varianceThreshold);
    }
    if (varianceMethod == 1) {
        block.calculateMeanAbsoluteDeviation();
        return (block.getArea() > minBlockSize) && (block.getMeanAbsoluteDeviation() > varianceThreshold);
    }
    if (varianceMethod == 2) {
        block.calculateMaxPixelDiff();
        return (block.getArea() > minBlockSize) && (block.getMaxPixelDiff() > varianceThreshold);
    }
    if (varianceMethod == 3) {
        block.calculateEntropy();
        return (block.getArea() > minBlockSize) && (block.getEntropy() > varianceThreshold);
    }
    return false;
}

Quadtree::Quadtree(int width, int height, const vector<vector<vector<int>>>* rgbMatrix, int varianceMethod, double varianceThreshold, int minBlockSize) {
    root = new Node(0, 0, width, height, rgbMatrix);
    Node::varianceMethod = varianceMethod;
    Node::varianceThreshold = varianceThreshold;
    Node::minBlockSize = minBlockSize;
}


void Quadtree::compressImage() {

}

