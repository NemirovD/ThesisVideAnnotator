#include "objectinfolistwidget.h"
#include "ui_objectinfolistwidget.h"

ObjectInfoListWidget::ObjectInfoListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectInfoListWidget)
{
    ui->setupUi(this);
}

ObjectInfoListWidget::ObjectInfoListWidget(cv::Mat icon,
                                           std::string name,
                                           std::string url,
                                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectInfoListWidget)
{
    ui->setupUi(this);
    ui->nameLabel->setText(QString(name.c_str()));
    ui->urlLabel->setText(QString(url.c_str()));

    QImage img = processImage(icon);

    ui->iconLabel->setAlignment(Qt::AlignCenter);
    ui->iconLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->iconLabel->size(),
                                                             Qt::KeepAspectRatio,
                                                             Qt::FastTransformation));
}

ObjectInfoListWidget::~ObjectInfoListWidget()
{
    delete ui;
}

QImage ObjectInfoListWidget::processImage(cv::Mat frame)
{
    QImage * img;
    cv::Mat RGBframe;
    //3channel transfer
    if(frame.channels() == 3)
    {
        cv::cvtColor(frame,RGBframe, CV_BGR2RGB);
        img = new QImage((const unsigned char*)(RGBframe.data),
                         RGBframe.cols,
                         RGBframe.rows,
                         RGBframe.step,
                         QImage::Format_RGB888);
    }
    //single channel transfer
    else
    {
        img = new QImage((const unsigned char*)(frame.data),
                         frame.cols,
                         frame.rows,
                         RGBframe.step,
                         QImage::Format_Indexed8);
    }

    QImage img2(*img);
    img2.detach();
    return img2;
}
