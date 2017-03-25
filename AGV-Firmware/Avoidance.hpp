#ifndef AVOIDANCE_H
#define AVOIDANCE_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>


#define histBuffer 25
class Avoidance
{
public:

    Avoidance(int cameraNum);
    virtual ~Avoidance();
    bool run_noAvoidance(bool flag);
    bool Init();
    cv::Mat Processed_Frame();
    cv::Mat Raw_Frame();
    cv::Mat Edges_Frame();


protected:

    struct histogram_History
    {
        cv::Mat H_hist;
        cv::Mat V_hist;
        float roll;
        float pitch;
        float yaw;
        bool valid = false;

    };

private:
    int threshold_hue = 40;
    int threshold_v = 30;
    int size_X = 160;
    int size_Y = 90;
    int histSizeI = 256;
    int histSizeH = 256;

    float system_Roll;
    float system_Pitch;
    float system_Yaw;
    bool uniform = true;
    bool accumulate = false;
    cv::Mat currentFrame, processedFrame, maskedFrame;
    int fdCam;
    histogram_History History[histBuffer];
    cv::VideoCapture cap;
    cv::Mat mask = cv::Mat::zeros(size_Y, size_X, CV_8U);
    cv::Mat mask_Hue[histBuffer];
    cv::Mat mask_Value[histBuffer];
    cv::Mat Result;
    cv::Mat Edges;
    std::vector<cv::Mat> HSV_planes;
    cv::Mat element;

    int buffPosition = 0;

    bool frameCapture();
    bool ProcessFrame(bool flag);
    bool fillBuffer();
    bool DetectObstacles();


};

#endif // AVOIDANCE_H
