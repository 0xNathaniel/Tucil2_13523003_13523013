#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <tuple>

using namespace std;

string convertWindowsToWSLPath(const string &windowsPath);
tuple<double, double, int> validateInputConstraints();
long long getFileSize(const string &filePath);
string askValidPath(const string &text);

#endif