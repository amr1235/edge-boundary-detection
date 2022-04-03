#include <opencv2/opencv.hpp>

void circle_contour(cv::Point center, double radius, int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    std::vector<double> mag = {radius};
    double current_angle = 0;
    double resolution = 360 / (double)numberOfPoints;
    std::cout << resolution << std::endl;
    for (int i = 0; i < numberOfPoints; i++)
    {
        std::vector<double> angle = {current_angle};
        std::vector<double> x = {};
        std::vector<double> y = {};
        cv::polarToCart(mag, angle, x, y, true);
        x_cooridinates[i] = (int)(x[0] + center.x);
        y_cooridinates[i] = (int)(y[0] + center.y);
        current_angle += resolution;
    }
}

void draw_contour(cv::Mat Image, int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    for (int i = 0; i < numberOfPoints; i++)
    {
        line(Image, cv::Point(x_cooridinates[i], y_cooridinates[i]), cv::Point(x_cooridinates[i], y_cooridinates[i]), cv::Scalar(255, 0, 0), 2);
    }
}

double contour_area(int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    double area = 0.0;
    // Calculate value of shoelace formula => 1/2 [ (x1y2 + x2y3 + … + xn-1yn + xny1) – (x2y1 + x3y2 + … + xnyn-1 + x1yn) ]
    int j = numberOfPoints - 1;
    for (int i = 0; i < numberOfPoints; i++)
    {
        area += (x_cooridinates[j] + x_cooridinates[i]) * (y_cooridinates[j] - y_cooridinates[i]);
        j = i; // j is previous vertex to i
    }
    return std::abs(area / 2.0);
}