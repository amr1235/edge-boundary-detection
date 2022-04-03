#ifndef __SNAKE__
#define __SNAKE__

#include <opencv2/opencv.hpp>
#include <cmath>
#include "cvector.hpp"
#include "image.hpp"
#include <stdio.h>

void circle_contour(cv::Point center, double radius, int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
void draw_contour(cv::Mat Image, int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
double contour_area(int numberOfPoints, int *x_cooridinates, int *y_cooridinates);
double avg_distance(int *x_points, int *y_points, int points_n);
cvector<cvector<int>> window_neighbours(int size);
void greedy_contour(img::Image source, int iterations, int alpha, int beta, int gamma, int *x_points, int *y_points, int points_n, int window_size);

#endif