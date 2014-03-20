#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->objectSideBar->setVisible(false);
    ui->trackerSideBar->setVisible(false);

    vidController = new VidController(this);

    editObjectDialog = new EditObjectDialog(this);

    //oh my god this list is getting tedious...
    //I wish there was a better way
    connect(vidController,
            SIGNAL(processedImage(QImage)),
            this,
            SLOT(updatePlayerUI(QImage)));

    //Mouse connections
    connect(ui->vidLabel,
            SIGNAL(mousePressed(const QPoint&, const QSize&)),
            vidController,
            SLOT(mouseDown(const QPoint&,const QSize&)));

    connect(ui->vidLabel,
            SIGNAL(mouseMoved(const QPoint&, const QSize&)),
            vidController,
            SLOT(mouseMove(const QPoint&,const QSize&)));

    connect(ui->vidLabel,
            SIGNAL(mouseReleased(const QPoint&, const QSize&)),
            vidController,
            SLOT(mouseUp(const QPoint&,const QSize&)));

    //updateing list
    qRegisterMetaType<QVector<ul::ObjectInfo>>("QVector<ul::ObjectInfo>");
    connect(vidController,
            SIGNAL(objectListChanged(const QVector<ul::ObjectInfo>)),
            ui->objectList,
            SLOT(updateObjectList(const QVector<ul::ObjectInfo>)));

    connect(ui->objectList,
            SIGNAL(currentRowChanged(int)),
            editObjectDialog,
            SLOT(activeObject(int)));

    connect(vidController,
            SIGNAL(objectListChanged(const QVector<ul::ObjectInfo>)),
            editObjectDialog,
            SLOT(updateObjectList(const QVector<ul::ObjectInfo>)));

    connect(editObjectDialog,
            SIGNAL(updatedObjectInfo(int,ul::ObjectInfo)),
            vidController,
            SLOT(editObject(int,ul::ObjectInfo)));

    connect(vidController,
            SIGNAL(videoEnded()),
            this,
            SLOT(onVidEnding()));

    connect(ui->objectList,
            SIGNAL(currentRowChanged(int)),
            vidController,
            SLOT(currentObjectListIndex(int)));

    connect(vidController,
            SIGNAL(objectListChanged(const QVector<ul::ObjectInfo>)),
            ui->trackerList,
            SLOT(updateObjectList(const QVector<ul::ObjectInfo>)));

    connect(ui->trackerList,
            SIGNAL(currentRowChanged(int)),
            vidController,
            SLOT(currentTrackerListIndex(int)));

    connect(vidController,
            SIGNAL(trackerListChanged(QVector<ObjectTracker>)),
            ui->trackerList,
            SLOT(updateTrackerList(const QVector<ObjectTracker>)));

    connect(ui->deleteTrackerButton,
            SIGNAL(clicked()),
            vidController,
            SLOT(deleteTracker()));
}

MainWindow::~MainWindow()
{
    vidController->exit();
    vidController->wait();
    delete ui;
    qApp->quit();
}

void MainWindow::openVid()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"), ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4"));
    QFileInfo name = filename;

    if(!filename.isEmpty())
    {
        if(!vidController->loadVideo(filename.toStdString()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else
        {

            currentFileName = ul::getFileNameWithoutExtension(filename.toStdString());
            ui->trackBarSlider->setEnabled(true);
            this->setWindowTitle(name.fileName());
            ui->trackBarSlider->setMaximum(vidController->getNumberOfFrames());

            //if OpenCV reports the wrong framerate then:
            //game over man, it's over, its over
            int totalSecs = (int)vidController->getNumberOfFrames()/
                    (int)vidController->getFrameRate();

            formatTime.totalTime(totalSecs);
            ui->timeLabel->setText(tr(formatTime.getFormattedTime().c_str()));
        }
    }
}

void MainWindow::onPlay()
{
    if(vidController->isOpened())
    {
        if(vidController->isStopped())
        {
            vidController->play();
            ui->playpauseButton->setText("Pause");
        }
        else
        {
            vidController->stopVid();
            ui->playpauseButton->setText("Play");
        }
    }
    else
    {
        openVid();
    }
}

void MainWindow::updatePlayerUI(QImage img)
{
    if(!img.isNull())
    {
        ui->vidLabel->setAlignment(Qt::AlignCenter);
        ui->vidLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->vidLabel->size(),
                                                                Qt::KeepAspectRatio,
                                                                Qt::FastTransformation));
        if(!vidController->isStopped()){
            ui->trackBarSlider->setValue(vidController->getCurrentFrame());
            this->updateLabelTime();
        }
    }
}

