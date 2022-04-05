#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "utils.hpp"
#include "hough_circle.hpp"
#include "hough_line.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "active_contour.hpp"

using namespace cv;
using namespace img;

static void test_hough_lines()
{
    Image img("..\\..\\images\\lines.png");
    std::vector<Vec2f> lines;
    Image cny;
    Canny(img.mat, cny.mat, 100, 150);
    hough_lines(cny, lines, 50);
    superimpose(img, lines, Scalar(0, 0, 255));
    img.display("test");
}

static void test_hough_circles()
{
    Image image = Image("..\\..\\images\\circles.png");
    Image fin;
    image.display("original image");
    fin.mat = hough_circle(image.mat, 20);
    fin.display("final circle detection recieved");
}

static void test_contours()
{
    // active contour
    Image original_image = Image("..\\..\\images\\coin.jpeg");
    int points = 80;
    cv::Size sz = original_image.mat.size();
    int x_cooridinates[80];
    int y_cooridinates[80];
    circle_contour(Point(sz.width / 2, sz.height / 2), 150, points, x_cooridinates, y_cooridinates);
    // greedy_contour(original_image, 100, 2, 0.9, 20, x_cooridinates, y_cooridinates, points, 5);
    greedy_contour(original_image, 50, 2.5, 0.9, 20, x_cooridinates, y_cooridinates, points, 11, true);
    // calculate area of the contour
    double perimeter = contour_perimeter(x_cooridinates, y_cooridinates, points);
    double area = contour_area(points, x_cooridinates, y_cooridinates);
    std::cout << "[contour perimeter : " << perimeter << " ]" << std::endl;
    std::cout << "[contour area : " << area << " ]" << std::endl;
}

int main(int argc, char **argv)
{
    test_hough_lines();
    test_hough_circles();
    test_contours();
    return 0;
}