#include "cvector.hpp"
#include "image.hpp"
#include "utils.hpp"
#include <opencv2/opencv.hpp>
#include <random>

#define PI 3.14159265

namespace img
{
    Image::Image(std::string path)
    {
        this->mat = cv::imread(path);
        this->vectorize();
    }

    Image::Image(cvector<uchar> pixels, size_t rows, size_t cols, int type)
    {
        this->mat = cv::Mat(rows, cols, type);
        memcpy(this->mat.data, pixels.data(), pixels.size() * sizeof(unsigned char));
        this->pixels = pixels;
    }

    Image::Image(cv::Mat mat)
    {
        this->mat = mat;
        this->vectorize();
    }

    void Image::display(std::string title) const
    {
        cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
        cv::imshow(title, this->mat);
        cv::waitKey(0);
    }

    void Image::vectorize()
    {
        if (this->mat.isContinuous())
        {
            this->pixels.assign((unsigned char *)this->mat.datastart, (unsigned char *)this->mat.dataend);
        }
        else
        {
            for (int i = 0; i < this->mat.rows; ++i)
            {
                this->pixels.insert(this->pixels.end(), this->mat.ptr<unsigned char>(i), this->mat.ptr<unsigned char>(i) + this->mat.cols);
            }
        }
    }

    cvector<double> filter(const Image &img, cvector<cvector<double>> mask)
    {
        int img_rows = img.mat.rows;
        int img_cols = img.mat.cols;
        int mask_rows = mask.size();
        int mask_cols = mask[0].size();

        // convert uchar to int to handle -ve values
        cvector<double> fltr(img.pixels.begin(), img.pixels.end());
        cvector<cvector<double>> mtrx = fltr.to_2d(img_rows, img_cols);
        fltr.clear();

        for (int row = 0; row < (img_rows - mask_rows + 1); row++)
        {
            for (int col = 0; col < (img_cols - mask_cols + 1); col++)
            {
                cvector<cvector<double>> sub = mtrx.range(row, row + mask_rows, col, col + mask_cols);
                double px = 0;
                for (int mask_row = 0; mask_row < mask_rows; mask_row++)
                {
                    px += sub[mask_row].dot(mask[mask_row]);
                }
                fltr.push_back(px);
            }
        }
        return fltr;
    }

    Image normalize(Image &img, int min, int max)
    {
        int oldmin = int(*std::min_element(img.pixels.begin(), img.pixels.end()));
        int oldmax = int(*std::max_element(img.pixels.begin(), img.pixels.end()));
        int oldrange = oldmax - oldmin;
        int newrange = max - min;
        cvector<uchar> array;
        for (int i = 0; i < img.pixels.size(); i++)
        {
            array.push_back(normalization(oldmin, oldmax, min, max, img.pixels[i]));
        }
        return Image(array, img.mat.rows, img.mat.cols, img.mat.type());
    }

