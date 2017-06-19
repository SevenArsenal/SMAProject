#ifndef CAMERA_H
#define CAMERA_H

#include "tbb/tbb.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "commons.h"
#include "wiringPi.h"

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QColor>
#include <QElapsedTimer>
#include <QDateTime>

#include "opencv2/opencv.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "mythreadclass.h"

#include <iostream>

//using namespace tbb;
using namespace cv;

typedef enum {CAMERA_VIDEO,CAMERA_DISPARITY,CAMERA_STOP}Camera_Run_t;

class Camera : public QObject, public MyThreadClass
{
    Q_OBJECT

private:

    bool isCameraInit;

    bool isRunning;
    Camera_Run_t Camera_run;
    void InternalThreadEntry();

    cv::VideoCapture CamLeft;
    cv::VideoCapture CamRight;

    Mutex Camera_run_mutex;

    double Distance[4];

    int MaxValue;

    VideoWriter SaveDisparity;

    QElapsedTimer Timer;

    int ndisparities ;   /**< Range of disparity */
    int SADWindowSize ; /**< Size of the block window. Must be odd */

    Size CamSize;

public:
    explicit Camera(QObject *parent = 0);

    int StartCamera();
    int StartVideo();

    int StartRUN();
    void StopRUN();
    int getDisparity();
    int getDisparity(bool loop);
    int InitCamera();
signals:
    void SendText(QString _Text,QColor _Color);
    void SendVal(int);
    void SendDistance(double*);

public slots:

};

#endif // CAMERA_H
