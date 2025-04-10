#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__

#include "Block.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class TreeNode
{
public:
    Block block;
    TreeNode *child[4];
    bool isLeaf;

    static int varianceMethod;
    static double varianceThreshold;
    static int minBlockSize;

    TreeNode(int x, int y, int width, int height, const vector<vector<vector<int>>> *rgbMatrix);
    ~TreeNode();

    void subdivide(const vector<vector<vector<int>>> *rgbMatrix);
    void draw(Mat &output);
    int getMaxDepth() const;
    int getTotalNodes() const;
    void drawAtDepth(Mat &output, int targetDepth, int currentDepth = 1);

private:
    int getMaxDepthRecursive() const;
    int getTotalNodesRecursive() const;
};

class Quadtree
{
private:
    int width, height;
    TreeNode *root;
    const vector<vector<vector<int>>> *rgbMatrix;

public:
    Quadtree(int width, int height, const vector<vector<vector<int>>> *rgbMatrix,
             int varianceMethod, double varianceThreshold, int minBlockSize);
    ~Quadtree();

    void compressImage();
    bool saveCompressedImage(const string &outputPath);
    const vector<vector<vector<int>>> *getImageMatrix() const;
    int getTreeDepth();
    int getTotalNodes();
    void renderAtDepth(Mat &output, int depth);
};

#endif