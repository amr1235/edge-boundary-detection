#include <opencv2/opencv.hpp>
void circle_contour(cv::Point center,double radius, int numberOfPoints,int * x_cooridinates,int * y_cooridinates);
void draw_contour(cv::Mat Image,int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
double contour_area(int numberOfPoints, int * x_cooridinates, int * y_cooridinates);