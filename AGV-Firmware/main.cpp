#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Avoidance.hpp"
#include <sys/timeb.h>
#include <unistd.h>
#include <signal.h>

#define BUFFERDELAY .25

using namespace cv;
cv::Mat dst;


double CLOCK()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}


double _avgdur=0;
double _fpsstart=0;
double _avgfps=0;
double _fps1sec=0;

double avgdur(double newdur)
{
    _avgdur=0.98*_avgdur+0.02*newdur;
    return _avgdur;
}

double avgfps()
{
    if(CLOCK()-_fpsstart>1000)
    {
        _fpsstart=CLOCK();
        _avgfps=0.7*_avgfps+0.3*_fps1sec;
        _fps1sec=0;
    }
    _fps1sec++;
    return _avgfps;
}

////////////////////////////////////////////////////////////

volatile sig_atomic_t cap_flag = false;

void sigalrm_handler(int sig)
{
    cap_flag = true;
}





int main(int argc, char *argv[])
{


    signal(SIGALRM, &sigalrm_handler);  // set a signal handler
    alarm(BUFFERDELAY);  // set an alarm for 10 seconds from now

    int frameno=0;

    Avoidance Algorithm(0);
    Algorithm.Init();



    double alpha = 0.5;
    double beta;
    double input;






    while(1)
    {
        double start=CLOCK();
        Algorithm.run_noAvoidance(cap_flag);
        double dur = CLOCK()-start;
        printf("avg time per frame %f ms. fps %f. frameno = %d\n",avgdur(dur),avgfps(),frameno++ );

        /// Create Windows
        namedWindow("Linear Blend", CV_WINDOW_NORMAL);
        cvtColor(Algorithm.Processed_Frame(), dst, CV_GRAY2BGR);
        beta = ( 1.0 - alpha );
        addWeighted( Algorithm.Raw_Frame(), alpha, dst, beta, 0.0, dst);



        /////////////////////////////////
//
//
//
//
//        int x = dst.cols;
//        int y = dst.rows;
//
//
//    vector<Point> contour; //    x  y
//
//    contour.push_back(Point(x, y));
//    contour.push_back(Point(0, y));
//    contour.push_back(Point((x / 2 - .07 * x), y - y / 2.5  )); //value on Y controls depth into the photo, decimal controls width of box
//    contour.push_back(Point((x / 2 + .07 * x), y - y / 2.5));
//
//
//    // For debugging purposes, draw green lines connecting those points
//    // and save it on disk
//    const Point* point = &contour[0];
//    int npts = (int) contour.size();
//    polylines(dst, &point, &npts, 1, true, Scalar(0, 255, 0), 3, CV_AA);

       ////////////////////////////////////////////////////////////

        imshow( "Linear Blend", dst );

        //namedWindow("Edges Frame", CV_WINDOW_NORMAL);
        //imshow( "Edges Frame", Algorithm.Edges_Frame());


        if (cap_flag)
        {
            cap_flag = false;
            alarm(BUFFERDELAY);
        }


//press ESC to exit
        char c= cvWaitKey(1);
        if(c == 27)
        {
            break;
        }

    } // while(1) loop end





    cv::destroyWindow( "Linear Blend" );
    dst.release();



    return 0;
}



