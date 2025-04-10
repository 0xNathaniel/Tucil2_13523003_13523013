#include "Quadtree.hpp"
#include <opencv2/opencv.hpp>
#include "Utils.hpp"

using namespace std;
using namespace cv;

int TreeNode::varianceMethod = 0;
double TreeNode::varianceThreshold = 0.0;
int TreeNode::minBlockSize = 0;

TreeNode::TreeNode(int x, int y, int width, int height, const vector<vector<vector<int>>> *rgbMatrix)
    : block(x, y, width, height, TreeNode::minBlockSize, TreeNode::varianceThreshold, TreeNode::varianceMethod, rgbMatrix), isLeaf(true)
{
    for (int i = 0; i < 4; i++)
    {
        child[i] = nullptr;
    }
    subdivide(rgbMatrix);
}

TreeNode::~TreeNode()
{
    for (int i = 0; i < 4; i++)
    {
        if (child[i] != nullptr)
        {
            delete child[i];
            child[i] = nullptr;
        }
    }
}

void TreeNode::subdivide(const vector<vector<vector<int>>> *rgbMatrix)
{
    if (!block.calcIsValid())
    {
        isLeaf = false;

        int childWidth = block.getWidth() / 2;
        int childHeight = block.getHeight() / 2;

        int remainderWidth = block.getWidth() % 2;
        int remainderHeight = block.getHeight() % 2;

        child[0] = new TreeNode(block.getX(), block.getY(), childWidth, childHeight, rgbMatrix);
        child[1] = new TreeNode(block.getX() + childWidth, block.getY(), childWidth + remainderWidth, childHeight, rgbMatrix);
        child[2] = new TreeNode(block.getX(), block.getY() + childHeight, childWidth, childHeight + remainderHeight, rgbMatrix);
        child[3] = new TreeNode(block.getX() + childWidth, block.getY() + childHeight, childWidth + remainderWidth, childHeight + remainderHeight, rgbMatrix);
    }
    else
    {
        isLeaf = true;
    }
}

void TreeNode::draw(Mat &output)
{
    if (isLeaf)
    {
        vector<double> avgColor = block.getAverageRGB();
        Scalar color(avgColor[2], avgColor[1], avgColor[0]);

        Point topLeft(block.getX(), block.getY());
        Point bottomRight(block.getX() + block.getWidth(), block.getY() + block.getHeight());

        rectangle(output, topLeft, bottomRight - Point(1, 1), color, -1);
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (child[i] != nullptr)
            {
                child[i]->draw(output);
            }
        }
    }
}

int TreeNode::getMaxDepth() const
{
    return getMaxDepthRecursive();
}

int TreeNode::getMaxDepthRecursive() const
{
    if (isLeaf)
    {
        return 1;
    }

    int maxChildDepth = 0;
    for (int i = 0; i < 4; i++)
    {
        if (child[i] != nullptr)
        {
            int childDepth = child[i]->getMaxDepthRecursive();
            if (childDepth > maxChildDepth)
            {
                maxChildDepth = childDepth;
            }
        }
    }
    return maxChildDepth + 1;
}

int TreeNode::getTotalNodes() const
{
    return getTotalNodesRecursive();
}

int TreeNode::getTotalNodesRecursive() const
{
    int count = 1;

    if (!isLeaf)
    {
        for (int i = 0; i < 4; i++)
        {
            if (child[i] != nullptr)
            {
                count += child[i]->getTotalNodesRecursive();
            }
        }
    }
    return count;
}

Quadtree::Quadtree(int width, int height, const vector<vector<vector<int>>> *rgbMatrix, int varianceMethod, double varianceThreshold, int minBlockSize) : width(width), height(height), rgbMatrix(rgbMatrix)
{
    TreeNode::varianceMethod = varianceMethod;
    TreeNode::varianceThreshold = varianceThreshold;
    TreeNode::minBlockSize = minBlockSize;

    root = nullptr;
}

Quadtree::~Quadtree()
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
}

void Quadtree::compressImage()
{
    if (root == nullptr)
    {
        root = new TreeNode(0, 0, width, height, rgbMatrix);
    }
}

bool Quadtree::saveCompressedImage(const string &outputPath)
{
    if (root == nullptr)
    {
        return false;
    }

    int width = root->block.getWidth();
    int height = root->block.getHeight();

    Mat output(height, width, CV_8UC3, Scalar(255, 255, 255));

    root->draw(output);

    return imwrite(convertWindowsToWSLPath(outputPath), output);
}

int Quadtree::getTreeDepth()
{
    if (root == nullptr)
    {
        return 0;
    }
    return root->getMaxDepth();
}

int Quadtree::getTotalNodes()
{
    if (root == nullptr)
    {
        return 0;
    }
    return root->getTotalNodes();
}