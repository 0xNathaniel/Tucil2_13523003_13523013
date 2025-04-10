#include "Block.hpp"
#include <bits/stdc++.h>
#include <vector>

Block::Block(int x, int y, int width, int height, int minBlockSize, double threshold, int methodNum, const vector<vector<vector<int>>> *rgbMatrix) : x(x), y(y), width(width), height(height), area(width * height), minBlockSize(minBlockSize), methodNum(methodNum), threshold(threshold), rgbMatrix(rgbMatrix)
{
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
    double maxR = (*rgbMatrix)[y][x][0];
    double maxG = (*rgbMatrix)[y][x][1];
    double maxB = (*rgbMatrix)[y][x][2];
    double minR = (*rgbMatrix)[y][x][0];
    double minG = (*rgbMatrix)[y][x][1];
    double minB = (*rgbMatrix)[y][x][2];

    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
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
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
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
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
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
    vector<int> histR(256, 0);
    vector<int> histG(256, 0);
    vector<int> histB(256, 0);

    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            int r = (*rgbMatrix)[i][j][0];
            int g = (*rgbMatrix)[i][j][1];
            int b = (*rgbMatrix)[i][j][2];

            histR[r]++;
            histG[g]++;
            histB[b]++;
        }
    }

    double entropyR = 0, entropyG = 0, entropyB = 0;

    for (int i = 0; i < 256; i++)
    {
        double probR = (double)histR[i] / area;
        double probG = (double)histG[i] / area;
        double probB = (double)histB[i] / area;

        if (probR > 0)
            entropyR += (probR * log2(probR));
        if (probG > 0)
            entropyG += (probG * log2(probG));
        if (probB > 0)
            entropyB += (probB * log2(probB));
    }

    entropyR = -entropyR;
    entropyG = -entropyG;
    entropyB = -entropyB;

    return (entropyR + entropyG + entropyB) / 3.0;
}

double Block::getStructSimIdx() const
{
    const double K1 = 0.01, K2 = 0.03;
    const double L = 255.0;
    const double C1 = (K1 * L) * (K1 * L);
    const double C2 = (K2 * L) * (K2 * L);

    double ssimSum = 0.0;

    for (int channel = 0; channel < 3; channel++)
    {
        double muX = 0.0;
        double sigmaX = 0.0;

        for (int i = y; i < y + height; ++i)
        {
            for (int j = x; j < x + width; ++j)
            {
                muX += (*rgbMatrix)[i][j][channel];
            }
        }
        muX /= area;

        double muY = averageRGB[channel];

        for (int i = y; i < y + height; ++i)
        {
            for (int j = x; j < x + width; ++j)
            {
                double diff = (*rgbMatrix)[i][j][channel] - muX;
                sigmaX += diff * diff;
            }
        }
        sigmaX /= area;

        double sigmaXY = 0.0;
        for (int i = y; i < y + height; ++i)
        {
            for (int j = x; j < x + width; ++j)
            {
                sigmaXY += ((*rgbMatrix)[i][j][channel] - muX) * (muY - muY);
            }
        }
        sigmaXY /= area;

        double numerator = (2 * muX * muY + C1);
        double denominator = (muX * muX + muY * muY + C1) * (sigmaX + C2);
        double ssim = (denominator > 0) ? numerator / denominator : 1.0;

        if (channel == 0)
        {
            ssimSum += 0.3 * ssim;
        }
        else if (channel == 1)
        {
            ssimSum += 0.59 * ssim;
        }
        else
        {
            ssimSum += 0.11 * ssim;
        }
    }

    return 1.0 - ssimSum;
}

bool Block::calcIsValid() const
{
    return ((area / 4) < minBlockSize) ||
           (methodNum == 1 && getVariance() < threshold) ||
           (methodNum == 2 && getMeanAbsoluteDeviation() < threshold) ||
           (methodNum == 3 && getMaxPixelDiff() < threshold) ||
           (methodNum == 4 && getEntropy() < threshold) ||
           (methodNum == 5 && getStructSimIdx() < threshold);
}