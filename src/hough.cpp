#include "hough.hpp"
#include <iostream>

// initialize accumulator
// for each edge pixel:
//      for each theta:
//          r = pixel.x * cos(theta) + pixel.y* sin(theta)
//          acc[r][theta] += 1

// threshold accumulator

void hough_lines(cv::Mat &src, std::vector<cv::Vec2f> &lines, double r, double theta, unsigned int threshold)
{
    if (theta > 180 || theta < 0)
        throw "theta must be between 0 and 180";
    if (r > src.rows * src.cols)
        throw "r must be smaller than or equal image size";

    double mag = sqrt(src.rows * src.rows + src.cols * src.cols);
    std::vector<std::vector<int>> acc(2 * mag, std::vector<int>(180/theta, 0));
    
    for (size_t row = 0; row < src.rows; row++)
    {
        for (size_t col = 0; col < src.cols; col++)
        {
            if (src.at<uchar>(row, col))
            {
                for (size_t i = 0; i < 180; i += theta)
                {
                    r = round(col * cos(i - 90) + row * sin(i - 90)) + mag;
                    // std::cout << i << "\n";
                    acc[r][i]++;
                }
            }
        }
    }

    for (size_t i = 0; i < acc.size(); i++)
    {
        for (size_t j = 0; j < acc[0].size(); j++)
        {
            if (acc[i][j] >= threshold)
                lines.push_back(cv::Vec2f(i, j));
        }
    }
}

void superimpose(cv::Mat &src, std::vector<cv::Vec3f> &circles, cv::Scalar centers_color, cv::Scalar circles_color)
{
    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle(src, center, 1, centers_color, 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, circles_color, 3, cv::LINE_AA);
    }
}

void superimpose(cv::Mat &src, std::vector<cv::Vec2f> &lines, cv::Scalar color)
{
    for (size_t i = 0; i < lines.size(); i++)
    {
        float r = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * r, y0 = b * r;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        cv::line(src, pt1, pt2, color, 3, cv::LINE_AA);
    }
}