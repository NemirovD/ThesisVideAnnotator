#include "vidcontroller.h"
#include "vidcontroller.h"

VidController::VidController(QObject *parent) :
    QThread(parent)
{
    stop = true;
    quit = false;
    capture = new cv::VideoCapture;
    draw_objects = false;
    mouseCallbackMode = MODE_NONE;
}

VidController::~VidController()
{
    mutex.lock();
    stop = true;
    capture->release();
    mutex.unlock();
    wait();
}
void VidController::play()
{if(isStopped()){stop = false;}}

void VidController::stopVid()
{stop = true;}

void VidController::exit()
{quit = true;}

bool VidController::loadVideo(std::string filename)
{
    exit();
    wait();
    quit = false;
    capture = new cv::VideoCapture;
    capture->open(filename);

    std::string fWithoutExt = ul::getFileNameWithoutExtension(filename);

    objectHandler = ul::ObjectInfoHandler(fWithoutExt+".xml");

    if(capture->isOpened())
    {
        this->framerate = (int) capture->get(CV_CAP_PROP_FPS);
        start(NormalPriority);
        return true;
    }
    else
    {
        return false;
    }
}

double VidController::getCurrentFrame() const
{return capture->get(CV_CAP_PROP_POS_FRAMES);}

double VidController::getNumberOfFrames() const
{return capture->get(CV_CAP_PROP_FRAME_COUNT);}

double VidController::getFrameRate() const
{return framerate;}

void VidController::setCurrentFrame( int frameNumber )
{capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);}

bool VidController::isStopped() const
{return stop;}

bool VidController::isOpened() const
{return capture->isOpened();}

void VidController::mouseDown(const QPoint &pt, const QSize &sz)
{
    switch(mouseCallbackMode)
    {
    case MODE_ADD_OBJECT:
    {
        AddObjectRectDrawer *t = new AddObjectRectDrawer(pt,sz,frame,getCurrentFrame());
        connect(t,
                SIGNAL(objectInfoCreated(ul::ObjectInfo)),
                this,
                SLOT(addObject(ul::ObjectInfo)));
        _rectDrawer = t;
        break;
    }
    default:
    case MODE_NONE:
        break;
    }
}

void VidController::mouseMove(const QPoint &pt, const QSize &sz)
{
    switch(mouseCallbackMode)
    {
    case MODE_ADD_OBJECT:
        _rectDrawer->updateRect(pt,sz);
        break;

    default:
    case MODE_NONE:
        break;
    }
}

void VidController::mouseUp(const QPoint &pt, const QSize &sz)
{
    switch(mouseCallbackMode)
    {
    case MODE_ADD_OBJECT:
        _rectDrawer->onMouseUp(pt,sz);
        break;

    default:
    case MODE_NONE:
        break;
    }
}

void VidController::addObject(ul::ObjectInfo oi)
{
    objectHandler.addObject(oi);
}

void VidController::run()
{
    int delay = 1000/framerate;
    while(!quit)
    {
        double start = ((double)clock())/CLOCKS_PER_SEC * 1000;

        if(!stop)
        {
            //Get the frame
            if(!capture->read(frame))
            {
                stop = true;
            }
            else
            {
                //if(draw_annotations)
                //   addObjectsToFrame();
                img = processImage(frame);
            }
        }
        if(!frame.empty())
        {
            emit processedImage(*img);

            if(stop)
                this->msleep(delay);
        }
        double currTime = ((double)clock())/CLOCKS_PER_SEC * 1000;
        long waitTime = (long)(start + 1000/framerate - currTime);

        if(waitTime > 0 && !stop)
            this->msleep(waitTime);
    }
}

cv::Point VidController::labelSizeToImageSize(const QPoint& pt,const QSize& sz)
{
    float xf = (pt.x()/(float)sz.width())*frame.size().width;
    float yf = (pt.y()/(float)sz.width())*frame.size().height;
    return cv::Point((int)xf,(int)yf);
}

QImage* VidController::processImage(cv::Mat frame)
{
    QImage * img;
    //3channel transfer
    if(frame.channels() == 3)
    {
        cv::cvtColor(frame,RGBframe, CV_BGR2RGB);
        img = new QImage((const unsigned char*)(RGBframe.data),
                         RGBframe.cols,
                         RGBframe.rows,
                         QImage::Format_RGB888);
    }
    //single channel transfer
    else
    {
        img = new QImage((const unsigned char*)(frame.data),
                         frame.cols,
                         frame.rows,
                         QImage::Format_Indexed8);
    }
    return img;
}

void VidController::addObjectsToFrame()
{

    std::vector<ul::ObjectInfo> objectsToDraw(objectHandler.getObjectsIn(getCurrentFrame()));

    for(unsigned int i = 0; i < objectsToDraw.size(); ++i)
    {
        cv::Rect r = objectsToDraw[i].location();
        cv::rectangle(frame,
                      r,
                      cv::Scalar::all(255),
                      4);

        cv::putText(frame,
                    objectsToDraw[i].name(),
                    cv::Point(r.x,r.y-10),
                    cv::FONT_HERSHEY_DUPLEX,
                    2,
                    cv::Scalar::all(255),
                    4);
    }
}
