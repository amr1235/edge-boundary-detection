#include "histogram.hpp"
#include "Image.hpp"
using namespace cv;
using namespace std;
using namespace img;

int grayscale_Histogram[256]={0} , Red_Histogram[256]={0},Green_Histogram[256]={0},Blue_Histogram[256]={0};
int gray_Cumulative[256]={0} , Red_Cumulative[256]={0},Green_Cumulative[256]={0},Blue_Cumulative[256]={0};

void calculate_Histogram(Image img, string image_type) 
{
    Mat image = img.mat,clone;
    vector<int> Red_Values , Green_Values , Blue_Values , Grayscale_Values ;
if (image_type == "grayscale")
{
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
            {Grayscale_Values.push_back(image.at<Vec3b>(i, j)[0]); }
    }
    for (int i = 0; i < Grayscale_Values.size(); i++)
        {grayscale_Histogram[Grayscale_Values[i]]++; }
}  
else if (image_type == "color")
{     
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            Red_Values.push_back(image.at<Vec3b>(i, j)[2]);
            Green_Values.push_back(image.at<Vec3b>(i, j)[1]);
            Blue_Values.push_back(image.at<Vec3b>(i, j)[0]);
         }       
    }  
    for (int i = 0; i < Red_Values.size(); i++)
    {
        Red_Histogram[Red_Values[i]]++;  
        Green_Histogram[Green_Values[i]]++;
        Blue_Histogram[Blue_Values[i]]++; 
    }   
}
}

void Plot_Histogram(Image img,string image_type)
{
calculate_Histogram ( img , image_type );
if (image_type == "grayscale")
{
    plot(grayscale_Histogram,"grayscale_Histogram",Scalar(0, 0, 0)); 
}  
else if (image_type == "color")
{     
    plot(Red_Histogram,"Red_Histogram",Scalar(0, 0, 255)); 
    plot(Green_Histogram,"Green_Histogram",Scalar(0, 255, 0)); 
    plot(Blue_Histogram,"Blue_Histogram",Scalar(255, 0, 0));     
}
}

void plot_Distribution_curve(Image img,string image_type)
{
calculate_Histogram ( img , image_type );
if (image_type == "grayscale")
{
    plot(grayscale_Histogram,"Distribution_Curve",Scalar(0, 0, 0)); 
}  
else if (image_type == "color")
{     
    plot(Red_Histogram,"Distribution_Curve",Scalar(0, 0, 255)); 
    plot(Green_Histogram,"Distribution_Curve",Scalar(0, 255, 0)); 
    plot(Blue_Histogram,"Distribution_Curve",Scalar(255, 0, 0));     
}
}


void calculate_Cumulative(Image img,string image_type)
{
    calculate_Histogram(img , image_type );
    if (image_type == "grayscale")
    {
        gray_Cumulative[0]=grayscale_Histogram[0];
        for (int i = 1; i < 256; ++i)
        {
            gray_Cumulative[i] = gray_Cumulative[i - 1] + grayscale_Histogram[i];  
        } 
    }  
    else if (image_type == "color")
    {
        Red_Cumulative[0]=Red_Histogram[0];
        Green_Cumulative[0]=Green_Histogram[0];
        Blue_Cumulative[0]=Blue_Histogram[0];
        for (int i = 1; i < 256; ++i)
        {
            Red_Cumulative[i] = Red_Cumulative[i - 1] + Red_Histogram[i]; 
            Green_Cumulative[i] = Green_Cumulative[i - 1] + Green_Histogram[i]; 
            Blue_Cumulative[i] = Blue_Cumulative[i - 1] + Blue_Histogram[i];  
        }   
    }
}

void Plot_Cumulative(Image img,string image_type)
{
calculate_Cumulative ( img , image_type );
if (image_type == "grayscale")
{
    plot(gray_Cumulative,"Gray_Cumulative",Scalar(0, 0, 0)); 
}  
else if (image_type == "color")
{     
    plot(Red_Cumulative,"Red_Cumulative",Scalar(0, 0, 255)); 
    plot(Green_Cumulative,"Green_Cumulative",Scalar(0, 255, 0)); 
    plot(Blue_Cumulative,"Blue_Cumulative",Scalar(255, 0, 0));   
}
}

void plot(int array[],string window_name ,Scalar color)
{
    Mat Image(400, 512, CV_8UC3, Scalar(255, 255, 255));
    int max= *max_element( array , array + 256 );
    for (int i = 0; i < 256; i++)
    {
        array[i] = ((double)array[i] / max) * Image.rows;
    }    
    for (int i = 0; i < 256; i++)
    { 
        if(window_name=="Distribution_Curve")
        {
        line(Image, Point(2*i,400-array[i]), Point(2*(i+1),  400- array[i+1]), color, 1.5);
        }
        else
        {
        rectangle(Image, Point(2 *(i),400), Point(2* (i),400- array[i]), color, 1);
        }
    }
    namedWindow(window_name, WINDOW_NORMAL);
    imshow( window_name, Image);
    waitKey(0);
}
