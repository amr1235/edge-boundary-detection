#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "cvector.hpp"
#include "Image.hpp"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>

int count_edges(cv::Mat img,cv::Mat test);
void draw_circle(cv::Mat img,int x,int y, int r);
#endif