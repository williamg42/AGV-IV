/*
 * segmenting.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: william
 */
#include "Segmenting.hpp"

cv::Mat SEGMENT(cv::Mat Photo)
{
    int x = 320;
    int y = 240;
    int threshold_hue = 50;
    int threshold_v = 70;


    Mat src, srcclone, photoclone, dst;

    photoclone = Photo.clone();

    srcclone = photoclone.clone();

    GaussianBlur(srcclone, srcclone, Size(5, 5), 0, 0);

    /// Separate the image in 3 places ( B, G and R )


    //cvtColor(srcclone, src, CV_BGR2HSV_FULL);
    src = BGR2HSI2(srcclone);


    Mat mask = Mat::zeros(y, x, CV_8U);
    vector<Point> contour; //    x  y

    contour.push_back(Point(x, y));
    contour.push_back(Point(0, y));
    contour.push_back(Point((x / 2 - .15 * x), y - y / 2.5)); //value on Y controls depth into the photo, decimal controls width of box
    contour.push_back(Point((x / 2 + .15 * x), y - y / 2.5));


    // For debugging purposes, draw green lines connecting those points
    // and save it on disk
    const Point* point = &contour[0];
    int npts = (int) contour.size();
    Mat draw = photoclone.clone();
    polylines(draw, &point, &npts, 1, true, Scalar(0, 255, 0), 3, CV_AA);
    namedWindow("draw.jpg");
    imshow("draw.jpg", draw);

    const cv::Point *pts = (const cv::Point*) Mat(contour).data;

    fillConvexPoly(mask, pts, npts, Scalar(255, 255, 255), 4, 0);

    vector<Mat> HSI_planes;
    split(src, HSI_planes);

    /// Establish the number of bins
    int histSizeI = 256;
    int histSizeH = 256;

    /// Set the ranges ( for B,G,R) )
    float rangeH[] = { 0, 255 };
    const float* histRangeH = { rangeH };
    float rangeI[] = { 0, 255 };
    const float* histRangeI = { rangeI };

    bool uniform = true;
    bool accumulate = false;

    Mat H_hist, I_hist;

    /// Compute the histograms:
    calcHist(&HSI_planes[0], 1, 0, mask, H_hist, 1, &histSizeH, &histRangeH,
             uniform, accumulate);
    calcHist(&HSI_planes[2], 1, 0, mask, I_hist, 1, &histSizeI, &histRangeI,
             uniform, accumulate);

    medianBlur(H_hist, H_hist, 5);
    medianBlur(I_hist, I_hist, 5);

    Mat hue_mask = Mat::zeros(y, x, CV_8U);
    Mat Int_mask = Mat::zeros(y, x, CV_8U);

    int rows = 0;
    int cols = 0;

    while (rows < y)
    {

        while (cols < x)
        {

            Vec3b intensity = src.at<Vec3b>(rows, cols);
            int Hue = intensity.val[0];
            int I = intensity.val[2];



            if (H_hist.at<int>(Hue) < threshold_hue)
            {
                hue_mask.at<uchar>(rows, cols) = 255;
            }

            else
            {
                hue_mask.at<uchar>(rows, cols) = 0;
            }

            if (I_hist.at<int>(I) < threshold_v)
            {
                Int_mask.at<uchar>(rows, cols) = 255;
            }

            else
            {
                Int_mask.at<uchar>(rows, cols) = 0;
            }

            cols = cols + 1;

        }
        cols = 0;

        rows = rows + 1;

    }




    Mat Result;
    bitwise_or(Int_mask, hue_mask, Result);
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * 2 + 1, 2 * 2 + 1), Point(2, 2));
    morphologyEx( Result, Result, MORPH_OPEN, element );

    return Result;


}


