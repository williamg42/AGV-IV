/*
 * MovingAvarageFilter.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: william
 */
#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(char newDataPointsCount) {
	t = 0; //initialize so that we start to write at index 0
	if (newDataPointsCount < MAX_DATA_POINTS)
		dataPointsCount = newDataPointsCount;
	else
		dataPointsCount = MAX_DATA_POINTS;
	for (i = 0; i < dataPointsCount; i++) {
		values[i] = 0; // fill the array with 0's
	}
}
double MovingAverageFilter::process(double in) {
	output = 0;
	values[t] = in;
	t = (t + 1) % dataPointsCount;
	for (i = 0; i < dataPointsCount; i++) {
		output += values[i];
	}
	return output / dataPointsCount;
}

