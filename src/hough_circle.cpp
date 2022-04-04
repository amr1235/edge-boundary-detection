#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "utils.hpp"
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace img;

cv::Mat hough_circle(cv::Mat _img, uint threshold)
{
  Image img;
  cv::Canny(_img, img.mat, 50, 80);
  
  Image test =img.mat.clone();
    int count = count_edges(img.mat,test.mat);

      //img.display("blCK");
        std::cout<<count<<std::endl;
        std::cout<<img.mat.rows << std::endl<< img.mat.cols<<std::endl;
  int rmax = sqrt((img.mat.rows * img.mat.rows) + (img.mat.rows * img.mat.rows));
   map<int, map<int, map<int, int>> > m;
  
    // For accessing outer map
    map<int, map<int, map<int, int>> >::iterator itr;

  img.display("canny detection");
  vector<int> radius;
  vector<int> x_pt;
  vector<int> y_pt;
  vector<int> rep; 
  int a,b;
  for (int r = 5; r < 11; r++)
  {
      for (int i = 0; i < img.mat.rows; i++)
      {
        for (int j = 0; j < img.mat.cols; j++)
          {
            if((int)img.mat.at<uchar>(i, j)!=0)
            {
               for (int theta = 0; theta < 360; theta = theta +10)
               {
                  a = (int)(j - r*cos(theta*3.14159/180));
                  b = (int)(i - r*sin(theta*3.14159/180));
                  m[r][a][b] ++; 
                  if (m[r][a][b] >threshold && a>0 && a<img.mat.cols && b>0 && b<img.mat.rows)
                  {
                    radius.push_back(r);
                    x_pt.push_back(i);
                    y_pt.push_back(j);
                    rep.push_back(m[r][a][b]);
                  }
               }
            }
          }
      }
  }
  int ieter = radius[0];
for (int c = 0; c < rep.size()-1; c++)
{ 
  if (radius[c+1] != radius[c]  || (c+1) ==(rep.size()-1))
  {
      draw_circle(test.mat,x_pt[c],y_pt[c],radius[c]);
  }
} 

test.display("final image sent");

  return test.mat;
}