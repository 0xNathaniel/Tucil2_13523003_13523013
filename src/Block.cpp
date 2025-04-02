#include "Block.hpp"
#include <bits/stdc++.h>
#include <vector>

Block::Block(int x, int y, int width, int height, int minBlockSize, double threshold, int methodNum, const vector<vector<vector<int>>> *rgbMatrix) : x(x), y(y), width(width), height(height), area(width * height), minBlockSize(minBlockSize), methodNum(methodNum), threshold(threshold), rgbMatrix(rgbMatrix)
{
    area = width * height;
    vector<double> averageRGB;
    averageRGB = getAverageRGB();
}

int Block::getX() const
{
    return x;
}

int Block::getY() const
{
    return y;
}

int Block::getWidth() const
{
    return width;
}

int Block::getHeight() const
{
    return height;
}

int Block::getArea() const
{
    return area;
}

vector<double> Block::getAverageRGB() const
{
    double sumR = 0, sumG = 0, sumB = 0;
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            sumR += (*rgbMatrix)[i][j][0];
            sumG += (*rgbMatrix)[i][j][1];
            sumB += (*rgbMatrix)[i][j][2];
        }
    }
    vector<double> averageRGB(3);
    averageRGB[0] = sumR / area;
    averageRGB[1] = sumG / area;
    averageRGB[2] = sumB / area;

    return averageRGB;
}

double Block::getMaxPixelDiff() const
{
    double maxR = (*rgbMatrix)[x][y][0];
    double maxG = (*rgbMatrix)[x][y][1];
    double maxB = (*rgbMatrix)[x][y][2];
    double minR = (*rgbMatrix)[x][y][0];
    double minG = (*rgbMatrix)[x][y][1];
    double minB = (*rgbMatrix)[x][y][2];

    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            if ((double)(*rgbMatrix)[i][j][0] > maxR)
            {
                maxR = (*rgbMatrix)[i][j][0];
            }
            if ((double)(*rgbMatrix)[i][j][1] > maxG)
            {
                maxG = (*rgbMatrix)[i][j][1];
            }
            if ((double)(*rgbMatrix)[i][j][2] > maxB)
            {
                maxB = (*rgbMatrix)[i][j][2];
            }
            if ((double)(*rgbMatrix)[i][j][0] < minR)
            {
                minR = (*rgbMatrix)[i][j][0];
            }
            if ((double)(*rgbMatrix)[i][j][1] < minG)
            {
                minG = (*rgbMatrix)[i][j][1];
            }
            if ((double)(*rgbMatrix)[i][j][2] < minB)
            {
                minB = (*rgbMatrix)[i][j][2];
            }
        }
    }

    return ((maxR - minR) + (maxG - minG) + (maxB - minB)) / 3.0;
}

double Block::getVariance() const
{
    double varianceR = 0, varianceG = 0, varianceB = 0;
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            varianceR += pow((double)(*rgbMatrix)[i][j][0] - averageRGB[0], 2);
            varianceG += pow((double)(*rgbMatrix)[i][j][1] - averageRGB[1], 2);
            varianceB += pow((double)(*rgbMatrix)[i][j][2] - averageRGB[2], 2);
        }
    }
    varianceR = varianceR / area;
    varianceG = varianceG / area;
    varianceB = varianceB / area;

    return (varianceR + varianceG + varianceB) / 3.0;
}

double Block::getMeanAbsoluteDeviation() const
{
    double madR = 0, madG = 0, madB = 0;
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            madR += abs((double)(*rgbMatrix)[i][j][0] - averageRGB[0]);
            madG += abs((double)(*rgbMatrix)[i][j][1] - averageRGB[1]);
            madB += abs((double)(*rgbMatrix)[i][j][2] - averageRGB[2]);
        }
    }
    madR = madR / area;
    madG = madG / area;
    madB = madB / area;

    return (madR + madG + madB) / 3.0;
}

double Block::getEntropy() const
{
    double entropyR = 0, entropyG = 0, entropyB = 0;
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            const double pixelR = (double)(*rgbMatrix)[i][j][0];
            const double pixelG = (double)(*rgbMatrix)[i][j][1];
            const double pixelB = (double)(*rgbMatrix)[i][j][2];

            if (pixelR > 0)
                entropyR += (pixelR * log2(pixelR));
            if (pixelG > 0)
                entropyG += (pixelG * log2(pixelG));
            if (pixelB > 0)
                entropyB += (pixelB * log2(pixelB));
        }
    }
    entropyR = -entropyR;
    entropyG = -entropyG;
    entropyB = -entropyB;

    return (entropyR + entropyG + entropyB) / 3.0;
}

bool Block::calcIsValid() const
{
    cout << "checking validity";
    if ((area / 4) < minBlockSize)
    {
        return true;
    }
    return (methodNum == 1 && getVariance() < threshold) ||
           (methodNum == 2 && getMeanAbsoluteDeviation() < threshold) ||
           (methodNum == 3 && getMaxPixelDiff() < threshold) ||
           (methodNum == 4 && getEntropy() < threshold);
}