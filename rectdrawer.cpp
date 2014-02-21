#include "rectdrawer.h"

RectDrawer::RectDrawer(QObject *parent) :
    QObject(parent)
{
}

RectDrawer::RectDrawer(const QPoint & pt, const QSize & sz, cv::Mat frame, QObject *parent) :
    QObject(parent)
{
    _frame = frame;


    cv::Point cpt(labelSizeToImageSize(pt,sz));
    _rect = cv::Rect(cpt, cv::Size(0,0));
}

cv::Mat RectDrawer::updateRect(const QPoint &pt, const QSize &sz)
{
    _tFrame = _frame.clone();
    cv::Point cpt(labelSizeToImageSize(pt,sz));

    _rect.width = cpt.x - _rect.x;
    _rect.height = cpt.y - _rect.y;

    cv::rectangle(_tFrame, _rect, cv::Scalar::all(255),3);
    return _tFrame;
}

void RectDrawer::onMouseUp(const QPoint&, const QSize&)
{
}

cv::Point RectDrawer::labelSizeToImageSize(const QPoint& pt,const QSize& sz)
{
    float xf = (pt.x()/(float)sz.width())*_frame.size().width;
    float yf = (pt.y()/(float)sz.width())*_frame.size().height;

    return cv::Point((int)xf,(int)yf);
}

AddObjectRectDrawer::AddObjectRectDrawer(const QPoint & pt,
                                         const QSize & sz,
                                         cv::Mat frame,
                                         int frameNumber) :
    RectDrawer(pt,sz,frame)
{
    _frameNumber = frameNumber;
}

void AddObjectRectDrawer::onMouseUp(const QPoint &pt, const QSize &sz)
{
    _tFrame = _frame.clone();
    cv::Point cpt(labelSizeToImageSize(pt,sz));

    _rect.width = cpt.x - _rect.x;
    _rect.height = cpt.y - _rect.y;

    _icon = cv::Mat(_frame,_rect);

    cv::rectangle(_tFrame, _rect, cv::Scalar::all(255),3);
    if(_rect.area() > 0)
    {
        _addObjectDialog = new AddObjectDialog();
        connect(_addObjectDialog,
                SIGNAL(rectInfo(std::string,std::string)),
                this,
                SLOT(createObjectInfo(std::string,std::string)));
        _addObjectDialog->exec();
    }
}

void AddObjectRectDrawer::createObjectInfo(std::string name, std::string url)
{
    ul::ObjectInfo t(_icon, name, url, _frameNumber, _rect);
    emit objectInfoCreated(t);
}
