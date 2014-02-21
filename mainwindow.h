#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include "utils.h"
#include "vidcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openVid();
    void onPlay();
    void updatePlayerUI(QImage img);

    void trackBarSliderPressed();
    void trackBarSliderReleased();
private:
    void updateLabelTime();

    std::string currentFileName;
    ul::FormatTime formatTime;
    VidController * vidController;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
