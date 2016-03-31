#include "obstacleavoidance.hpp"

#define xsize 64
#define ysize 48


void obstacleavoidance(const cv::Mat &Photo)
{
    int bproject[xsize] = {ysize-1};
    std::fill_n(bproject, xsize, ysize-1);
    Mat image;
    Size size(xsize, ysize);
    resize (Photo, image, size);

    namedWindow("resized", WINDOW_KEEPRATIO);
    imshow("resized", image);

    for (int row = 0; row < xsize; row++)
    {
        for (int cols = ysize-1; cols > -1 ; cols--)
        {
            int value = image.at<uchar>(cols,row);
            if (image.at<uchar>(cols,row) != 0)
            {
                bproject[row] = 47-cols;
                break;
            }

        }
    }

    int leftlower = bproject[0];

    for (int i = 1; i<(xsize-xsize/4)/2; i++)
    {
        if (bproject[i] < leftlower)
            leftlower = bproject[i];
    }

    int middlelower = bproject[(xsize-xsize/4)/2];

    for (int i = (xsize-xsize/4)/2; i < (xsize-xsize/4)/2+(xsize/4); i++)
    {
        if (bproject[i] < middlelower)
            middlelower = bproject[i];
    }

    int rightlower = bproject[(xsize-xsize/4)/2+(xsize/4)];

    for (int i = (xsize-xsize/4)/2+(xsize/4); i < xsize; i++)
    {
        if (bproject[i] < rightlower)
            rightlower = bproject[i];
    }


    std::cout << std::endl << "Left lower pixel: " << leftlower;
    std::cout << std::endl << "Middle lower pixel: " << middlelower;
    std::cout << std::endl << "Right lower pixel: " << rightlower;


}
