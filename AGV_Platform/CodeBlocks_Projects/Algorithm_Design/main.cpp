#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
#include<vector>
#include <stdio.h>
#include <fstream>
#include<time.h>

#include <string>

#include <ctype.h>
#include <math.h>
#include <sstream>

#include "Segmenting.hpp"

using namespace std;
using namespace cv;

#include "opencv2/opencv.hpp"



int main(int, char**)
{
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Size size(480, 320);
    Mat processed;

    namedWindow("Linear Blend", 1);

    Mat blend;
    double alpha = 0.5;
    double beta;

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera

        processed = SEGMENT(frame);

        cvtColor(processed,blend, CV_GRAY2BGR);

        beta = ( 1.0 - alpha );
        resize(frame, frame, size); //resize image
        addWeighted( frame, alpha, blend,  beta, 0.0, blend);

        imshow( "Linear Blend", blend );

        if(waitKey(5) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

//cv::Mat frame;
//
//int main(int argc, char *argv[])
//{
//
//
//    Size size(480, 320);
//
//    frame = cv::imread("Capture3.png", CV_LOAD_IMAGE_COLOR);// read the file
//
//    resize(frame, frame, size); //resize image
//
//
//    Mat processed;
//// declare the writer properties
//
//
//    cv::namedWindow("Processed");
//
//
//    processed = SEGMENT(frame);
//
//    imshow("Processed",processed);
//
//
//    waitKey(0);// wait for a keystroke in the window
//
//
//    cv::destroyWindow( "Processed" );
//    frame.release();
//    processed.release();


//
//    return 0;
//}

//////////////////////////////////////////////////////////////////////////
