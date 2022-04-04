#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
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
    Image original_image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\cv-project-1\\images\\coin.jpeg");
    Image initial_image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\cv-project-1\\images\\coin.jpeg");
    Image final_image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\cv-project-1\\images\\coin.jpeg");
    original_image.display("Original Image");
    int points = 80;
    cv::Size sz = original_image.mat.size();
    int x_cooridinates[80];
    int y_cooridinates[80];
    circle_contour(Point(sz.width / 2, sz.height / 2), 150, points, x_cooridinates, y_cooridinates);
    draw_contour(initial_image.mat, points, x_cooridinates, y_cooridinates);
    initial_image.display("initial contour");
    // coin
    // 10, 0.4, 20
    greedy_contour(original_image, 100, 2, 0.9, 20, x_cooridinates, y_cooridinates, points, 5);
    draw_contour(final_image.mat, points, x_cooridinates, y_cooridinates);
    final_image.display("active contour");
    // calculate area of the contour
    double area = contour_area(points, x_cooridinates, y_cooridinates);
    std::cout << "[contour area : " << area << " ]" << std::endl;

    cv::waitKey();
    return 0;
}