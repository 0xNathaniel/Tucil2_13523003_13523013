#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include <vector>

using namespace std;

class Block {
    private:
        int x, y;
        int width, height, area;
        double maxPixelDiff;
        double variance;
        double meanAbsoluteDeviation;
        double entropy;
        vector<double> averageRGB;
        const vector<vector<vector<int>>>* rgbMatrix;
    public:
        // Constructor
        Block(int x, int y, int width, int height, const vector<vector<vector<int>>>* rgbMatrix);
        // Getter
        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;
        int getArea() const;
        double getMaxPixelDiff() const;
        double getVariance() const;
        double getMeanAbsoluteDeviation() const;
        double getEntropy() const;
        vector<double> getAverageRGB() const;
        // Calculate
        void calculateArea();
        void calculateMaxPixelDiff();
        void calculateVariance();
        void calculateMeanAbsoluteDeviation();
        void calculateEntropy();
        void calculateAverageRGB();

        // Note: belum ada attribute dan method SSIM
};

#endif