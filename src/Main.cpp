#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
    string imagePath;
    cout << "Masukan alamat absolut gambar: ";
    cin >> imagePath;

    Mat image = imread(imagePath, IMREAD_COLOR);
    if (image.empty())
    {
        cerr << "Gambar tidak ditemukan" << endl;
        return -1;
    }

    int rows = image.rows;
    int cols = image.cols;
    vector<vector<vector<int>>> rgbMatrix(rows, vector<vector<int>>(cols, vector<int>(3)));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Vec3b pixel = image.at<Vec3b>(i, j);
            rgbMatrix[i][j][0] = pixel[2];
            rgbMatrix[i][j][1] = pixel[1];
            rgbMatrix[i][j][2] = pixel[0];
        }
    }

    cout << "[";
    for (int i = 0; i < rows; i++)
    {
        cout << "[";
        for (int j = 0; j < cols; j++)
        {
            cout << "[" << rgbMatrix[i][j][0] << "," << rgbMatrix[i][j][1] << "," << rgbMatrix[i][j][2] << "]";
            if (j < cols - 1)
                cout << ",";
        }
        cout << "]";
        if (i < rows - 1)
            cout << ",";
    }
    cout << "]" << endl;

    return 0;
}
