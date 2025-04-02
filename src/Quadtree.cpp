#include "Quadtree.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Initialize static members
int TreeNode::varianceMethod = 0;
double TreeNode::varianceThreshold = 0.0;
int TreeNode::minBlockSize = 0;

// TreeNode constructor
TreeNode::TreeNode(int x, int y, int width, int height, const vector<vector<vector<int>>> *rgbMatrix)
    : block(x, y, width, height, TreeNode::minBlockSize, TreeNode::varianceThreshold, TreeNode::varianceMethod, rgbMatrix), isLeaf(true)
{
    cout << "creating tree node";
    for (int i = 0; i < 4; i++)
    {
        child[i] = nullptr;
    }
    cout << "subdividing";
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
        // This is a leaf TreeNode, fill with average color
        vector<double> avgColor = block.getAverageRGB();
        Scalar color(avgColor[2], avgColor[1], avgColor[0]); // BGR format for OpenCV

        // Draw rectangle with average color
        // Use Point() constructor for exact pixel positioning
        Point topLeft(block.getX(), block.getY());
        Point bottomRight(block.getX() + block.getWidth(), block.getY() + block.getHeight());

        // Fill the rectangle with color (no gaps)
        rectangle(output, topLeft, bottomRight - Point(1, 1), color, -1); // -1 for filled rectangle
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

int TreeNode::countLeafTreeNodes() const
{
    if (isLeaf)
    {
        return 1;
    }

    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (child[i] != nullptr)
        {
            count += child[i]->countLeafTreeNodes();
        }
    }
    return count;
}

// Quadtree constructor
Quadtree::Quadtree(int width, int height, const vector<vector<vector<int>>> *rgbMatrix, int varianceMethod, double varianceThreshold, int minBlockSize) : width(width), height(height), rgbMatrix(rgbMatrix)
{
    // Set static TreeNode parameters
    TreeNode::varianceMethod = varianceMethod;
    TreeNode::varianceThreshold = varianceThreshold;
    TreeNode::minBlockSize = minBlockSize;

    // Create root Treenode
    root = nullptr;
}

// Quadtree destructor
Quadtree::~Quadtree()
{
    if (root != nullptr)
    {
        delete root;
        root = nullptr;
    }
}

// Compress the image using quadtree
void Quadtree::compressImage()
{
    // Create the root Treenode, which will recursively create the entire tree
    if (root == nullptr)
    {
        root = new TreeNode(0, 0, width, height, rgbMatrix);
    }
}

// Save the compressed image to a file
bool Quadtree::saveCompressedImage(const string &outputPath)
{
    if (root == nullptr)
    {
        return false;
    }

    // Get the dimensions from the root Treenode's block
    int width = root->block.getWidth();
    int height = root->block.getHeight();

    // Create output image
    Mat output(height, width, CV_8UC3, Scalar(255, 255, 255));

    // Draw the quadtree
    root->draw(output);

    // Save the image
    return imwrite(outputPath, output);
}

// Get the size of the compressed representation (number of blocks * bytes per block)
double Quadtree::getCompressedSize()
{
    if (root == nullptr)
    {
        return 0.0;
    }

    // Count leaf Treenodes
    int leafCount = root->countLeafTreeNodes();

    // Each leaf Treenode stores position (2 integers), size (2 integers), and color (3 bytes)
    // Total: 4 * sizeof(int) + 3 * sizeof(char) per leaf
    return leafCount * (4 * sizeof(int) + 3 * sizeof(char));
}