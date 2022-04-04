#include <stdio.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "hough.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat im = imread("../../images/lines.png");

    Mat cny,sup_imposed;
    Canny(im, cny,50,200);

    // convert to bgr to display lines in rgb
    cvtColor(cny, sup_imposed, COLOR_GRAY2BGR);

    vector<Vec2f> lines; // will hold the results of the detection
    
    // cny may be modified
    // probably need to copy incase of using cny in other functions
    hough_lines(cny, lines, 1, 1, 20); // runs the actual detection
    // HoughLines(cny, lines, 1, CV_PI / 180, 3);
    superimpose(sup_imposed, lines, Scalar(0, 0, 255));
    cout << lines.size() << "\n";
    imshow("final", sup_imposed);

    waitKey(0);

    return 0;
}