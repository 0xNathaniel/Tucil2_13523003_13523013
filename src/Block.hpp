#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__
#include <vector>

using namespace std;

class Block
{
private:
    int x, y; //(x,y) location of pixel from top left of image
    int width, height, area, minBlockSize, methodNum;
    double threshold;
    const vector<vector<vector<int>>> *rgbMatrix;
    vector<double> averageRGB;

public:
    // Constructor
    Block(int x, int y, int width, int height, int minBlockSize, double threshold, int methodNum, const vector<vector<vector<int>>> *rgbMatrix);

    // Destructor
    ~Block();

    // Getter
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getArea() const;

    // Functions
    vector<double> getAverageRGB() const;
    double getMaxPixelDiff() const;
    double getVariance() const;
    double getMeanAbsoluteDeviation() const;
    double getEntropy() const;

    // Validation
    bool calcIsValid() const;

    // Note: belum ada attribute dan method SSIM
};

#endif