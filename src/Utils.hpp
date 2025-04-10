#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <tuple>
#include <functional>
#include <cmath>
#include <memory>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

string convertWindowsToWSLPath(const string &windowsPath);
tuple<int, double, int> validateInputConstraints(
    const string &inputImagePath,
    const vector<vector<vector<int>>> &rgbMatrix,
    int width,
    int height);
long long getFileSize(const string &filePath);
tuple<int, double, int, string> findOptimalSettingsBinarySearch(
    double targetCompression,
    const string &inputImagePath,
    const vector<vector<vector<int>>> &rgbMatrix,
    int width,
    int height);
string askValidPath(const string &text);

#endif