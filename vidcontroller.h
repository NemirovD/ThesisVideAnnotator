#ifndef VIDCONTROLLER_H
#define VIDCONTROLLER_H

#include <ctime>
#include <QMutex>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include "utils.h"
#include "rectdrawer.h"

class VidController : public QThread
{
    Q_OBJECT
public:
    explicit VidController(QObject *parent = 0);
    ~VidController();

    //static variables
    static const int MODE_NONE = 0;
    static const int MODE_ADD_OBJECT = 1;
    static const int MODE_MOVE_RECT = 2;

    //video control
    void play();
    void stopVid();
    void exit();
    bool loadVideo(std::string filename);

    //Video getters
    double getFrameRate() const;
    double getCurrentFrame() const;
    double getNumberOfFrames() const;

    //Video setters
    void setCurrentFrame( int frameNumber );

    //Video State Checkers
    bool isStopped() const;
    bool isOpened() const;

signals:
    void processedImage(const QImage& image);
public slots:
    void mouseDown(const QPoint&, const QSize&);
    void mouseMove(const QPoint&, const QSize&);
    void mouseUp(const QPoint&, const QSize&);
    void addObject(ul::ObjectInfo);
protected:
    void run();

private:
    //helper functions
    cv::Point labelSizeToImageSize(const QPoint&,const QSize&);
    QImage* processImage(cv::Mat frame);
    void addObjectsToFrame();
    //void addObjectToFrame(ul::ObjectInfo object);

    bool quit;
    bool stop;
    bool draw_objects;
    QMutex mutex;

    int framerate;
    QImage* img;
    cv::Mat frame;
    cv::Mat RGBframe;
    cv::VideoCapture* capture;
    ul::ObjectInfoHandler objectHandler;
    int mouseCallbackMode;
    RectDrawer *_rectDrawer;
};

#endif // VIDCONTROLLER_H