    Image convert(const Image &img, std::string from, std::string to)
    {
        Image dest;

        if (from == "bgr" && to == "gray")
        {
            cvector<Image> splt = split(img);
            cv::Mat cp = img.mat.clone();
            cv::Mat grayscaled_image = cv::Mat(cp.rows, cp.cols, CV_8UC1);
            double gray = 0;
            for (int i = 0; i < splt[0].mat.rows; i++)
            {
                for (int j = 0; j < splt[0].mat.cols; j++)

                {
                    gray = (0.299 * cp.at<cv::Vec3b>(i, j)[2]) + (0.587 * cp.at<cv::Vec3b>(i, j)[1]) + (0.114 * cp.at<cv::Vec3b>(i, j)[0]);
                    grayscaled_image.at<uchar>(i, j) = gray;
                }
            }
            dest = Image(grayscaled_image);
            /*
            std::cout<<cp.mat.type()<<std::endl;

           cv::Mat cpy = img.mat.clone();
            cvtColor(img.mat,cpy,cv::COLOR_BGR2GRAY);
                dest  = Image(cpy);
                */
        }
        else if (from == "bgr" && to == "hsv")
        {
            float fH, fS, fV;
            cvector<Image> splt = split(img);
            Image cp = img.mat.clone();
            for (int i = 0; i < splt[0].mat.rows; i++)
            {
                for (int j = 0; j < splt[0].mat.cols; j++)

                {
                    fH = 0;
                    fS = 0;
                    fV = 0;
                    RGBtoHSV((float)(img.mat.at<cv::Vec3b>(i, j)[2]), (float)(img.mat.at<cv::Vec3b>(i, j)[1]), (float)(img.mat.at<cv::Vec3b>(i, j)[0]), &fH, &fS, &fV);

                    cp.mat.at<cv::Vec3b>(i, j)[0] = round(fH / 10);
                    cp.mat.at<cv::Vec3b>(i, j)[1] = fS;
                    cp.mat.at<cv::Vec3b>(i, j)[2] = fV;
                }
            }
            // std::cout<<"img  "<<(int)img.mat.at<cv::Vec3b>(10,10)[2]<<" "<<(int)img.mat.at<cv::Vec3b>(10,10)[1]<<" "<<(int)img.mat.at<cv::Vec3b>(10,10)[0]<<std::endl;
            // std::cout<<"cp  "<<(int)cp.mat.at<cv::Vec3b>(10,10)[0]<<" "<<(int)cp.mat.at<cv::Vec3b>(10,10)[1]<<" "<<(int)cp.mat.at<cv::Vec3b>(10,10)[2]<<std::endl;
            dest = cp;
            // std::cout<<"dest  "<<(int)dest.mat.at<cv::Vec3b>(10,10)[0]<<" "<<(int)dest.mat.at<cv::Vec3b>(10,10)[1]<<" "<<(int)dest.mat.at<cv::Vec3b>(10,10)[2]<<std::endl;
        }

        else if (from == "hsv" && to == "bgr")
        {
            float fR, fG, fB;
            cvector<Image> splt = split(img);
            Image cp = img.mat.clone();
            for (int i = 0; i < splt[0].mat.rows; i++)
            {
                for (int j = 0; j < splt[0].mat.cols; j++)
                {
                    fR = 0;
                    fG = 0;
                    fB = 0;
                    HSVtoRGB(&fR, &fG, &fB, (float)(img.mat.at<cv::Vec3b>(i, j)[0]) * 10, (float)((img.mat.at<cv::Vec3b>(i, j)[1]) / 100.0), (float)((img.mat.at<cv::Vec3b>(i, j)[2]) / 100.0));
                    cp.mat.at<cv::Vec3b>(i, j)[0] = fB * 255;
                    cp.mat.at<cv::Vec3b>(i, j)[1] = fG * 255;
                    cp.mat.at<cv::Vec3b>(i, j)[2] = fR * 255;
                }
            }
            dest = cp;
            //         std::cout<<"img  "<<(int)img.mat.at<cv::Vec3b>(10,10)[0]<<" "<<(float)((img.mat.at<cv::Vec3b>(10,10)[1])/100.0)<<" "<<(float)((img.mat.at<cv::Vec3b>(10,10)[2])/100.0)<<std::endl;
            // std::cout<<"cp  "<<(int)cp.mat.at<cv::Vec3b>(10,10)[2]<<" "<<(int)cp.mat.at<cv::Vec3b>(10,10)[1]<<" "<<(int)cp.mat.at<cv::Vec3b>(10,10)[0]<<std::endl;
            // std::cout<<"dest  "<<(int)dest.mat.at<cv::Vec3b>(10,10)[2]<<" "<<(int)dest.mat.at<cv::Vec3b>(10,10)[1]<<" "<<(int)dest.mat.at<cv::Vec3b>(10,10)[0]<<std::endl;
        }
        else
        {
            throw "Invalid conversion";
        }
        return dest;
    }

