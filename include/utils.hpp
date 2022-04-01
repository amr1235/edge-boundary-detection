#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "cvector.hpp"
#include "Image.hpp"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <opencv2/opencv.hpp>

unsigned char normalization(int oldmin, int oldmax, int newmin, int newmax, unsigned char value);
cvector<cvector<double>> gaussian_kernel(int size, double std);
void RGBtoHSV(float fR, float fG, float fB, float *fH, float *fS, float *fV);
void HSVtoRGB(float *fR, float *fG, float *fB, float fH, float fS, float fV);
void fftshift(const cv::Mat &input_img, cv::Mat &output_img);
void DFT(cv::Mat &scr, cv::Mat &dst);
void filtering(cv::Mat &scr, cv::Mat &dst, cv::Mat &H);
cv::Mat construct_H(cv::Mat &scr, float D0,img::p_types pass_type);

#endif