#include "imageresizer.h"

ImageResizer::ImageResizer(){}

ImageResizer::ImageResizer(cv::Mat inputMat,cv::Size destinationSize)
{
    _iSize = inputMat.size();
    _dSize = destinationSize;
}

//equation iF * dS/iS = dF
//for toDestRect
cv::Rect ImageResizer::toDestRect(cv::Rect r)
{
    cv::Rect ret;
    ret.x = (int)((_dSize.width / (float)_iSize.width) * r.x);
    ret.y = (int)((_dSize.height / (float)_iSize.height) * r.y);
    ret.width = (int)((_dSize.width / (float)_iSize.width) * r.width);
    ret.height = (int)((_dSize.height / (float)_iSize.height) * r.height);
    return ret;
}

//equation dF * iS/dS = iF
//for toDestRect
cv::Rect ImageResizer::toInRect(cv::Rect r)
{
    cv::Rect ret;
    ret.x = (int)((_iSize.width / (float)_dSize.width) * r.x);
    ret.y = (int)((_iSize.height / (float)_dSize.height) * r.y);
    ret.width = (int)((_iSize.width / (float)_dSize.width) * r.width);
    ret.height = (int)((_iSize.height / (float)_dSize.height) * r.height);
    return ret;
}

cv::Mat ImageResizer::toDestMat(cv::Mat m)
{
    cv::Mat ret;
    cv::resize(m, ret, _dSize);
    return ret;
}

cv::Mat ImageResizer::toInMat(cv::Mat m)
{
    cv::Mat ret;
    cv::resize(m, ret, _iSize);
    return ret;
}

/*
cv::Mat ImageResizer::resize(cv::Mat m)
{
    cv::Mat ret(_destinationSize,CV_8UC3);

    cv::resize(m,ret,_destinationSize);
    return ret;
}
*/
