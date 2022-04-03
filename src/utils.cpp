#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "histogram.hpp"
#include "utils.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

unsigned char normalization(int oldmin, int oldmax, int newmin, int newmax, unsigned char value)
{
  int oldrange = oldmax - oldmin;
  int newrange = newmax - newmin;
  return (newrange * (value - oldmin) / oldrange) + newmin;
}

cvector<cvector<double>> gaussian_kernel(int size, double std)
{
  double gaussian_value;
  double normalization_sum = 0;
  cvector<cvector<double>> filter;
  for (int row = -size / 2; row <= size / 2; row++)
  {
    cvector<double> row_vector;
    for (int col = -size / 2; col <= size / 2; col++)
    {
      gaussian_value = (1 / (2 * (22 / 7) * pow(std, 2))) * exp(-(pow(row, 2) + pow(col, 2)) / (2 * pow(std, 2)));
      normalization_sum += gaussian_value;
      row_vector.push_back(gaussian_value);
    }
    filter.push_back(row_vector);
  }

  return filter;
}
void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;

  // h, s, v = hue, saturation, value
  double cmax = max(r, max(g, b)); // maximum of r, g, b
  double cmin = min(r, min(g, b)); // minimum of r, g, b
  double diff = cmax - cmin;       // diff of cmax and cmin.

  // if cmax and cmax are equal then h = 0
  if (cmax == cmin)
    *h = 0;

  // if cmax equal r then compute h
  else if (cmax == r)
    *h = (int)(60 * ((g - b) / diff) + 360) % 360;

  // if cmax equal g then compute h
  else if (cmax == g)
    *h = (int)(60 * ((b - r) / diff) + 120) % 360;

  // if cmax equal b then compute h
  else if (cmax == b)
    *h = (int)(60 * ((r - g) / diff) + 240) % 360;

  // if cmax equal zero
  if (cmax == 0)
    *s = 0;
  else
    *s = (diff / cmax) * 100;

  // compute v
  *v = cmax * 100;
}

void HSVtoRGB(float *fR, float *fG, float *fB, float fH, float fS, float fV)
{
  double fC = fV * fS; // Chroma
  double fHPrime = fmod(fH / 60.0, 6);
  double fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  double fM = fV - fC;

  if (0 <= fHPrime && fHPrime < 1)
  {
    *fR = fC;
    *fG = fX;
    *fB = 0;
  }
  else if (1 <= fHPrime && fHPrime < 2)
  {
    *fR = fX;
    *fG = fC;
    *fB = 0;
  }
  else if (2 <= fHPrime && fHPrime < 3)
  {
    *fR = 0;
    *fG = fC;
    *fB = fX;
  }
  else if (3 <= fHPrime && fHPrime < 4)
  {
    *fR = 0;
    *fG = fX;
    *fB = fC;
  }
  else if (4 <= fHPrime && fHPrime < 5)
  {
    *fR = fX;
    *fG = 0;
    *fB = fC;
  }
  else if (5 <= fHPrime && fHPrime < 6)
  {
    *fR = fC;
    *fG = 0;
    *fB = fX;
  }
  else
  {
    *fR = 0;
    *fG = 0;
    *fB = 0;
  }

  *fR += fM;
  *fG += fM;
  *fB += fM;
}

void fftshift(const Mat &input_img, Mat &output_img)
{
  output_img = input_img.clone();
  int cx = output_img.cols / 2;
  int cy = output_img.rows / 2;
  Mat q1(output_img, Rect(0, 0, cx, cy));
  Mat q2(output_img, Rect(cx, 0, cx, cy));
  Mat q3(output_img, Rect(0, cy, cx, cy));
  Mat q4(output_img, Rect(cx, cy, cx, cy));

  Mat temp;
  q1.copyTo(temp);
  q4.copyTo(q1);
  temp.copyTo(q4);
  q2.copyTo(temp);
  q3.copyTo(q2);
  temp.copyTo(q3);
}

void DFT(Mat &scr, Mat &dst)
{
  // define mat consists of two mat, one for real values and the other for complex values
  Mat planes[] = {scr, Mat::zeros(scr.size(), CV_32F)};
  Mat complexImg;
  merge(planes, 2, complexImg);

  dft(complexImg, complexImg);
  dst = complexImg;
}

void filtering(Mat &scr, Mat &dst, Mat &H)
{
  fftshift(H, H);
  Mat planesH[] = {Mat_<float>(H.clone()), Mat_<float>(H.clone())};

  Mat planes_dft[] = {scr, Mat::zeros(scr.size(), CV_32F)};
  split(scr, planes_dft);

  Mat planes_out[] = {Mat::zeros(scr.size(), CV_32F), Mat::zeros(scr.size(), CV_32F)};
  planes_out[0] = planesH[0].mul(planes_dft[0]);
  planes_out[1] = planesH[1].mul(planes_dft[1]);

  merge(planes_out, 2, dst);
}

Mat construct_H(Mat &scr, float D0, p_types pass_type)
{
  Mat H(scr.size(), CV_32F, Scalar(1));
  float D = 0;
  for (int u = 0; u < H.rows; u++)
  {
    for (int v = 0; v < H.cols; v++)
    {
      D = sqrt((u - scr.rows / 2) * (u - scr.rows / 2) + (v - scr.cols / 2) * (v - scr.cols / 2));
      if (pass_type == LOW_PASS_FILTER)
      {
        if (D > D0)
        {
          H.at<float>(u, v) = 0;
        }
      }
      else
      {
        if (D < D0)
        {
          H.at<float>(u, v) = 0;
        }
      }
    }
  }
  return H;
}

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