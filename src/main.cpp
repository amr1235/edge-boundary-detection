#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "utils.hpp"
#include "hough_circle.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "active_contour.hpp"

using namespace cv;
using namespace img;

// cv.
int main(int argc, char **argv)
{
    Image image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\edge-boundary-detection\\images\\circs.png");
    Image fin;
    image.display("original image");
    fin.mat = hough_circle(image.mat, 20);

    fin.display("final circle detection recieved");

    // map from cartisian to
    Mat img = cv::imread("D:\\sbme\\fourth year\\second term\\CV\\projects\\edge-boundary-detection\\images\\coin.jpg");
    // draw a circle => (x - a)^2 + (y - b)^2 = r^2 || x = r * cos(s) , y = r * sin(s)
    int x_cooridinates[80];
    int y_cooridinates[80];
    Mat Image(400, 512, CV_8UC3, Scalar(255, 255, 255));
    cv::Size sz = Image.size();
    circle_contour(Point(sz.width / 2, sz.height / 2), 100.0, 80, x_cooridinates, y_cooridinates);
    draw_contour(Image, 80, x_cooridinates, y_cooridinates);
    std::cout << contour_area(80, x_cooridinates, y_cooridinates);
    cv::imshow("test", Image);
    cv::waitKey();
    return 0;
}