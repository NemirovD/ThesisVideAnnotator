#ifndef OBJECT_TRACKER_H
#define OBJECT_TRACKER_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctype.h>

class ObjectTracker
{
public:
	ObjectTracker();
	ObjectTracker(cv::Mat frame, cv::Rect objectROI);
    ~ObjectTracker();

    void trackerIndex(int);
    int trackerIndex() const;

	void setObject(cv::Mat frame, cv::Rect objectROI);
    cv::Rect getObject(cv::Mat frame);

private:
	void prepVariables();
	cv::Mat prepHue(cv::Mat hsv);
	cv::Mat makeMask(cv::Mat hsv);
	cv::Mat convertToHSV(cv::Mat frame);

    int indexOfTracker;

	int hsize;
	int valMin;
	int valMax;
	int satMin;
	float hranges[2];

	cv::Mat objectROI;
	cv::Mat objectMask;
	cv::Mat objectHist;
	cv::Rect trackWindow;
	cv::RotatedRect ObjectLocation;
};

#endif