    cvector<Image> split(const Image &img)
    {
        cv::Mat channels[3];
        split(img.mat, channels);
        cvector<Image> imgs = {channels[0], channels[1], channels[2]};
        return imgs;
    }

    Image merge(const cvector<Image> &imgs)
    {
        cv::Mat merged;
        cv::Mat cs[] = {imgs[0].mat, imgs[1].mat, imgs[2].mat};
        cv::merge(cs, 3, merged);
        return Image(merged);
    }

    cvector<double> sobel(const Image &img, bool dx, bool dy)
    {
        if (!dx && !dy)
        {
            throw "Must set dx or dy or both";
        }
        cvector<cvector<double>> x = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1},
        };
        cvector<cvector<double>> y = {
            {1, 2, 1},
            {0, 0, 0},
            {-1, -2, -1},
        };

        size_t dim = (img.mat.rows - 3 + 1) * (img.mat.cols - 3 + 1);
        cvector<double> gx(dim, 0);
        cvector<double> gy(dim, 0);
        if (dx)
        {
            gx = filter(img, x);
        }
        if (dy)
        {
            gy = filter(img, y);
        }
        return cvector<double>::mag(gx, gy);
    }

    cvector<double> prewitt(const Image &img, bool dx, bool dy)
    {
        cvector<cvector<double>> x = {
            {1, 0, -1},
            {1, 0, -1},
            {1, 0, -1},
        };
        cvector<cvector<double>> y = {
            {1, 1, 1},
            {0, 0, 0},
            {-1, -1, -1},
        };

        size_t dim = (img.mat.rows - 3 + 1) * (img.mat.cols - 3 + 1);
        cvector<double> gx(dim, 0);
        cvector<double> gy(dim, 0);
        if (dx)
        {
            gx = filter(img, x);
        }
        if (dy)
        {
            gy = filter(img, y);
        }
        return cvector<double>::mag(gx, gy);
    }

    cvector<double> roberts(const Image &img, bool dx, bool dy)
    {
        if (!dx && !dy)
        {
            throw "Must set dx or dy or both";
        }
        cvector<cvector<double>> x = {
            {1, 0},
            {0, -1},
        };
        cvector<cvector<double>> y = {
            {0, 1},
            {-1, 0},
        };
        size_t dim = (img.mat.rows - 2 + 1) * (img.mat.cols - 2 + 1);
        cvector<double> gx(dim, 0);
        cvector<double> gy(dim, 0);
        if (dx)
        {
            gx = filter(img, x);
        }
        if (dy)
        {
            gy = filter(img, y);
        }
        return cvector<double>::mag(gx, gy);
    }

    cvector<double> canny(const Image &img, double threshold1, double threshold2)
    {
        if (!threshold1 || !threshold2)
        {
            throw "Must specify thresholds 1 and 2";
        }
        double max = std::max(threshold1, threshold2);
        double min = std::min(threshold1, threshold2);

        cvector<double> gx = sobel(img, 1, 0);
        cvector<double> gy = sobel(img, 0, 1);
        cvector<double> mag = cvector<double>::mag(gx, gy);
        cvector<double> phase = cvector<double>::phase(gx, gy);
        cvector<cvector<double>> mtrx = mag.to_2d(img.mat.rows - 3 + 1, img.mat.cols - 3 + 1);
        cvector<cvector<double>> cpy = mtrx;

        // Lower bound cut-off suppression
        for (int row = 0; row < mtrx.size(); row++)
        {
            for (int col = 0; col < mtrx[0].size(); col++)
            {
                double p = abs(phase[(row * mtrx.size()) + col]);
                // 45
                if (p >= PI / 8 && p < 3 * PI / 8)
                {
                    // Top left or bottom right pixels
                    if ((row - 1 < 0 && col - 1 < 0) || (row + 1 == mtrx.size() && col + 1 == mtrx[0].size()))
                        continue;

                    else if (col + 1 == mtrx[0].size() || row - 1 < 0)
                    {
                        if (mtrx[row][col] < mtrx[row + 1][col - 1])
                            cpy[row][col] = 0;
                    }

                    else if (col - 1 < 0 || row + 1 == mtrx.size())
                    {
                        if (mtrx[row][col] < mtrx[row - 1][col + 1])
                            cpy[row][col] = 0;
                    }

                    else if (mtrx[row][col] < mtrx[row - 1][col + 1] || mtrx[row][col] < mtrx[row + 1][col - 1])
                        cpy[row][col] = 0;
                }
                // 90
                else if (p >= 3 * PI / 8 && p < 5 * PI / 8)
                {
                    if (row + 1 == mtrx.size())
                    {
                        if (mtrx[row][col] < mtrx[row - 1][col])
                            cpy[row][col] = 0;
                    }
                    else if (row - 1 < 0)
                    {
                        if (mtrx[row][col] < mtrx[row + 1][col])
                            cpy[row][col] = 0;
                    }
                    else if (mtrx[row][col] < mtrx[row + 1][col] || mtrx[row][col] < mtrx[row - 1][col])
                        cpy[row][col] = 0;
                }
                // 135
                else if (p >= 5 * PI / 8 && p < 7 * PI / 2)
                {
                    // Top right or bottom left pixels
                    if ((row - 1 < 0 && col + 1 == mtrx[0].size()) || (row + 1 == mtrx.size() && col - 1 < 0))
                        continue;

                    else if (col + 1 == mtrx[0].size() || row + 1 == mtrx.size())
                    {
                        if (mtrx[row][col] < mtrx[row - 1][col - 1])
                            cpy[row][col] = 0;
                    }

                    else if (col - 1 < 0 || row - 1 < 0)
                    {
                        if (mtrx[row][col] < mtrx[row + 1][col + 1])
                            cpy[row][col] = 0;
                    }
                    else if (mtrx[row][col] < mtrx[row - 1][col - 1] || mtrx[row][col] < mtrx[row + 1][col + 1])
                        cpy[row][col] = 0;
                }
                // 0
                else
                {
                    if (col + 1 == mtrx[0].size())
                    {
                        if (mtrx[row][col] < mtrx[row][col - 1])
                            cpy[row][col] = 0;
                    }

                    else if (col - 1 < 0)
                    {
                        if (mtrx[row][col] < mtrx[row][col + 1])
                            cpy[row][col] = 0;
                    }
                    else if (mtrx[row][col] < mtrx[row][col + 1] || mtrx[row][col] < mtrx[row][col - 1])
                        cpy[row][col] = 0;
                }
            }
        }

        // Double thresholding and hysteresis
        cvector<double> pxs;
        for (int row = 0; row < cpy.size(); row++)
        {
            for (int col = 0; col < cpy[0].size(); col++)
            {
                if (strong_edge(cpy, row, col, min, max))
                {
                    pxs.push_back(255);
                }
                else
                {
                    pxs.push_back(0);
                }
            }
        }
        return pxs;
    }

    bool strong_edge(cvector<cvector<double>> &mtrx, int row, int col, double min, double max)
    {
        if (mtrx[row][col] < min)
            return false;
        if (mtrx[row][col] >= max)
            return true;
        cvector<cvector<double>> sub = mtrx.range(row - 1, row + 2, col - 1, col + 2);
        for (int i = 0; i < sub.size(); i++)
        {
            for (int j = 0; j < sub[0].size(); j++)
            {
                if (sub[i][j] >= max)
                    return true;
            }
        }
        return false;
    }

    cvector<uchar> scale(cvector<double> pixels)
    {
        pixels = pixels.abs();
        cvector<uchar> res;
        double scaled = (255.0 / pixels.max());
        for (auto itr = pixels.begin(); itr != pixels.end(); itr++)
        {
            res.push_back((uchar)std::round((*itr) * scaled));
        }
        return res;
    }

    Image noise_filter(Image &source, int size, std::string type, double std)
    {
        cvector<cvector<double>> filter = gaussian_kernel(size, std);
        double pixel_value = 0;
        int limit = size - 1;
        cvector<uchar> sub_matrix;
        cv::Mat source_mat = source.mat.clone();
        int channels_number = source_mat.channels();
        int cols_number = source_mat.cols;
        int rows_number = source_mat.rows;
        for (int src_row = 0; src_row < rows_number - limit; src_row++)
        {
            for (int src_col = 0; src_col < cols_number - limit; src_col++)
            {
                for (int ch = 0; ch < channels_number; ch++)
                {
                    for (int flt_row = 0; flt_row < size; flt_row++)
                    {
                        for (int flt_col = 0; flt_col < size; flt_col++)
                        {

                            if (type == "Gaussian")
                            {
                                pixel_value += source_mat.at<cv::Vec3b>(src_row + flt_row, src_col + flt_col)[ch] * filter[flt_row][flt_col];
                            }
                            else if (type == "Mean" || type == "Median")
                            {
                                sub_matrix.push_back(source_mat.at<cv::Vec3b>(src_row + flt_row, src_col + flt_col)[ch]);
                            }
                        }
                    }

                    if (type == "Gaussian")
                    {
                        source_mat.at<cv::Vec3b>(src_row, src_col)[ch] = (uchar)pixel_value;
                        pixel_value = 0;
                    }
                    else if (type == "Mean")
                    {
                        source_mat.at<cv::Vec3b>(src_row, src_col)[ch] = sub_matrix.mean();
                        sub_matrix.clear();
                    }
                    else if (type == "Median")
                    {
                        source_mat.at<cv::Vec3b>(src_row, src_col)[ch] = sub_matrix.median();
                        sub_matrix.clear();
                    }
                }
            }
        }
        return Image(source_mat.colRange(0, cols_number - limit).rowRange(0, rows_number - limit));
    }

    Image add_noise(Image &source, std::string type, uchar noise_factor)
    {

        cv::Mat source_mat = source.mat.clone();

        std::default_random_engine generator;
        std::normal_distribution<double> gauss_random(0, 1);
        std::uniform_real_distribution<double> uniform_random(-1, 1);
        std::uniform_int_distribution<int> sp_random(0, 100);

        int channels_number = source_mat.channels();
        int cols_number = source_mat.cols;
        int rows_number = source_mat.rows;
        double noise = 0.f;
        int random_salt;

        for (int row = 0; row < rows_number; row++)
        {
            for (int col = 0; col < cols_number; col++)
            {
                if (type == "Gaussian")
                {
                    noise = gauss_random(generator) * noise_factor;
                }
                else if (type == "Uniform")
                {
                    noise = uniform_random(generator) * noise_factor;
                }
                else if (type == "Salt")
                {
                    random_salt = sp_random(generator);
                }
                for (int ch = 0; ch < channels_number; ch++)
                {
                    if (type == "Salt")
                    {
                        if (random_salt < noise_factor)
                        {
                            source_mat.at<cv::Vec3b>(row, col)[ch] = 0;
                        }
                        else if (random_salt > (100 - noise_factor))
                        {
                            source_mat.at<cv::Vec3b>(row, col)[ch] = 255;
                        }
                    }
                    else
                    {
                        source_mat.at<cv::Vec3b>(row, col)[ch] += (uchar)noise;
                    }
                }
            }
        }
        return Image(source_mat);
    }

    Image histogram_equalization(Image &eq_img)
    {
        int hist[256] = {0};
        Image equalized = eq_img.mat.clone();
        Image hsv = convert(equalized, "bgr", "hsv");
        cvector<Image> splt = split(hsv);
        for (int i = 0; i < equalized.mat.rows * equalized.mat.cols; i++)
            hist[(int)(splt[2].pixels[i])] = hist[(int)((splt[2].pixels[i]) * 2.55)] + 1;
        int sum = 0;
        int pdf[256] = {0};
        int map[256] = {0};
        for (int i = 0; i < 256; i++)
        {
            sum = sum + hist[i];
            pdf[i] = sum;
        }
        for (int i = 0; i < 256; i++)
        {
            map[i] = (int)((pdf[i] - pdf[0] - 0.f) * 255 / (pdf[255] - pdf[0]));
        }

        for (int i = 0; i < equalized.mat.rows * equalized.mat.cols; i++)
            splt[2].mat.data[i] = map[(int)((splt[2].pixels[i]) / 255.0 * 100)];

        Image merged = merge(splt);
        return convert(merged, "hsv", "bgr");
    }

    // Image globalThreshold(Image &img)
    Image globalThreshold(Image &img, float threshold, float mx_value, t_types thresholdType)
    {
        cv::Mat cp = img.mat.clone();
        cv::Mat thresholded_image = cv::Mat(cp.rows, cp.cols, CV_8UC1);
        for (int i = 0; i < img.mat.rows; i++)
        {
            for (int j = 0; j < img.mat.cols; j++)
            {
                float pixelValue = cp.at<uchar>(i, j);
                if (thresholdType == THRESH_BIN)
                {
                    if (pixelValue > threshold)
                    {
                        thresholded_image.at<uchar>(i, j) = mx_value;
                    }
                    else
                    {
                        thresholded_image.at<uchar>(i, j) = 0;
                    }
                }
                else
                {
                    if (pixelValue > threshold)
                    {
                        thresholded_image.at<uchar>(i, j) = 0;
                    }
                    else
                    {
                        thresholded_image.at<uchar>(i, j) = mx_value;
                    }
                }
            }
        }
        return Image(thresholded_image);
    }

    Image localThreshold(Image &img, float mx_value, t_types thresholdType)
    {
        cv::Mat cp = img.mat.clone();
        cv::Mat thresholded_image = cv::Mat(cp.rows, cp.cols, CV_8UC1);
        float gcurr = 0.25;
        float gup = 0.125;
        float gleft = 0.125;
        float gright = 0.125;
        float gdown = 0.125;
        float gur = 0.0625;
        float gul = 0.0625;
        float gdr = 0.0625;
        float gdl = 0.0625;
        unsigned char curr = 0;
        unsigned char up = 0;
        unsigned char left = 0;
        unsigned char right = 0;
        unsigned char down = 0;
        unsigned char ur = 0;
        unsigned char ul = 0;
        unsigned char dr = 0;
        unsigned char dl = 0;
        int rows = img.mat.rows;
        int cols = img.mat.cols;
        int count = 0;
        for (int r = 0; r < rows - 1; r++)
        {
            for (int c = 0; c < cols - 1; c++)
            {
                // std::cout << count << std::endl;
                // std::cout << r << " : " <<  c << std::endl;
                count++;
                if (c - 1 >= 0)
                {
                    // std::cout << "left" << std::endl;
                    // left = gleft * im->data[(r * cols) + (c - 1)].i;
                    left = gleft * cp.at<uchar>(r, (c - 1));
                    // std::cout << "end left" << std::endl;
                    if (r - 1 >= 0)
                    {
                        // std::cout << "ul" << std::endl;
                        // ul = gul * im->data[((r - 1) * cols) + (c - 1)].i;
                        ul = gul * cp.at<uchar>((r - 1), (c - 1));
                        // std::cout << "end ul" << std::endl;
                    }
                    if (r + 1 <= rows)
                    {
                        // dl = gdl * im->data[((r + 1) * cols) + (c - 1)].i;
                        // std::cout << "dl" << std::endl;
                        dl = gdl * cp.at<uchar>((r + 1), (c - 1));
                        // std::cout << "end dl" << std::endl;
                    }
                }
                if (c + 1 <= cols)
                {
                    // right = gright * im->data[(r * cols) + (c + 1)].i;
                    // std::cout << "right" << std::endl;
                    right = gright * cp.at<uchar>(r, (c + 1));
                    // std::cout << "end right" << std::endl;
                    if (r - 1 >= 0)
                    {
                        // ur = gur * im->data[((r - 1) * cols) + (c + 1)].i;
                        // std::cout << "ul" << std::endl;
                        ur = gur * cp.at<uchar>((r - 1), (c + 1));
                        // std::cout << "end ul" << std::endl;
                    }
                    if (r + 1 <= rows)
                    {
                        // dr = gdr * im->data[((r + 1) * cols) + (c + 1)].i;
                        // std::cout << "dr" << std::endl;
                        dr = gdr * cp.at<uchar>((r + 1), (c + 1));
                        // std::cout << "end dr" << std::endl;
                    }
                }
                if (r - 1 >= 0)
                {
                    // up = gup * im->data[((r - 1) * cols) + c].i;
                    up = gup * cp.at<uchar>((r - 1), c);
                }
                if (r + 1 <= rows)
                {
                    // down = gdown * im->data[((r + 1) * cols) + c].i;
                    down = gdown * cp.at<uchar>((r + 1), c);
                }
                // curr = gcurr * im->data[(r * cols) + c].i;
                curr = gcurr * cp.at<uchar>(r, c);
                unsigned char weightedAvg = down + up + left + right + curr + ul + ur + dl + dr;
                if (thresholdType == THRESH_BIN)
                {
                    if (cp.at<uchar>(r, c) > weightedAvg)
                    {
                        thresholded_image.at<uchar>(r, c) = mx_value;
                    }
                    else
                    {
                        thresholded_image.at<uchar>(r, c) = 0;
                    }
                }
                else
                {
                    if (cp.at<uchar>(r, c) > weightedAvg)
                    {
                        thresholded_image.at<uchar>(r, c) = 0;
                    }
                    else
                    {
                        thresholded_image.at<uchar>(r, c) = mx_value;
                    }
                }
                curr = 0;
                up = 0;
                left = 0;
                right = 0;
                down = 0;
                ur = 0;
                ul = 0;
                dr = 0;
                dl = 0;
            }
        }
        return Image(thresholded_image);
    }

    Image PassFilter(Image &img, float freq_threshold, p_types pass_type)
    {
        cv::Mat imgIn = img.mat;
        imgIn.convertTo(imgIn, CV_32F);
        // DFT
        cv::Mat DFT_image;
        DFT(imgIn, DFT_image);

        // construct H
        cv::Mat H;
        H = construct_H(imgIn, freq_threshold, pass_type);

        // filtering
        cv::Mat complexIH;
        filtering(DFT_image, complexIH, H);

        // IDFT
        cv::Mat imgOut;
        cv::dft(complexIH, imgOut, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

        cv::normalize(imgOut, imgOut, 0, 1, cv::NORM_MINMAX);
        return Image(imgOut);
    }

    Image hybridImages(Image &img1, Image &img2)
    {
        cv::Mat imgIn1 = img1.mat;
        imgIn1.convertTo(imgIn1, CV_32F);

        cv::Mat imgIn2 = img2.mat;
        imgIn2.convertTo(imgIn2, CV_32F);

        // DFT
        cv::Mat DFT_image_1;
        cv::Mat DFT_image_2;
        DFT(imgIn1, DFT_image_1);
        DFT(imgIn2, DFT_image_2);

        // construct H
        cv::Mat H;
        cv::Mat L;
        L = construct_H(imgIn1, 40, HIGH_PASS_FILTER);
        H = construct_H(imgIn2, 40, LOW_PASS_FILTER);

        // filtering
        cv::Mat complexIH;
        filtering(DFT_image_1, complexIH, H);
        cv::Mat complexIL;
        filtering(DFT_image_2, complexIL, L);

        cv::Mat hyprid;
        cv::add(complexIH, complexIL, hyprid);

        // IDFT
        cv::Mat imgOut;
        cv::dft(hyprid, imgOut, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);

        cv::normalize(imgOut, imgOut, 0, 1, cv::NORM_MINMAX);
        return Image(imgOut);
    }
}