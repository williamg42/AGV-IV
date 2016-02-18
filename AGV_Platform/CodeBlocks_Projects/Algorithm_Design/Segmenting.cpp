/*
 * HSI.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: william
 */
#include "Segmenting.hpp"

cv::Mat SEGMENT(cv::Mat Photo)
{
    int x = 480;
    int y = 320;
	int threshold_hue = 180;
	int threshold_sat = 70;
	int threshold_v = 200;


    Mat src, srcclone, photoclone, dst;

    photoclone = Photo.clone();

    Size size(x, y); //the dst image size,e.g.100x100

    resize(photoclone, photoclone, size); //resize image

    srcclone = photoclone.clone();

    GaussianBlur(srcclone, srcclone, Size(17, 17), 0, 0);

    /// Separate the image in 3 places ( B, G and R )

    //src = BGR2HSI2(Photo);
    cvtColor(srcclone, src, CV_BGR2HSV_FULL);

    Mat mask = Mat::zeros(y, x, CV_8U);
    vector<Point> contour; //    x  y

    contour.push_back(Point(x, y));
    contour.push_back(Point(0, y));
    contour.push_back(Point((x / 2 - .25 * x), y - y / 3)); //value on Y controls depth into the photo, decimal controls width of box
    contour.push_back(Point((x / 2 + .25 * x), y - y / 3));


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
    int histSizeS = 256;
    int histSizeH = 256;

    /// Set the ranges ( for B,G,R) )
    float rangeH[] = { 0, 256 };
    const float* histRangeH = { rangeH };
    float rangeS[] = { 0, 256 };
    const float* histRangeS = { rangeS };
    float rangeI[] = { 0, 256 };
    const float* histRangeI = { rangeI };

    bool uniform = true;
    bool accumulate = false;

    Mat H_hist, S_hist, I_hist;

    /// Compute the histograms:
    calcHist(&HSI_planes[0], 1, 0, mask, H_hist, 1, &histSizeH, &histRangeH,
             uniform, accumulate);
    calcHist(&HSI_planes[1], 1, 0, mask, S_hist, 1, &histSizeS, &histRangeS,
             uniform, accumulate);
    calcHist(&HSI_planes[2], 1, 0, mask, I_hist, 1, &histSizeI, &histRangeI,
             uniform, accumulate);

    medianBlur(H_hist, H_hist, 5);

    medianBlur(S_hist, S_hist, 5);

    medianBlur(I_hist, I_hist, 5);

    Mat hue_mask = Mat::zeros(y, x, CV_8U);
    Mat sat_mask = Mat::zeros(y, x, CV_8U);
    Mat Int_mask = Mat::zeros(y, x, CV_8U);

    int rows = 0;
    int cols = 0;

    while (rows < y)
    {

        while (cols < x)
        {

            Vec3b intensity = src.at<Vec3b>(rows, cols);
            int Hue = intensity.val[0];
            int Sat = intensity.val[1];
            int V = intensity.val[2];

            //hue_mask.at<uchar>(60, cols) = 255;

            if (H_hist.at<float>(Hue) < threshold_hue)
            {
                hue_mask.at<uchar>(rows, cols) = 255;
            }

            else
            {
                hue_mask.at<uchar>(rows, cols) = 0;
            }

            if (S_hist.at<float>(Sat) < threshold_sat)
            {
                sat_mask.at<uchar>(rows, cols) = 255;
            }

            else
            {
                sat_mask.at<uchar>(rows, cols) = 0;
            }

            if (I_hist.at<float>(V) < threshold_v)
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
    Mat element = getStructuringElement(MORPH_RECT, Size(3 * 3 + 1, 3 * 3 + 1),
                                        Point(3, 3));


    erode(hue_mask, hue_mask, element);
    erode(Int_mask, Int_mask, element);

    erode(hue_mask, hue_mask, element);
    erode(Int_mask, Int_mask, element);





    Mat Result;
    bitwise_or(Int_mask, hue_mask, Result);

    //erode(Result, Result, element);
    //erode(Result, Result, element);



//		namedWindow("Sat Mask", WINDOW_NORMAL);
//		imshow("Sat Mask", sat_mask);
//
//		namedWindow("Hue Mask", WINDOW_NORMAL);
//		imshow("Hue Mask", hue_mask);
//
//		namedWindow("I Mask", WINDOW_NORMAL);
//		imshow("I Mask", Int_mask);
//
//		namedWindow("Result", WINDOW_NORMAL);
//		imshow("Result", Result);


    return Result;


}


