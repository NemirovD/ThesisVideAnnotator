#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T20:49:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThesisVideAnnotator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vidcontroller.cpp \
    utils.cpp \
    vidlabel.cpp \
    addobjectdialog.cpp \
    rectdrawer.cpp \
    objectinfolistwidget.cpp \
    objectlistwidget.cpp \
    editobjectdialog.cpp

HEADERS  += mainwindow.h \
    vidcontroller.h \
    utils.h \
    vidlabel.h \
    addobjectdialog.h \
    rectdrawer.h \
    objectinfolistwidget.h \
    objectlistwidget.h \
    editobjectdialog.h

FORMS    += mainwindow.ui \
    addobjectdialog.ui \
    objectinfolistwidget.ui \
    editobjectdialog.ui

win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_core240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_highgui240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_imgproc240

INCLUDEPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
DEPENDPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
