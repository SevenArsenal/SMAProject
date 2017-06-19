#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // SIGNAL SLOT

    //qDebug() << "MainWindow Start !";

    ui->setupUi(this);

    WatchDog.singleShot(600000,this,SLOT(close()));

    RcvText(__FILE__" build at " __DATE__ " "  __TIME__ "",QColor(255,255,0));
    printf(__FILE__" build at " __DATE__ " "  __TIME__ "\n");

    printf("Opencv  = %s\n",CV_VERSION);

    RcvText("Start");

    MyDisplay.Write_yellow("Start\n"__DATE__ " "  __TIME__ );

    ui->SMA_Current->setValue(500);

    for(int i=0;i<NB_HEADSET;i++)
        mySMA_Headset[i] = new SMA_Headset();
    
    
    for(int i=0;i<NB_HEADSET;i++)
        mySMA_Headset[i]->SetMAXCurrent(ui->SMA_Current->value());

    this->Init();

    //qDebug() << "MainWindow 1 !";

}

int MainWindow::Init()
{

    //qDebug() << "MainWindow Init !";
    mysma[0][0].Channel = SMA_CHANNEL_A;
    mysma[0][0].Pin     = 0;
    mysma[0][1].Channel = SMA_CHANNEL_D;
    mysma[0][1].Pin     = 0;
    mysma[0][2].Channel = SMA_CHANNEL_D;
    mysma[0][2].Pin     = 1;

    mysma[1][0].Channel = SMA_CHANNEL_A;
    mysma[1][0].Pin     = 1;
    mysma[1][1].Channel = SMA_CHANNEL_D;
    mysma[1][1].Pin     = 2;
    mysma[1][2].Channel = SMA_CHANNEL_D;
    mysma[1][2].Pin     = 3;

    mysma[2][0].Channel = SMA_CHANNEL_A;
    mysma[2][0].Pin     = 2;
    mysma[2][1].Channel = SMA_CHANNEL_D;
    mysma[2][1].Pin     = 4;
    mysma[2][2].Channel = SMA_CHANNEL_D;
    mysma[2][2].Pin     = 5;

    mysma[3][0].Channel = SMA_CHANNEL_A;
    mysma[3][0].Pin     = 3;
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

    connect(ui->actionADC_Command,SIGNAL(triggered()),this,SLOT(ADC_sma()));
    connect(ui->actionAll_SMA,SIGNAL(triggered()),this,SLOT(CheckAll()));
    connect(ui->actionCommand_SMA,SIGNAL(triggered()),this,SLOT(CheckSMAcmd()));
    connect(ui->actionCurrent_Calibration,SIGNAL(triggered()),this,SLOT(CheckCurrent()));
    connect(ui->actionSinus,SIGNAL(triggered()),this,SLOT(SMA_Sinus()));

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

void MainWindow::CheckAll()
{
    MyDisplay.Write_yellow("Check SMA\n\n");

    for (int i = 0 ; i < NB_HEADSET ; i++)
    {
        QString Ret = mySMA_Headset[i]->isOKstring();

        RcvText(Ret);
    }

    DisplaySmaState();
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

void MainWindow::CheckCurrent()
{
    RcvText("TestCurrent Starting");

    double ret = mySMA_Headset[0]->TestCurrent(0,1000);

    RcvText("Channel A Error = "+QString::number(ret),QColor(100,200,255));

    ret = mySMA_Headset[0]->TestCurrent(1,1000);

    RcvText("Channel D Error = "+QString::number(ret),QColor(100,200,255));

}

void MainWindow::RcvDistance(double *_Distance,unsigned int _DuringTime)
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
           Signal[i].DuringTime    = _DuringTime;
           Signal[i].DutyCycle     = (int) 10*_Distance[i];
           Signal[i].Frequency     = 50;
       }

       for(int i=0;i<NB_HEADSET;i++) mySMA_Headset[i]->SetSignal(Signal[i]);
       for(int i=0;i<NB_HEADSET;i++) {int ret = mySMA_Headset[i]->StartRUN();
           //qDebug() << "StartRUN" << ret ;
       }
    }
}

