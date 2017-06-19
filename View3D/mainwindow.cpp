#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // SIGNAL SLOT

    //qDebug() << "MainWindow Start !";

    ui->setupUi(this);

    ui->CheckADC->hide();

    WatchDog.singleShot(30000,this,SLOT(close()));

    RcvText(__FILE__" build at " __DATE__ " "  __TIME__ "",QColor(255,255,0));
    printf(__FILE__" build at " __DATE__ " "  __TIME__ "");

    RcvText("Start");

    MyDisplay.Write_yellow("Start\n"__DATE__ " "  __TIME__ );

    ui->SMA_Current->setValue(500);


    for(int i=0;i<NB_HEADSET;i++)
        mySMA_Headset[i] = new SMA_Headset();

    this->Init();

    //qDebug() << "MainWindow 1 !";

}

int MainWindow::Init()
{

    //qDebug() << "MainWindow Init !";
    mysma[0][0].Channel = SMA_CHANNEL_A;
    mysma[0][0].Pin     = 4;
    mysma[0][1].Channel = SMA_CHANNEL_D;
    mysma[0][1].Pin     = 0;
    mysma[0][2].Channel = SMA_CHANNEL_D;
    mysma[0][2].Pin     = 1;

    mysma[1][0].Channel = SMA_CHANNEL_A;
    mysma[1][0].Pin     = 5;
    mysma[1][1].Channel = SMA_CHANNEL_D;
    mysma[1][1].Pin     = 2;
    mysma[1][2].Channel = SMA_CHANNEL_D;
    mysma[1][2].Pin     = 3;

    mysma[2][0].Channel = SMA_CHANNEL_A;
    mysma[2][0].Pin     = 6;
    mysma[2][1].Channel = SMA_CHANNEL_D;
    mysma[2][1].Pin     = 4;
    mysma[2][2].Channel = SMA_CHANNEL_D;
    mysma[2][2].Pin     = 5;

    mysma[3][0].Channel = SMA_CHANNEL_A;
    mysma[3][0].Pin     = 7;
    mysma[3][1].Channel = SMA_CHANNEL_D;
    mysma[3][1].Pin     = 6;
    mysma[3][2].Channel = SMA_CHANNEL_D;
    mysma[3][2].Pin     = 7;


    for(int i=0;i<NB_HEADSET;i++)
    {
        mySMA_Headset[i]->init(mysma[i]);
        mySMA_Headset[i]->SetMAXCurrent(ui->SMA_Current->value());
    }
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

void MainWindow::closeEvent(QCloseEvent *event)
{


    qDebug()<<"Close event !!!";

    qDebug() << "close Camera";

    MyCamera.StopRUN();

    qDebug() << "close Headset";
    for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->~SMA_Headset();


    qDebug() << "close End";
   // MyCamera.~Camera();
    //MyCamera.~QObject();
    MyDisplay.kill();

    event->accept();
}

void MainWindow::ReturnToTextbox(int _ret)
{
    if(_ret == 0)
        RcvText("Return = "+QString::number(_ret),QColor(0,255,0));
    else
        RcvText("Return = "+QString::number(_ret),QColor(255,0,0));
}

void MainWindow::on_CheckAll_clicked()
{
    MyDisplay.Write_yellow("Check SMA\n\n");

    for (int i = 0 ; i < NB_HEADSET ; i++)
    {
        RcvText(mySMA_Headset[i]->isOKstring());
    }
}

void MainWindow::on_StartWebcam_clicked()
{
    MyDisplay.Write_yellow("Camera Starting\n\n");
    RcvText("Camera Starting");
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
    RcvText("Video Starting");
    MyDisplay.Write_yellow("Video Starting\n\n");
    ReturnToTextbox(MyCamera.StartVideo());
}

void MainWindow::on_Stop_clicked()
{
    MyCamera.StopRUN();
}

void MainWindow::on_Getdisparity_clicked()
{
    RcvText("Get One Disparity");
    ReturnToTextbox(MyCamera.getDisparity(0));
}

void MainWindow::on_Disparity_loop_clicked()
{
    MyDisplay.Write_yellow("Disparity loop\nStarting\n");

    RcvText("Disparity loop Starting");
    int ret = MyCamera.getDisparity(1);
    ReturnToTextbox(ret);

    MyDisplay.Write_yellow("Disparity loop\nReturn "+ QString::number(ret) + "\n");
}

void MainWindow::RcvText(QString _Text, QColor _Color)
{
    ui->textBrowser->setTextColor(_Color);
    ui->textBrowser->append(_Text);
    ui->textBrowser->setTextColor(QColor(200,200,200));
}

void MainWindow::on_CheckCurrent_clicked()
{
    RcvText("TestCurrent Starting");

    double ret = mySMA_Headset[0]->TestCurrent(0,1000);

    RcvText("Channel A Error = "+QString::number(ret),QColor(100,200,255));

    ret = mySMA_Headset[0]->TestCurrent(1,1000);

    RcvText("Channel D Error = "+QString::number(ret),QColor(100,200,255));

}

void MainWindow::RcvDistance(double *_Distance)
{
//    qDebug()<<"RCV Distance";
//    qDebug()<<_Distance[0];
//    qDebug()<<_Distance[1]*100;
//    qDebug()<<_Distance[2]*100;
//    qDebug()<<_Distance[3]*100;
    ui->progressBar->setValue((int)(_Distance[0]*100));
    ui->progressBar_2->setValue((int)(_Distance[1]*100));
    ui->progressBar_3->setValue((int)(_Distance[2]*100));
    ui->progressBar_4->setValue((int)(_Distance[3]*100));

    QString Data = "";

    for(int i=0;i<NB_HEADSET;i++)
        Data += QString::number(i) + "/4 = " + QString::number((int)(_Distance[i]*100)) + "% \n";

    //qDebug() << Data;

    MyDisplay.Write_blue(Data);

    if(ui->Command_SMA->checkState())
    {
        qDebug() << "Command SMA enable /!\\" ;

       RcvText("Command SMA enable /!\\ ",QColor(250,0,0));

       Signal_t Signal[NB_HEADSET];

       for(int i=0;i<NB_HEADSET;i++)
       {
           Signal[i].DuringTime    = 500;
           Signal[i].DutyCycle     = (int) 10*_Distance[i];
           Signal[i].Frequency     = 50;
       }

       for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->SetSignal(Signal[i]);
       for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->StartRUN();
    }
}

void MainWindow::on_CheckADC_clicked()
{

    RcvText("CheckADC Starting\n look at debug ^^",QColor(100,200,200));

    mySMA_Headset[0]->checkADC();
}

void MainWindow::on_SMA_Current_valueChanged(int arg1)
{
    for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->SetMAXCurrent(arg1);

    RcvText("MAX Current set to "+QString::number(arg1)+"mA",QColor(180,180,80));
}


void MainWindow::on_Command_SMA_stateChanged(int arg1)
{
    if(arg1)
    {
       RcvText("Command SMA enable (/!\\) ",QColor(250,0,0));
    }else
    {
        RcvText("Command SMA disable (^_^) ",QColor(0,250,0));
    }
}

void MainWindow::on_CheckSMAcmd_clicked()
{
    qDebug() << "Command SMA enable /!\\" ;

   RcvText("Command SMA enable /!\\ ",QColor(250,0,0));

   Signal_t Signal[NB_HEADSET];

   for(int i=0;i<NB_HEADSET;i++)
   {
       Signal[i].DuringTime    = 500;
       Signal[i].DutyCycle     = (int) 10;
       Signal[i].Frequency     = 50;
   }

   for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->SetSignal(Signal[i]);
   for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->StartRUN();

}

void MainWindow::on_I2C_clicked()
{
    //MyDisplay.Write_blue("123456789ABCDEFGHIJKL");
    MyDisplay.Write_blue("1\n2\n3\n4\n5\n6\n7\n");
}
