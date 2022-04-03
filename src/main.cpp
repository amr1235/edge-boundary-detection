#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "histogram.hpp"
#include "utils.hpp"
#include "task2.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>
using namespace cv;
using namespace img;

// cv.
int main(int argc, char **argv)
{
  Image image = Image("images\\circs.png");
  Image fin;
  image.display("original image");
  fin.mat = hough_circle(image.mat, 20);

  fin.display("final circle detection recieved");

  

  return 0;
}