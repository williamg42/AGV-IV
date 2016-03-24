/*
 * HSI.h
 *
 *  Created on: Mar 17, 2015
 *      Author: william
 */

#ifndef SRC_SEGMENTING_H_
#define SRC_SEGMENTING_H_
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "BGR2HSI2.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

cv::Mat SEGMENT(const cv::Mat &Photo);




#endif /* SRC_SEGMENTING_H_ */
