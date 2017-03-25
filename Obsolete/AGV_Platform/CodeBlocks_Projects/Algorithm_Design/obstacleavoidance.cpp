#include "obstacleavoidance.hpp"

#define xsize 64
#define ysize 48
#define angularconstant .005
#define dmin 5
#define velocityconstant .03


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



    float angularvelocity = angularconstant*(leftlower-rightlower);
    float velocity = velocityconstant*(middlelower-dmin);


    std::cout << std::endl << "angular velocity: " << angularvelocity;
    std::cout << std::endl << "velocity: " << velocity;






}