void MainWindow::CheckADC()
{

    RcvText("CheckADC Starting\n look at debug ^^",QColor(100,200,200));

    mySMA_Headset[0]->checkADC(); // ADC SAM cmd with log
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

void MainWindow::CheckSMAcmd()
{
    qDebug() << "Command SMA enable /!\\" ;

    ui->Command_SMA->setCheckState(Qt::Checked);

   RcvText("Command SMA enable /!\\ ",QColor(250,0,0));
   RcvText("MAX Current set to "+QString::number(mySMA_Headset[0]->getMaxCurrent())+"mA",QColor(180,180,80));

   MyDisplay.Write_yellow("Check SMA cmd\n\n");

   double Distance[4]  ={0};


   for(int i=0;i<NB_HEADSET;i++)
   {
       for(int j =0;j<4;j++)Distance[j] = 0 ;

       Distance[i%4] = 1;

       RcvDistance(Distance);

       delay(500);
   }
   for(int j =0;j<4;j++)Distance[j] = 0 ;
   RcvDistance(Distance);

   delay(350);

   for(int i=0;i<NB_HEADSET;i++)
   {
       for(int j =0;j<4;j++)Distance[j] = 0 ;

       Distance[3-(i%4)] = 1;

       RcvDistance(Distance);

       delay(500);
   }


for(int j =0;j<4;j++)Distance[j] = 0 ;
RcvDistance(Distance);
ui->Command_SMA->setCheckState(Qt::Unchecked);

}

void MainWindow::on_I2C_clicked()
{
    //MyDisplay.Write_blue("123456789ABCDEFGHIJKL");
    MyDisplay.Write_blue("123456789ABCDEFGHIJK\n2 .\n3  .\n4   .\n5    .\n6     .\n7       .\n");
    //MyDisplay.Write_blue("123456789ABCDEFGHIJK\n");
    //MyDisplay.Write_blue("2 .\n3  .\n4   .\n5    .\n6     .\n7      .\n");
}

void MainWindow::DisplaySmaState()
{
    MyDisplay.clear_blue();
    MyDisplay.Write_yellow("Check SMA\n\n");
    int NB_sma_ok =0;

    for (int i = 0 ; i < NB_HEADSET ; i++)
    {
        int SMA[3] = {0};
        for(int j = 0;j<NB_SMA_PER_HEADSET;j++)
        {
            SMA[j] = mySMA_Headset[i]->isOK(j);
            NB_sma_ok += SMA[j];
        }

        MyDisplay.Headset(i,SMA);
    }
    MyDisplay.Write_yellow("Check SMA\nNB of sma = "+QString::number(NB_sma_ok)+ "\n");
}

void MainWindow::on_I2C_sma_clicked()
{
    //DisplaySmaState();
    int SMA[3] = {0,1,0};
    MyDisplay.Headset(1,SMA);
}

void MainWindow::ADC_sma()
{
    MyDisplay.clear();

    MyDisplay.Write_yellow("Start Check SMA \nwith ADC measure\n");

    Signal_t Signal[NB_HEADSET];

    for(int i=0;i<NB_HEADSET;i++)
    {
        Signal[i].DuringTime    = 500;
        Signal[i].DutyCycle     = (int) 10;
        Signal[i].Frequency     = 50;
    }

    mySMA_Headset[3]->CheckCommandADC(Signal[0]);
}

void MainWindow::SMA_Sinus()
{
    qDebug() << "Command SMA enable /!\\" ;

    ui->Command_SMA->setCheckState(Qt::Checked);

   RcvText("Command SMA enable /!\\ \nWithSinus ",QColor(250,0,0));
   RcvText("MAX Current set to "+QString::number(mySMA_Headset[0]->getMaxCurrent())+"mA",QColor(180,180,80));

   MyDisplay.Write_yellow("Check SMA cmd\nWithSinus\n");

   double Distance[4] = {0};


   for(int i=0;i<(18*2);i++)
   {
       double Rad = qDegreesToRadians(i*20.0);

       Distance[0]=((double)qSin(Rad+M_PI*1.0));
       Distance[1]=((double)qSin(Rad+M_PI*1.5));
       Distance[2]=((double)qSin(Rad+M_PI*0.0));
       Distance[3]=((double)qSin(Rad+M_PI*0.5));

       RcvDistance(Distance,50);
       delay(50);
   }

    for(int j =0;j<4;j++)Distance[j] = 0 ;
    RcvDistance(Distance);
    ui->Command_SMA->setCheckState(Qt::Unchecked);
    qDebug() << "Finish";
}
