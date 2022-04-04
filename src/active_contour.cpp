#include <opencv2/opencv.hpp>
#include "active_contour.hpp"

void circle_contour(cv::Point center, double radius, int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    std::vector<double> mag = {radius};
    double current_angle = 0;
    double resolution = 360 / (double)numberOfPoints;
    std::cout << resolution << std::endl;
    for (int i = 0; i < numberOfPoints; i++)
    {
        std::vector<double> angle = {current_angle};
        std::vector<double> x = {};
        std::vector<double> y = {};
        cv::polarToCart(mag, angle, x, y, true);
        x_cooridinates[i] = (int)(x[0] + center.x);
        y_cooridinates[i] = (int)(y[0] + center.y);
        current_angle += resolution;
    }
}

void draw_contour(cv::Mat Image, int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    int next;
    for (int i = 0; i < numberOfPoints; i++)

    {
        next = i + 1;
        if (i == numberOfPoints - 1)
        {
            next = 0;
        }
        line(Image, cv::Point(x_cooridinates[i], y_cooridinates[i]), cv::Point(x_cooridinates[next], y_cooridinates[next]), cv::Scalar(255, 0, 0), 2);
    }
}

double contour_area(int numberOfPoints, int *x_cooridinates, int *y_cooridinates)
{
    double area = 0.0;
    // Calculate value of shoelace formula => 1/2 [ (x1y2 + x2y3 + … + xn-1yn + xny1) – (x2y1 + x3y2 + … + xnyn-1 + x1yn) ]
    int j = numberOfPoints - 1;
    for (int i = 0; i < numberOfPoints; i++)
    {
        area += (x_cooridinates[j] + x_cooridinates[i]) * (y_cooridinates[j] - y_cooridinates[i]);
        j = i; // j is previous vertex to i
    }
    return std::abs(area / 2.0);
}

double contour_perimeter(int *x_points, int *y_points, int points_n)
{
    double distance_sum = 0;
    for (int i = 0; i < points_n; i++)
    {
        int next_point = i + 1;
        if (i == points_n - 1)
        {
            next_point = 0;
        }

        double distance = points_distance(x_points[i], y_points[i], x_points[next_point], y_points[next_point]);
        distance_sum += distance;
    }
    return distance_sum;
}

double internal_energy(int *x_points, int *y_points, int points_n, double alpha, double beta)
{
    double curv_sum = 0;
    double second_drev = 0;
    double cont_sum = 0;
    double first_driv = 0;
    double avg_dist = contour_perimeter(x_points, y_points, points_n)/points_n;
    for (int i = 0; i < points_n; i++)
    {
        int next_point = i + 1;
        int prev_point = i - 1;
        if (i == points_n - 1)
        {
            next_point = 0;
        }
        else if (i == 0)
        {
            prev_point = points_n - 1;
        }
        first_driv = points_distance(x_points[i], y_points[i], x_points[next_point], y_points[next_point]);
        cont_sum += pow((first_driv - avg_dist), 2);
        second_drev = pow((x_points[next_point] - 2 * x_points[i] + x_points[prev_point]), 2) + pow((y_points[next_point] - 2 * y_points[i] + y_points[prev_point]), 2);
        curv_sum += second_drev;
    }
    double energy = alpha * cont_sum + beta * curv_sum;
    return energy;
}

img::Image external_energy(img::Image source)
{
    img::Image filtered_Gaussian = img::noise_filter(source, 3, "Gaussian");
    img::Image gray = img::convert(filtered_Gaussian, "bgr", "gray");
    // cv::Sobel(gray.mat,gray.mat,CV_8UC1,1,1);
    return img::Image(img::scale(img::sobel(gray, 1, 1)), gray.mat.rows - 2, gray.mat.rows - 2, CV_8UC1);
    // return gray;
}

void greedy_contour(img::Image source, int iterations ,int alpha, int beta, int gamma,int* x_points, int* y_points, int points_n, int window_size, bool plot){
    img::Image sobel_energy = external_energy(source);
    cvector<cvector<int>> window = window_neighbours(window_size);
    int current_x[180];
    int current_y[180];
    std::copy(x_points,x_points + points_n,current_x);
    std::copy(y_points,y_points + points_n,current_y);
    cv::Mat plot_img;
    int point_energy;
    int min_energy;
    int min_index;
    int movements = 0;
    int iteration = 0;
    bool loob = true;
    int threshold = 5;
    int neighbours_size = pow(window_size,2);
    while(loob){
        for(int i=0;i<points_n;i++){
            for(int j=0;j<neighbours_size;j++){
                current_x[i] = x_points[i] + window[j][0];
                current_y[i] = y_points[i] + window[j][1];
                if(current_x[i] < sobel_energy.mat.rows   && current_x[i] > 0 && current_y[i] > 0 && current_y[i] < sobel_energy.mat.cols){
                    // point_energy = sobel_energy.mat.at<uchar>(current_x[i],current_y[i]) * -1 * gamma;
                    point_energy = sobel_energy.mat.at<uchar>(current_x[i],current_y[i]) * -1 * gamma + internal_energy(current_x, current_y,points_n, alpha, beta);
                    // std::cout<< point_energy<<"$$";
                    if(j==0){
                        min_energy = point_energy;
                        min_index = j;
                    }
                    else{
                        if(point_energy < min_energy){
                            min_energy = point_energy;
                            min_index = j;
                        }
                    }
                }
            }
            x_points[i] = x_points[i] + window[min_index][0];
            y_points[i] = y_points[i] + window[min_index][1];
            if(window[min_index][0] != 0 || window[min_index][1] != 0){
                movements++;
            }
        }
        iteration++;
        if(plot == true){
            plot_img = source.mat.clone();
            draw_contour(plot_img, points_n, x_points, y_points);
            cv::imshow("Active Contour", plot_img);
            cv::waitKey(10);
            plot_img.release();
        }
        // std::cout<<iteration;

        if(iteration > iterations || movements < threshold){
            loob = false;
            cv::waitKey(0);
        }
    }
}

cvector<cvector<int>> window_neighbours(int size)
{
    cvector<cvector<int>> window;
    cvector<int> point;

    for (int i = -size / 2; i <= size / 2; i++)
    {

        for (int j = -size / 2; j <= size / 2; j++)
        {
            point = {i, j};
            window.push_back(point);
        }
    }
    return window;
}