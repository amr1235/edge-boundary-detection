#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <opencv2/opencv.hpp>
#include "cvector.hpp"
#include <string.h>


namespace img
{   
    class Image
    {
    public:
        cv::Mat mat;
        cvector<uchar> pixels;
        Image(){};
        Image(std::string path);
        Image(cv::Mat mat);
        Image(cvector<uchar> pixels, size_t rows, size_t cols, int type);
        void display(std::string title) const;
        void vectorize();
    };
};

#endif