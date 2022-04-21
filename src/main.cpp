#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image.hpp"
#include "cvector.hpp"
#include "utils.hpp"
#include "hough_circle.hpp"
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "active_contour.hpp"

using namespace cv;
using namespace img;

// cv.
int codeList[9] = {5, 6, 7, 4, -1, 0, 3, 2, 1};
int getChainCode(int x1,int y1,int x2,int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    int hashKey = 3 * dy + dx + 4;
    return codeList[hashKey];
}
cvector<cvector<int>> Bresenham2D(int x1,int y1,int x2,int y2){
    cvector<cvector<int>> neighbor_points;
    cvector<int> point;
    int xs,ys;
    point.push_back(x1);
    point.push_back(y1);
    neighbor_points.push_back(point);
    point.clear();
    int xdif = x2 - x1;
    int dx = abs(xdif);
    int ydif = y2 - y1;
    int dy = abs(y2 - y1);

    if(xdif > 0){
        xs = 1;
    }
    else{
        xs = -1;
    }
    if (ydif > 0){
        ys = 1;
    }
    else{
        ys = -1;
    }

    if (dx > dy){
 
        //Driving axis is the X-axis
        int p = 2 * dy - dx;
        while (x1 != x2){
            x1 += xs;
            if (p >= 0){
                y1 += ys;
                p -= 2 * dx;
            }
            p += 2 * dy;
            point.push_back(x1);
            point.push_back(y1);
            neighbor_points.push_back(point);
            point.clear();     
        }
    }
    else{
        int p = 2 * dx-dy;
        while(y1 != y2){
            y1 += ys;
            if (p >= 0){
                x1 += xs;
                p -= 2 * dy;
            }
            p += 2 * dx;
            point.push_back(x1);
            point.push_back(y1);
            neighbor_points.push_back(point);
            point.clear();     
        }
    }
    return neighbor_points;
}
cvector<int> generateChainCode(cvector<cvector<int>> neighbor_points){
    cvector<int> chainCode;
    for (int i = 0; i< (neighbor_points.size() - 1) ; i++){
        cvector<int> a = neighbor_points[i];
        cvector<int> b = neighbor_points[i + 1];
        chainCode.push_back(getChainCode(a[0], a[1], b[0], b[1]));
    }
    return chainCode;
}
cvector<int> points_mean(int* points , int size){
    int z = 0;
    cvector<int> mean;
    int sum = 0;
    for(int i =0 ; i < size; i++){
            if(z < 5){
                sum += points[i];
                z++;
            }
            else{
                z = 0;
                mean.push_back(sum/5);
                sum = points[i];
                z++;
            }
            if(i == (size-1)){
                mean.push_back(sum/5);
            }
        }
    return mean;  
}
void chain_code(int* x_cooridinates, int* y_cooridinates, int size, bool mean){
    cvector<int> chainCode;
    if(mean){
        cvector<int> x_mean = points_mean(x_cooridinates,size);
        cvector<int> y_mean = points_mean(y_cooridinates,size);
        for(int i=0;i < (x_mean.size()-1);i++){
            cvector<cvector<int>> ListOfPoints = Bresenham2D(x_mean[i], y_mean[i], x_mean[i+1], y_mean[i+1] );
            chainCode = generateChainCode(ListOfPoints);
            for(int j=0; j<chainCode.size();j++){
                std::cout<<chainCode[j]<<" ";
            }
        }
    }
    else{
        for(int i=0;i < size-1;i++){
            cvector<cvector<int>> ListOfPoints = Bresenham2D(x_cooridinates[i], y_cooridinates[i], x_cooridinates[i+1], y_cooridinates[i+1] );
            chainCode = generateChainCode(ListOfPoints);
            for(int j=0; j<chainCode.size();j++){
                std::cout<<chainCode[j]<<" ";
            }
        } 
    }
}

int main(int argc, char **argv)
{
    // hough circle
    // Image image = Image("D:\\sbme\\fourth year\\second term\\CV\\projects\\edge-boundary-detection\\images\\circs.png");
    // Image fin;
    // image.display("original image");
    // fin.mat = hough_circle(image.mat, 20);

    // fin.display("final circle detection recieved");
    // std::cout<<chainCode;
    Image original_image = Image("H:\\sbme\\CV\\edge-boundary-detection\\images\\coin.jpeg");
    int points = 80;
    cv::Size sz = original_image.mat.size();
    int x_cooridinates[80];
    int y_cooridinates[80];
    circle_contour(Point(sz.width / 2, sz.height / 2), 150, points, x_cooridinates, y_cooridinates);
    //greedy_contour(original_image, 100, 2, 0.9, 20, x_cooridinates, y_cooridinates, points, 5);
    greedy_contour(original_image, 50 ,2.5, 0.9, 20,x_cooridinates, y_cooridinates, points, 11,false);
    chain_code(x_cooridinates,y_cooridinates,points,true);
    // std::cout<<x_cooridinates[0]<<"$"<<x_cooridinates[1]<<"$"<<x_cooridinates[2]<<"$"<<x_cooridinates[3]<<"$"<<x_cooridinates[4]<<"\n";
    // std::cout<<x_cooridinates[75]<<"$"<<x_cooridinates[76]<<"$"<<x_cooridinates[77]<<"$"<<x_cooridinates[78]<<"$"<<x_cooridinates[79]<<"\n";
    // // calculate area of the contour
    // double perimeter = contour_perimeter(x_cooridinates, y_cooridinates, points);
    // double area = contour_area(points, x_cooridinates, y_cooridinates);
    // std::cout << "[contour perimeter : " << perimeter << " ]" << std::endl;
    // std::cout << "[contour area : " << area << " ]" << std::endl;
    return 0;
}