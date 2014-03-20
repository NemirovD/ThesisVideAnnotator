#include "ObjectTracker.h"

//Assuming we have a rectangle and a frame

//need a function that handle the frame

//need a fucntion that can prepare the frame for processing

ObjectTracker::ObjectTracker(){
	this->prepVariables();
}

ObjectTracker::ObjectTracker(cv::Mat frame, cv::Rect objectROI)
{
	this->prepVariables();
	this->setObject(frame,objectROI);
}

ObjectTracker::~ObjectTracker()
{
    std::cout << "test" << std::endl;
}

void ObjectTracker::trackerIndex(int index)
{
    indexOfTracker = index;
}

int ObjectTracker::trackerIndex() const
{
    return indexOfTracker;
}

void ObjectTracker::prepVariables()
{
	this->valMin = 10;
	this->valMax = 256;
	this->satMin = 30;
	this->hsize = 16;
	this->hranges[0] = 0;
    this->hranges[1] = 180;
}

void ObjectTracker::setObject(cv::Mat frame, cv::Rect objectROI)
{
	this->trackWindow = objectROI;
	cv::Mat hsv = convertToHSV(frame);
	cv::Mat mask = makeMask(hsv);
	cv::Mat hue = prepHue(hsv);

	cv::Mat roi(hue,objectROI);
	cv::Mat maskroi(mask, objectROI);

    const float* phranges = this->hranges;

	cv::Mat hist;
	cv::calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
	normalize(hist,hist, 0, 255, CV_MINMAX);
	this->objectHist = hist;
}

cv::Rect ObjectTracker::getObject(cv::Mat frame)
{
	cv::Mat hsv = convertToHSV(frame);
	cv::Mat mask = makeMask(hsv);
	cv::Mat hue = prepHue(hsv);

  const float* phranges = this->hranges;
	cv::Mat backproj;
	calcBackProject(&hue, 1, 0 , this->objectHist, backproj, &phranges);
	backproj &= mask;
	cv::RotatedRect trackBox =  CamShift(backproj, 
								this->trackWindow,
                                cv::TermCriteria( CV_TERMCRIT_EPS | 
                                				CV_TERMCRIT_ITER, 
                                				10, 
                                				1 )
                                );
	if( trackWindow.area() <= 1 )
    {
        int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
        trackWindow = cv::Rect(trackWindow.x - r, trackWindow.y - r,
                           trackWindow.x + r, trackWindow.y + r) &
        cv::Rect(0, 0, cols, rows);
    }

    if(trackBox.boundingRect().area() < 1){
        return cv::Rect();
    }

    return trackBox.boundingRect();
}

cv::Mat ObjectTracker::prepHue(cv::Mat hsv)
{
	cv::Mat hue;
	int ch[] = {0,0};
	hue.create(hsv.size(),hsv.depth());
	cv::mixChannels(&hsv, 1, &hue, 1, ch , 1);
	return hue;
}

cv::Mat ObjectTracker::makeMask(cv::Mat hsv)
{
	cv::Mat mask;
	cv::inRange(hsv,
		cv::Scalar(0, this->satMin, this->valMin),
		cv::Scalar(180, 256, this->valMax),
		mask);
	return mask;
}

cv::Mat ObjectTracker::convertToHSV(cv::Mat frame)
{
	cv::Mat hsv;
	cv::cvtColor(frame, hsv, CV_BGR2HSV);
	return hsv;
}
