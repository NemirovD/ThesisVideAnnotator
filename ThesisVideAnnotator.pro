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
    objectinfo.cpp

HEADERS  += mainwindow.h \
    objectinfo.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_core240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_highgui240
win32: LIBS += -L$$PWD/../../../../../../Android_Project/opencv/build/x86/vc10/lib/ -lopencv_imgproc240

INCLUDEPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
DEPENDPATH += $$PWD/../../../../../../Android_Project/opencv/build/include
