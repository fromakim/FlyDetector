#pragma once

#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

constexpr auto THRESHOLD1 = 100;
constexpr auto THRESHOLD2 = 255;

constexpr auto EPSILON1 = 15;
constexpr auto EPSILON2 = 25;

constexpr auto NUMBER_OF_ROI = 7;