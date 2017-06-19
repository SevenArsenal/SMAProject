#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // SIGNAL SLOT

    //qDebug() << "MainWindow Start !";

    ui->setupUi(this);

    ui->textBrowser->setTextColor(QColor(255,255,0));
    ui->textBrowser->append(__FILE__" build at " __DATE__ " "  __TIME__ "");
    printf(__FILE__" build at " __DATE__ " "  __TIME__ "");

    ui->textBrowser->setTextColor(QColor(200,200,200));
    ui->textBrowser->append("Start");


    for(int i=0;i<NB_HEADSET;i++)
        mySMA_Headset[i] = new SMA_Headset();

    this->Init();

    //qDebug() << "MainWindow 1 !";

}

int MainWindow::Init()
{

    //qDebug() << "MainWindow Init !";
    mysma[0][0].Channel = SMA_CHANNEL_A;
    mysma[0][0].Pin     = 1;
    mysma[0][1].Channel = SMA_CHANNEL_D;
    mysma[0][1].Pin     = 1;
    mysma[0][2].Channel = SMA_CHANNEL_D;
    mysma[0][2].Pin     = 2;

    mysma[1][0].Channel = SMA_CHANNEL_A;
    mysma[1][0].Pin     = 2;
    mysma[1][1].Channel = SMA_CHANNEL_D;
    mysma[1][1].Pin     = 3;
    mysma[1][2].Channel = SMA_CHANNEL_D;
    mysma[1][2].Pin     = 4;

    mysma[2][0].Channel = SMA_CHANNEL_A;
    mysma[2][0].Pin     = 3;
    mysma[2][1].Channel = SMA_CHANNEL_D;
    mysma[2][1].Pin     = 5;
    mysma[2][2].Channel = SMA_CHANNEL_D;
    mysma[2][2].Pin     = 6;

    mysma[3][0].Channel = SMA_CHANNEL_A;
    mysma[3][0].Pin     = 4;
    mysma[3][1].Channel = SMA_CHANNEL_D;
    mysma[3][1].Pin     = 7;
    mysma[3][2].Channel = SMA_CHANNEL_D;
    mysma[3][2].Pin     = 8;


    for(int i=0;i<NB_HEADSET;i++)
        mySMA_Headset[i]->init(mysma[i]);

    QObject::connect(&MyCamera,SIGNAL(SendText(QString,QColor)),this,SLOT(RcvText(QString,QColor)));

    QObject::connect(&MyCamera,SIGNAL(SendVal(int)),this,SLOT(ReturnToTextbox(int)));

    QObject::connect(&MyCamera,SIGNAL(SendDistance(double*)),this,SLOT(RcvDistance(double*)));

    return 0;
}

MainWindow::~MainWindow()
{
    ui->centralWidget->close();
    delete ui;
}

void MainWindow::ReturnToTextbox(int _ret)
{
    if(_ret == 0)
        ui->textBrowser->setTextColor(QColor(0,255,0));
    else
        ui->textBrowser->setTextColor(QColor(255,0,0));

    ui->textBrowser->append("Return = "+QString::number(_ret));

    ui->textBrowser->setTextColor(QColor(200,200,200));
}

void MainWindow::on_CheckAll_clicked()
{
    for (int i = 0 ; i < NB_HEADSET ; i++)
    {
        ui->textBrowser->append(mySMA_Headset[i]->isOKstring());
    }
}

void MainWindow::on_StartWebcam_clicked()
{
    ui->textBrowser->append("Camera Starting");
    ReturnToTextbox(MyCamera.StartCamera());
}

void MainWindow::on_Clear_clicked()
{
    ui->textBrowser->clear();
}


void MainWindow::on_GPIO_OFF_clicked()
{
    for(int i =0;i<NB_HEADSET;i++)
    {
        mySMA_Headset[i]->set(0);
    }
}

void MainWindow::on_StartVideo_clicked()
{
    ui->textBrowser->append("Video Starting");
    ReturnToTextbox(MyCamera.StartVideo());
}

void MainWindow::on_Stop_clicked()
{
    MyCamera.StopRUN();
}

void MainWindow::on_Getdisparity_clicked()
{

    ui->textBrowser->append("Get One Disparity");
    ReturnToTextbox(MyCamera.getDisparity(0));
}

void MainWindow::on_Disparity_loop_clicked()
{

    ui->textBrowser->append("Disparity loop Starting");
    ReturnToTextbox(MyCamera.getDisparity(1));
}

void MainWindow::RcvText(QString _Text, QColor _Color)
{
    ui->textBrowser->setTextColor(_Color);
    ui->textBrowser->append(_Text);
    ui->textBrowser->setTextColor(QColor(200,200,200));
}

void MainWindow::on_CheckCurrent_clicked()
{
    ui->textBrowser->append("TestCurrent Starting");

    double ret = mySMA_Headset[0]->TestCurrent(1,1000);

    ui->textBrowser->setTextColor(QColor(100,200,255));
    ui->textBrowser->append("Channel A Error = "+QString::number(ret));

    ret = mySMA_Headset[0]->TestCurrent(0,1000);

    ui->textBrowser->setTextColor(QColor(100,200,255));
    ui->textBrowser->append("Channel D Error = "+QString::number(ret));

    ui->textBrowser->setTextColor(QColor(200,200,200));
}

void MainWindow::RcvDistance(double *_Distance)
{
//    qDebug()<<"RCV Distance";
//    qDebug()<<_Distance[0];
//    qDebug()<<_Distance[1]*100;
//    qDebug()<<_Distance[2]*100;
//    qDebug()<<_Distance[3]*100;
    ui->progressBar->setValue((int)(_Distance[0]*100));
    ui->progressBar->show();
    ui->progressBar_2->setValue((int)(_Distance[1]*100));
    ui->progressBar_3->setValue((int)(_Distance[2]*100));
    ui->progressBar_4->setValue((int)(_Distance[3]*100));
}
