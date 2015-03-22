/*
 * HSI.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: william
 */
#include "BGR2HSI2.h"

#define Intense_Thresh 2
#define Sat_Thresh 3

cv::Mat BGR2HSI2(cv::Mat Photo)
{

	int y = Photo.rows;
	int x = Photo.cols;
	cv::Mat bgr_image_f;
	Photo.convertTo(bgr_image_f, CV_32FC3);

	// Extract the color planes and calculate I = (r + g + b) / 3
	std::vector<cv::Mat> planes(3);
	cv::split(bgr_image_f, planes);

	cv::Mat intensity_f((planes[0] + planes[1] + planes[2]) / 3.0f);



	//void divide(InputArray src1, InputArray src2, OutputArray dst, double scale=1, int dtype=-1)
	cv::Mat b_normalized_f;
	cv::divide(planes[0], (planes[0] + planes[1] + planes[2]), b_normalized_f);


	cv::Mat g_normalized_f;
	cv::divide(planes[1], (planes[0] + planes[1] + planes[2]), g_normalized_f);


	cv::Mat r_normalized_f;
	cv::divide(planes[2], (planes[0] + planes[1] + planes[2]), r_normalized_f);



	cv::Mat numi = .5
			* ((r_normalized_f - g_normalized_f)
					+ (r_normalized_f - b_normalized_f));


	cv::Mat rminusgsquared;
	multiply((r_normalized_f - g_normalized_f),
			(r_normalized_f - g_normalized_f), rminusgsquared);

	cv::Mat rminusgmultigminusb;
	multiply((r_normalized_f - b_normalized_f),
			(g_normalized_f - b_normalized_f), rminusgmultigminusb);

	cv::Mat denom = (rminusgsquared + rminusgmultigminusb);

	sqrt(denom, denom);

	cv::Mat H = cv::Mat::zeros(y, x, CV_32FC1); //=acosd(numi./(denom+0.000001));
	cv::Mat S = cv::Mat::zeros(y, x, CV_32FC1);
	cv::Mat I = cv::Mat::zeros(y, x, CV_32FC1);


	min(min(g_normalized_f, b_normalized_f), r_normalized_f, S);

		subtract(1, (3 * S), S);

		S = S * 100;
		I = intensity_f;





	int rows = 0;
	int cols = 0;
	while (rows < y) {

		while (cols < x) {

			double q = (double) numi.at<float>(rows, cols)
					/ (double) (denom.at<float>(rows, cols) + .000001);

			float m = b_normalized_f.at<float>(rows, cols);
			float n = g_normalized_f.at<float>(rows, cols);

			if (m <= n) {

				H.at<float>(rows, cols) = acos(q);

			}

			else {

				H.at<float>(rows, cols) = ((2 * M_PI) - acos(q));

			}

			if (I.at<float>(rows, cols) < Intense_Thresh) {
				H.at<float>(rows, cols) = 0;
				S.at<float>(rows, cols) = 0;
			}

			if (S.at<float>(rows, cols) < Sat_Thresh) {
				H.at<float>(rows, cols) = 0;

			}

			cols = cols + 1;

		}
		cols = 0;

		rows = rows + 1;

	}

	H = H * (180 / M_PI);


	H.convertTo(H, CV_8UC1, .7083333333);
	S.convertTo(S, CV_8UC1);
	I.convertTo(I, CV_8UC1);


	cv::Mat HSI;


	cv::vector<cv::Mat> channels;
	  channels.push_back(H);
	    channels.push_back(S);
	    channels.push_back(I);

	merge(channels, HSI);

	return HSI;
}


