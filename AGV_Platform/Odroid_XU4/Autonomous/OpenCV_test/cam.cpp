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

using namespace std;
using namespace cv;

cv::Mat frame;

int main(int argc, char *argv[])
{ 

VideoCapture cap; // delare capture variable
cap.open(0); // open camera zero

cap >> frame; // grabe a frame
cv::VideoWriter record; // decalre writer handle

if( !cap.isOpened() )
{
printf("can not open camera \n");
return -1;
}



// decalre the writer properties
record.open("Video.avi", CV_FOURCC('M','J','P','G'), 10,frame.size() , true); 

cv::namedWindow("Frame");

while(1)
{

cap >> frame;
if(frame.empty()) break;




record.write(frame); // save the video file 

imshow("Frame",frame); 



//press ESC to exit
char c= cvWaitKey(5);
//if(c == 27)
//{
//break;
//}

} // while(1) loop end




cap.release();
record.release();
cv::destroyWindow( "Frame" );
frame.release();



return 0;
}

//////////////////////////////////////////////////////////////////////////