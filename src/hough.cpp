#include "hough.hpp"
#include <iostream>

void hough_lines(cv::Mat &src, std::vector<cv::Vec2f> &lines, double r, double theta, int threshold)
{
    std::vector<cv::Vec3f> acc = {
        cv::Vec3f(1,0,5),cv::Vec3f(1,0,0),cv::Vec3f(1,0,2),cv::Vec3f(1,0,4)
    };
    for (size_t i = 0; i < acc.size(); i++)
    {
        if(acc[i].operator[](2) >= threshold)
            lines.push_back(cv::Vec2f(acc[i].operator[](0),acc[i].operator[](1)));
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