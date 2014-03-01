#include "objecttracker.h"

ObjectTracker::ObjectTracker()
{
    prepVars();
}

ObjectTracker::ObjectTracker(cv::Mat frame, cv::Rect objectROI)
{
    prepVars();
    setObject(frame,objectROI);
}

void ObjectTracker::trackerIndex(int index)
{_indexOfTracker = index;}

int ObjectTracker::trackerIndex() const
{return _indexOfTracker;}

void ObjectTracker::setObject(cv::Mat frame, cv::Rect roi)
{
    _trackWindow = roi;
    cv::Mat hsv(convertToHSV(frame));
    cv::Mat hsvROI(hsv,roi);

    const float* ranges[2];
    ranges[0] = _hueRanges;
    ranges[1] = _satRanges;

    int histSize[] = {_hueSize,
                      _satSize};
    const int* pHistSize = histSize;

    cv::Mat hist;
    cv::calcHist(&hsvROI, 1, _channels, cv::Mat(), hist, 2, pHistSize, ranges);
    normalize(hist, hist, 0, 256, CV_MINMAX);

    _objectHist = hist;
}

cv::Rect ObjectTracker::getObject(cv::Mat frame)
{
    cv::Mat hsv(convertToHSV(frame));

    const float* ranges[2];
    ranges[0] = _hueRanges;
    ranges[1] = _satRanges;

    cv::Mat backproj;

    cv::calcBackProject(&hsv, 1 ,_channels, _objectHist, backproj, ranges);

    if(backproj.size().width <= 0 || backproj.size().height <= 0)
        return cv::Rect();

    cv::Mat binbackproj = backproj.clone();
    cv::threshold(binbackproj,binbackproj,70,255,cv::THRESH_TOZERO);

    cv::RotatedRect trackBox =  CamShift(binbackproj,
                                _trackWindow,
                                cv::TermCriteria( CV_TERMCRIT_EPS |
                                                CV_TERMCRIT_ITER,
                                                10,
                                                1 )
                                );
    if( _trackWindow.area() <= 1 )
    {
        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
        _trackWindow = cv::Rect(_trackWindow.x - r, _trackWindow.y - r,
                           _trackWindow.x + r, _trackWindow.y + r) &
        cv::Rect(0, 0, cols, rows);
    }
    return trackBox.boundingRect();
}

void ObjectTracker::prepVars()
{
    _hueSize = 32;
    _satSize = 16;
    _hueRanges[0] = 0;
    _hueRanges[1] = 179;
    _satRanges[0] = 0;
    _satRanges[1] = 255;
    _channels[0] = 0;
    _channels[1] = 1;
}

cv::Mat ObjectTracker::convertToHSV(cv::Mat frame)
{
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, CV_BGR2HSV);
    return hsv;
}
