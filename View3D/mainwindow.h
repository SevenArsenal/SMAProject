#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <QColor>
#include <QCloseEvent>
#include <QTimer>

#include "commons.h"
#include "sma_headset.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "opencv2/opencv.hpp"
#include "camera.h"

#include "oleddisplay.h"



/*
#include <signal.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
*/

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
private slots:
    void on_CheckAll_clicked();

    void on_StartWebcam_clicked();

    void on_Clear_clicked();

    void on_GPIO_OFF_clicked();

    void on_StartVideo_clicked();

    void on_Stop_clicked();

    void on_Getdisparity_clicked();

    void on_Disparity_loop_clicked();

    void RcvText(QString _Text,QColor _Color = QColor(200,200,200));

    void ReturnToTextbox(int _ret);

    void on_CheckCurrent_clicked();

    void RcvDistance(double* _Distance);

    void on_CheckADC_clicked();

    void on_SMA_Current_valueChanged(int arg1);

    void closeEvent(QCloseEvent *event);

    void on_Command_SMA_stateChanged(int arg1);

    void on_CheckSMAcmd_clicked();

    void on_I2C_clicked();

private:

    QTimer WatchDog;

    Ui::MainWindow *ui;
    SMA_Headset *mySMA_Headset[NB_HEADSET];
    sma mysma[NB_HEADSET][NB_SMA_PER_HEADSET];
    Camera MyCamera;
    OLEDdisplay MyDisplay;

    int Init();
};

#endif // MAINWINDOW_H
