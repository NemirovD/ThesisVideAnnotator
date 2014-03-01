#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H

#include <opencv2/opencv.hpp>
#include <QVector>

class ObjectTracker
{
public:
    ObjectTracker();
    ObjectTracker(cv::Mat frame, cv::Rect objectROI);

    void trackerIndex(int);
    int trackerIndex() const;

    //setters
    void setObject(cv::Mat frame,cv::Rect roi);
    cv::Rect getObject(cv::Mat frame);
private:
    void prepVars();
    cv::Mat convertToHSV(cv::Mat frame);

    int _indexOfTracker;
    int _hueSize;
    int _satSize;
    int _channels[2];
    float _hueRanges[2];
    float _satRanges[2];
    cv::Mat _objectHist;
    cv::Rect _trackWindow;
};
#endif // OBJECTTRACKER_H
