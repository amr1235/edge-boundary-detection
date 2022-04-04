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
    // hough circle
    Image image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\edge-boundary-detection\\images\\circs.png");
    Image fin;
    image.display("original image");
    fin.mat = hough_circle(image.mat, 20);

    fin.display("final circle detection recieved");

    // active contour
    Image original_image = Image("H:\\sbme\\CV\\edge-boundary-detection\\images\\coin.jpeg");
    int points = 80;
    cv::Size sz = original_image.mat.size();
    int x_cooridinates[80];
    int y_cooridinates[80];
    circle_contour(Point(sz.width / 2, sz.height / 2), 150, points, x_cooridinates, y_cooridinates);
    //greedy_contour(original_image, 100, 2, 0.9, 20, x_cooridinates, y_cooridinates, points, 5);
    greedy_contour(original_image, 50 ,2.5, 0.9, 20,x_cooridinates, y_cooridinates, points, 11,true);
    // calculate area of the contour
    double perimeter = contour_perimeter(x_cooridinates, y_cooridinates, points);
    double area = contour_area(points, x_cooridinates, y_cooridinates);
    std::cout << "[contour perimeter : " << perimeter << " ]" << std::endl;
    std::cout << "[contour area : " << area << " ]" << std::endl;
    return 0;
}