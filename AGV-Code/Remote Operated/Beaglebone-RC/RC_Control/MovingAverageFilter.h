#ifndef MovingAverageFilter_h
#define MovingAverageFilter_h
#define MAX_DATA_POINTS 200

class MovingAverageFilter {

public:
//construct without coefs

MovingAverageFilter(char newDataPointsCount);
double process(double in);

private:
double values[MAX_DATA_POINTS];
int t; // k stores the index of the current array read to create a circular memory through the array
int dataPointsCount;
double output;
int i; // just a loop counter

};
#endif
