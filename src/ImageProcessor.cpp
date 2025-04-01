#include "ImageProcessor.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;

string ImageProcessor::inputImagePath() {
    string imagePath;

    // Meminta input image path hingga pengguna exit atau memberikan alamat gambar yang valid
    while (true) {
        cout << "Masukkan alamat absolut gambar: ";
        getline(cin, imagePath);

        if (imagePath == "exit") {
            return "exit";
        }

        Mat image = imread(imagePath, IMREAD_COLOR);
        if (image.empty()) {
            cout << "Gambar tidak ditemukan!" << endl;
        }
        else {
            return imagePath;
        }
    }
}

vector<vector<vector<int>>> ImageProcessor::loadImage(const string& imagePath) {
    // Mulai proses load image
    Mat image = imread(imagePath, IMREAD_COLOR);
    int rows = image.rows;
    int cols = image.cols;
    vector<vector<vector<int>>> rgbMatrix(rows, vector<vector<int>>(cols, vector<int>(3)));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            rgbMatrix[i][j][0] = pixel[2];
            rgbMatrix[i][j][1] = pixel[1];
            rgbMatrix[i][j][2] = pixel[0];
        }
    }

    return rgbMatrix;
}
