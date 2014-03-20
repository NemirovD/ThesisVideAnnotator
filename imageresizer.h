#ifndef IMAGERESIZER_H
#define IMAGERESIZER_H
#include <opencv2/opencv.hpp>

class ImageResizer
{
public:
    ImageResizer();
    ImageResizer(cv::Mat inputMat, cv::Size destinationSize);

    cv::Rect toDestRect(cv::Rect r);
    cv::Rect toInRect(cv::Rect r);

    cv::Mat toDestMat(cv::Mat m);
    cv::Mat toInMat(cv::Mat m);
private:
    cv::Size _dSize;
    cv::Size _iSize;
};

#endif // IMAGERESIZER_H
