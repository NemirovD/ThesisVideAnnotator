#ifndef RECTDRAWER_H
#define RECTDRAWER_H

#include <QSize>
#include <QPoint>
#include <QObject>
#include <opencv2/opencv.hpp>
#include "utils.h"
#include "addobjectdialog.h"

class RectDrawer : public QObject
{
    Q_OBJECT
public:
    explicit RectDrawer(QObject *parent = 0);
    RectDrawer(const QPoint&, const QSize&,cv::Mat, QObject *parent = 0);
    cv::Mat updateRect(const QPoint&, const QSize&);
    virtual void onMouseUp(const QPoint&, const QSize&);
protected:
    cv::Point labelSizeToImageSize(const QPoint&,const QSize&);

    cv::Rect _rect;
    cv::Mat _frame;
    cv::Mat _tFrame;
};

class AddObjectRectDrawer : public RectDrawer
{
     Q_OBJECT
public:
    AddObjectRectDrawer(const QPoint&, const QSize&, cv::Mat, int);
    void onMouseUp(const QPoint &, const QSize &);
signals:
    void objectInfoCreated(const ul::ObjectInfo&);
public slots:
    void createObjectInfo(std::string,std::string);
private:
    AddObjectDialog *_addObjectDialog;
    cv::Mat _icon;
    int _frameNumber;
};

#endif // RECTDRAWER_H