void MainWindow::trackBarSliderPressed()
{
    vidController->stopVid();
}

void MainWindow::trackBarSliderReleased()
{
    vidController->setCurrentFrame(ui->trackBarSlider->value());
    this->updateLabelTime();

    vidController->play();
}

void MainWindow::setMouseCallbackAddObject(bool toggled)
{
    if(toggled)
    {
        vidController->setMouseCallbackMode(VidController::MODE_ADD_OBJECT);
        if(ui->moveRectButton->isChecked() ||
                ui->addTrackerButton->isChecked())
        {
            bool oldstate = ui->moveRectButton->blockSignals(true);
            ui->moveRectButton->setChecked(false);
            ui->moveRectButton->blockSignals(oldstate);

            oldstate = ui->addTrackerButton->blockSignals(true);
            ui->addTrackerButton->setChecked(false);
            ui->addTrackerButton->blockSignals(oldstate);
        }
    }
    else
    {
        vidController->setMouseCallbackMode(VidController::MODE_NONE);
    }
}

void MainWindow::setMouseCallbackMoveRect(bool toggled)
{
    if(toggled)
    {
        vidController->setMouseCallbackMode(VidController::MODE_MOVE_RECT);
        if(ui->createObjectButton->isChecked() ||
                ui->addTrackerButton->isChecked())
        {
            bool oldstate = ui->createObjectButton->blockSignals(true);
            ui->createObjectButton->setChecked(false);
            ui->createObjectButton->blockSignals(oldstate);

            oldstate = ui->addTrackerButton->blockSignals(true);
            ui->addTrackerButton->setChecked(false);
            ui->addTrackerButton->blockSignals(oldstate);
        }
    }
    else
    {
        vidController->setMouseCallbackMode(VidController::MODE_NONE);
    }
}

void MainWindow::setMouseCallbackAddTracker(bool toggled)
{
    if(toggled)
    {
        vidController->setMouseCallbackMode(VidController::MODE_ADD_TRACKER);
        if(ui->createObjectButton->isChecked() ||
                ui->moveRectButton->isChecked())
        {
            std::cout << "test" << std::endl;
            bool oldstate = ui->createObjectButton->blockSignals(true);
            ui->createObjectButton->setChecked(false);
            ui->createObjectButton->blockSignals(oldstate);

            oldstate = ui->moveRectButton->blockSignals(true);
            ui->moveRectButton->setChecked(false);
            ui->moveRectButton->blockSignals(oldstate);
        }
    }
    else
    {
        vidController->setMouseCallbackMode(VidController::MODE_NONE);
    }
}

void MainWindow::showEditObjectDialog()
{
    if(!(editObjectDialog->objectIndex() < 0))
    {
        editObjectDialog->exec();
    }
}

void MainWindow::onItemDoubleClick(QModelIndex mi)
{
    vidController->showObject(mi.row());
}

void MainWindow::showObjects(bool toggled)
{
    vidController->showObjects(toggled);
}

void MainWindow::onVidEnding()
{
    ui->objectSideBar->setVisible(true);
}

void MainWindow::updateLabelTime()
{
    ui->timeLabel->setText(tr(formatTime.getFormattedTime((int)vidController->getCurrentFrame()
                                                   /(int)vidController->getFrameRate()).c_str()));
}
