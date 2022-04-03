#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "cvector.hpp"
#include "Image.hpp"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>

cvector<cvector<double>> gaussian_kernel(int size, double std);
int count_edges(cv::Mat img,cv::Mat test);
void draw_circle(cv::Mat img,int x,int y, int r);
void RGBtoHSV(float fR, float fG, float fB, float *fH, float *fS, float *fV);
void HSVtoRGB(float *fR, float *fG, float *fB, float fH, float fS, float fV);
#endif