#-------------------------------------------------
#
# Project created by QtCreator 2018-06-20T14:00:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Newton_China_Potato_GC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
LIBS += -lopencv_nonfree
