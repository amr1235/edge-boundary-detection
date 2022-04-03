#ifndef __TASK2_HPP__
#define __TASK2_HPP__

#include "cvector.hpp"
#include "Image.hpp"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>

cv::Mat hough_circle(cv::Mat _img, uint threshold);
#endif