#ifndef __HOUGH__LINE__HPP__
#define __HOUGH__LINE__HPP__

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "image.hpp"

void hough_lines(img::Image &src, std::vector<cv::Vec2f> &lines, unsigned int threshold);
void superimpose(img::Image &src, std::vector<cv::Vec2f> &lines, cv::Scalar color);

#endif