/*
 * HSI.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: william
 */
#include "BGR2HSI2.hpp"


cv::Mat BGR2HSI2(cv::Mat Photo)
{

    Mat hsi(Photo.rows, Photo.cols, CV_8UC3);

    float r, g, b, h, s, in;

    for(int i = 0; i < Photo.rows; i++)
    {
        for(int j = 0; j < Photo.cols; j++)
        {
            b = Photo.at<Vec3b>(i, j)[0];
            g = Photo.at<Vec3b>(i, j)[1];
            r = Photo.at<Vec3b>(i, j)[2];

            in = (b + g + r) / 3;

            int min_val = 0;
            min_val = std::min(r, std::min(b,g));

            s = 1 - 3*(min_val/(b + g + r));
            if(s < 0.00001)
            {
                s = 0;
            }
            else if(s > 0.99999)
            {
                s = 1;
            }

            if(s != 0)
            {
                h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
                h = acos(h);

                if(b <= g)
                {
                    h = h;
                }
                else
                {
                    h = ((360 * 3.14159265) / 180.0) - h;
                }
            }

            hsi.at<Vec3b>(i, j)[0] = ((h * 180) / 3.14159265)*(255/360);
            hsi.at<Vec3b>(i, j)[1] = s*255;
            hsi.at<Vec3b>(i, j)[2] = in;
        }
    }

    Mat channel[3];

    // The actual splitting.
    split(hsi, channel);


    namedWindow("hsi");
    imshow("hsi", hsi);


    return hsi;
}


