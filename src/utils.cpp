#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "utils.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int count_edges(cv::Mat img,cv::Mat test)
{
  int count =0;
    for (int i = 0; i < img.rows; i++)
      {
        for (int j = 0; j < img.cols; j++)
          {
            test.at<uchar>(i, j) =0;
            if((int)img.at<uchar>(i, j)!=0)
            count = count +1;
          }
      }
return count;
}

void draw_circle(cv::Mat img,int x,int y, int r)
{
  int a,b;
    for (int theta = 0; theta < 360; theta++)
  {
    a = (int)(x-r - r*cos(theta*3.14159/180));
    b = (int)(y - r*sin(theta*3.14159/180));
    if ( a>0 && a<img.cols && b>0 && b<img.rows)
    img.at<uchar>(a, b) =255;
  }
}