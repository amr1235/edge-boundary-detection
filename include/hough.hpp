#ifndef __HOUGH_HPP__
#define __HOUGH_HPP__

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

void hough_lines(cv::Mat &src, std::vector<cv::Vec2f> &lines, double rho, double theta, unsigned int threshold);
void superimpose(cv::Mat &src, std::vector<cv::Vec3f> &circles, cv::Scalar centers_color, cv::Scalar circles_color);
void superimpose(cv::Mat &src, std::vector<cv::Vec2f> &lines, cv::Scalar color);

#endif