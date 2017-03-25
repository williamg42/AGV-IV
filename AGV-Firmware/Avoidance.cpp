#include "Avoidance.hpp"

Avoidance::Avoidance(int cameraNum) :  fdCam(cameraNum)
{
    std::vector<cv::Point> contour;
    contour.push_back(cv::Point(size_X, size_Y));
    contour.push_back(cv::Point(0, size_Y));
    contour.push_back(cv::Point((size_X / 2 - .07 * size_X), size_Y - size_Y / 2.5  )); //value on Y controls depth into the photo, decimal controls width of box
    contour.push_back(cv::Point((size_X / 2 + .07 * size_X), size_Y - size_Y / 2.5));
   // contour.push_back(cv::Point((size_X / 2 - .1 * size_X), size_Y - size_Y / 4  ));
   // contour.push_back(cv::Point((size_X / 2 + .1 * size_X), size_Y - size_Y / 4));
    int npts = (int) contour.size();
    const cv::Point *pts = (const cv::Point*) cv::Mat(contour).data;
    cv::fillConvexPoly(mask, pts, npts, cv::Scalar(255, 255, 255), 4, 0);
    element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    system_Roll = 0;
    system_Pitch = 0;
    system_Yaw = 0;


}



Avoidance::~Avoidance()
{
    cap.release();
    mask.release();
    Result.release();
}

bool Avoidance::Init()
{
    cap.open("test_footage.mp4"); // open camera zero
    if( !cap.isOpened() )
    {
        return false;
    }

    for(int i = 0; i < histBuffer; i++)
    {
        mask_Value[i] = cv::Mat::zeros(size_Y, size_X, CV_8U);
        mask_Hue[i] = cv::Mat::zeros(size_Y, size_X, CV_8U);
    }
    return this ->fillBuffer();
}

bool Avoidance::run_noAvoidance(bool flag)
{
    this -> frameCapture();
    this -> ProcessFrame(flag);
    this -> DetectObstacles();

}


bool Avoidance::frameCapture()
{

    cap >> currentFrame; // grab a frame
    cv::Size size(size_X, size_Y);
    resize(currentFrame, currentFrame, size); //resize image
    return true;

}

cv::Mat Avoidance::Processed_Frame()
{

    return this->Result;

}

cv::Mat Avoidance::Raw_Frame()
{


    return this->currentFrame;

}

cv::Mat Avoidance::Edges_Frame()
{


    return this->Edges;

}


bool Avoidance::ProcessFrame(bool flag)
{

    float rangeH[] = { 0, 256 };
    const float* histRangeH = { rangeH };
    float rangeI[] = { 0, 256 };
    const float* histRangeI = { rangeI };
    processedFrame = currentFrame.clone();
    cv::GaussianBlur(processedFrame, processedFrame, cv::Size(5, 5), 0, 0);

    cvtColor(processedFrame, Edges, CV_BGR2GRAY);

    double CannyAccThresh = threshold(Edges,Edges,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

    double CannyThresh = .0001 * CannyAccThresh;

    //Canny(Edges,Edges,CannyThresh,CannyAccThresh);
    Laplacian( Edges, Edges, CV_8U, 5, 1, 0, cv::BORDER_DEFAULT );

    cvtColor(processedFrame, processedFrame, CV_BGR2HSV);

    if (flag == true)
    {

        // std::cout << "new image " << buffPosition<< " in buffer" << std::endl;

        split(processedFrame, HSV_planes);
        calcHist(&HSV_planes[0], 1, 0, mask, History[buffPosition].H_hist, 1, &histSizeH, &histRangeH, uniform, accumulate);
        calcHist(&HSV_planes[2], 1, 0, mask, History[buffPosition].V_hist, 1, &histSizeI, &histRangeI, uniform, accumulate);
        medianBlur(History[buffPosition].H_hist, History[buffPosition].H_hist, 5);
        medianBlur(History[buffPosition].V_hist, History[buffPosition].V_hist, 5);

        //where the sensor data is added for each
        History[buffPosition].valid = true;
        History[buffPosition].roll = system_Roll;
        History[buffPosition].yaw = system_Yaw;
        History[buffPosition].pitch = system_Pitch;

        buffPosition++;
        if(buffPosition == histBuffer)
        {
            buffPosition = 0;

        }
    }
    return true;
}

bool Avoidance::DetectObstacles()
{
    cv::Mat result_Hue(size_Y,size_X,CV_8UC1,cv::Scalar(255));
    cv::Mat result_Value(size_Y,size_X,CV_8UC1,cv::Scalar(255));

    for(int i = 0; i < histBuffer; i++)
    {

        int rows = 0;
        int cols = 0;
        int Hue;
        int V;


        if ((system_Yaw <= (History[i].yaw+5) && system_Yaw >= (History[i].yaw-5))&& History[i].valid == true )
        {


            while (rows < size_Y)
            {

                while (cols < size_X)
                {
                    cv::Vec3b intensity = processedFrame.at<cv::Vec3b>(rows, cols);
                    Hue = intensity.val[0];
                    V = intensity.val[2];
                    int histH =History[i].H_hist.at<int>(Hue);
                    int histV =History[i].V_hist.at<int>(V);

                    if (History[i].H_hist.at<int>(Hue) < threshold_hue)
                    {
                        mask_Hue[i].at<uchar>(rows, cols) = 255;
                    }

                    else
                    {
                        mask_Hue[i].at<uchar>(rows, cols) = 0;
                    }

                    if (History[i].V_hist.at<int>(V) < threshold_v)
                    {
                        mask_Value[i].at<uchar>(rows, cols) = 255;
                    }

                    else
                    {
                        mask_Value[i].at<uchar>(rows, cols) = 0;
                    }

                    cols = cols + 1;
                }
                cols = 0;
                rows = rows + 1;


            }

            bitwise_and(result_Hue, this -> mask_Hue[i], result_Hue);
            bitwise_and(result_Value, this ->mask_Value[i], result_Value);
        }
    }


    bitwise_or(result_Value, result_Hue, this ->Result);
    bitwise_or(this ->Result, Edges, this ->Result);
    morphologyEx( this->Result, this->Result, cv::MORPH_OPEN, this ->element ); //Only need with large, hi res pictures
    bitwise_or(this ->Result, Edges, this ->Result);

    return true;

}

bool Avoidance::fillBuffer()
{
    for(int i = 0; i < 2*histBuffer; i++)
    {

        if(!this->frameCapture())
        {
            return false;
        }
        if(!this->ProcessFrame(true))
        {
            return false;
        }
    }
    return true;

}





