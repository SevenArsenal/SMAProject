#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T13:20:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = View3D
TEMPLATE = app

INCLUDEPATH += /usr/local/include\
                /usr/local/include/opencv
LIBS += -L/usr/local/lib \
        -lwiringPi \
        -lpthread\
        -lopencv_core\
        -lopencv_imgcodecs\
        -lopencv_highgui\
        -lopencv_videoio\
        -lopencv_calib3d\
        -lopencv_imgproc




SOURCES += main.cpp\
        mainwindow.cpp \
    MCP3208.cpp \
    MCP4922.cpp \
    sma.cpp \
    sma_headset.cpp \
    camera.cpp

HEADERS  += mainwindow.h \
    commons.h \
    MCP3208.h \
    MCP4922.h \
    sma.h \
    sma_headset.h \
    camera.h \
    mythreadclass.h

FORMS    += mainwindow.ui
