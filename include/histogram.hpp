#ifndef __HISTOGRAM_HPP__
#define __HISTOGRAM_HPP__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include <iostream>
using namespace std ;
using namespace cv;
using namespace img;

void plot(int array[],string window_name ,Scalar color);
void calculate_Histogram(Image img, string image_type) ;
void Plot_Histogram(Image img,string image_type);
void plot_Distribution_curve(Image img,string image_type);
void calculate_Cumulative(Image img,string image_type);
void Plot_Cumulative(Image img,string image_type);



#endif