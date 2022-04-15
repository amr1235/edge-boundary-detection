#include "hough_line.hpp"
#include "image.hpp"

void hough_lines(img::Image &src, std::vector<cv::Vec2f> &lines, unsigned int threshold)
{
    double r = 0;
    double diagonal = ceil(sqrt(src.mat.rows * src.mat.rows + src.mat.cols * src.mat.cols));
    std::vector<std::vector<int>> acc(2 * diagonal, std::vector<int>(180, 0));
    
    for (size_t row = 0; row < src.mat.rows; row++)
    {
        for (size_t col = 0; col < src.mat.cols; col++)
        {
            if (src.mat.at<uchar>(row, col))
            {
                for (double angle = 0; angle < 180; angle ++)
                {
                    r = round(col * cos(angle - 90) + row * sin(angle - 90)) + diagonal;
                    acc[r][angle]++;
                }
            }
        }
    }

    for (int i = 0; i < acc.size(); i++)
    {
        for (int j = 0; j < acc[0].size(); j++)
        {
            if (acc[i][j] >= threshold)
                lines.push_back(cv::Vec2f(i-diagonal, j-90));
        }
    }
}

void superimpose(img::Image &src, std::vector<cv::Vec2f> &lines, cv::Scalar color)
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
        cv::line(src.mat, pt1, pt2, color, 3, cv::LINE_AA);
    }
}