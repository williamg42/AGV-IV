////////////////////////////////////////////////////////////////////////////////////////////////////
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

cv::Mat frame;

int main(int argc, char *argv[])
{ 

VideoCapture cap; // declare capture variable
cap.open(0); // open camera zero

cap >> frame; // grab a frame
Size size(480, 320); 
resize(frame, frame, size); //resize image
cv::VideoWriter record; // declare writer handle
cv::VideoWriter recordfinal; // declare writer handle

if( !cap.isOpened() )
{
printf("can not open camera \n");
return -1;
}

 Mat processed;
// declare the writer properties
record.open("Orginal.avi", CV_FOURCC('M','J','P','G'), 5, frame.size(), true); 

recordfinal.open("Processed.avi", CV_FOURCC('M','J','P','G'), 5, frame.size(), true); 

cv::namedWindow("Processed");
cv::namedWindow("Original");

while(1)
{
cap >> frame;
if(frame.empty()) break;

processed = SEGMENT(frame);

cvtColor(processed,processed, CV_GRAY2BGR);
resize(frame, frame, size); //resize image
record.write(frame); // save the video file 
recordfinal.write(processed); // save the video file

imshow("Processed",processed); 
imshow("Original",frame); 







//press ESC to exit
char c= cvWaitKey(33);
if(c == 27)
{
break;
}

} // while(1) loop end




cap.release();
record.release();
recordfinal.release();
cv::destroyWindow( "Original" );
cv::destroyWindow( "Processed" );
frame.release();
processed.release();



return 0;
}

//////////////////////////////////////////////////////////////////////////


