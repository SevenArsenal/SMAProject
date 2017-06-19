#include "camera.h"


void Camera::InternalThreadEntry()
{
    while(isRunning)
    {
        //printf("Run thread Alive !\n");

        Camera_run_mutex.lock();
        Camera_Run_t _Camera_Run = Camera_run;
        Camera_run_mutex.unlock();

    switch(_Camera_Run)
    {
        case CAMERA_STOP:
            qDebug() << "thread is on CAMERA_STOP";
            isRunning = false;

            CamLeft.release();
            CamRight.release();

            cv::destroyAllWindows();
            break;
        case CAMERA_VIDEO:
                {
        //qDebug() << "thread is on CAMERA_VIDEO";
            cv::Mat frameL;
            cv::Mat frameR;
            bool bSuccessL = CamLeft.read(frameL); // read a new frame from video
            bool bSuccessR = CamRight.read(frameR); // read a new frame from video

             if (!bSuccessL&&!bSuccessR) //if not success, break loop
            {
                 qDebug() << "Cannot read a frame from video stream" << endl;
            }
             if( frameL.empty() || frameR.empty() )
             {
                 qDebug() << " --(!) Error reading images ";
             }

             if(IS_IMSHOW_ENABLE)cv::imshow("Left", frameL); //show the frame in "MyVideo" window
             if(IS_IMSHOW_ENABLE)cv::imshow("Right", frameR); //show the frame in "MyVideo" window

            }
            break;
        case CAMERA_DISPARITY:
        //qDebug() << "thread is on CAMERA_DISPARITY";
            getDisparity();
            delayMicroseconds(1000) ;
            break;

        }

    }
    isRunning = false;
    Camera_run_mutex.lock();
    Camera_run = CAMERA_STOP;
    Camera_run_mutex.unlock();
}

Camera::Camera(QObject *parent) :
    QObject(parent)
{
    Distance[0] = 0;
    Distance[1] = 0;
    Distance[2] = 0;
    Distance[3] = 0;

    MaxValue = DISPARITY_MAX;

    isRunning = false;
    isCameraInit = false;
    Camera_run_mutex.lock();
    Camera_run = CAMERA_STOP;
    Camera_run_mutex.unlock();
}

int Camera::InitCamera()
{
    if(isRunning)
        return -1;

    qDebug() << "InitCamera";
    emit SendText("InitCamera",QColor(255,255,0));

    CamLeft.open(CAMERA_LEFT_INDEX);
    CamRight.open(CAMERA_RIGHT_INDEX);

    bool Leftisopened = CamLeft.isOpened();
    bool Rightisopened = CamRight.isOpened();

    qDebug() << "CamLeft.isOpened() " << Leftisopened;
    qDebug() << "CamRight.isOpened() " << Rightisopened;

    if ((Leftisopened == false) || (Rightisopened == false) )  // if not success, exit program
    {
        qDebug() << "ERROR";
        emit SendText("InitCamera ERROR",QColor(255,0,0));

        if (Leftisopened == false)  // if not success, exit program
        {
            emit SendText("Camera left don't start",QColor(255,150,0));
        }
        if (Rightisopened == false)
        {
            emit SendText("Camera right don't start",QColor(255,150,0));
        }
        return -1;
    }
    double dWidthR = CamRight.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeightR = CamRight.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    double fpsR = CamRight.get(CV_CAP_PROP_FPS); //get the fps of the video
    double BUFR = CamRight.get(CV_CAP_PROP_BUFFERSIZE); //get the buf of the video

    qDebug() << "CamRight :"<< endl <<" - Frame size : " << dWidthR << " x " << dHeightR;
    qDebug() <<" - FPS : " << fpsR;
    qDebug() <<" - Buffersize : " << BUFR << endl;

    double dWidthL = CamLeft.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeightL = CamLeft.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    double fpsL = CamLeft.get(CV_CAP_PROP_FPS); //get the fps of the video
    double BUFL = CamRight.get(CV_CAP_PROP_BUFFERSIZE); //get the buf of the video

    qDebug() << "CamLeft :"<< endl <<" - Frame size : " << dWidthL << " x " << dHeightL;
    qDebug() <<" - FPS : " << fpsL;
    qDebug() <<" - Buffersize : " << BUFL << endl;


    isCameraInit = true;

    qDebug() << "InitCamera End";
    return 0;
}
int Camera::StartCamera()
{
    qDebug() << "StartCamera";

    if(!isCameraInit)
        if(InitCamera())
            return -1;
    qDebug() << "StartCamera Init Finish";


    cv::Mat frameL;
    cv::Mat frameR;
    bool bSuccessL = CamLeft.read(frameL); // read a new frame from video
    bool bSuccessR = CamRight.read(frameR); // read a new frame from video

     if (!bSuccessL&&!bSuccessR) //if not success, break loop
    {
         qDebug() << "Cannot read a frame from video stream" << endl;
    }

    if(IS_IMSHOW_ENABLE)cv::imshow("Left", frameL); //show the frame in "MyVideo" window
    if(IS_IMSHOW_ENABLE)cv::imshow("Right", frameR); //show the frame in "MyVideo" window

    qDebug() << "StartCamera End";

    return 0;
}
int Camera::StartVideo()
{
    if(isRunning)
        return -1;
    if(StartCamera() != 0)
        return -1;

    Camera_run_mutex.lock();
    Camera_run = CAMERA_VIDEO;
    Camera_run_mutex.unlock();

    if(StartRUN() != 0)
        return -1;

    return 0;
}

int Camera::StartRUN()
{
    if(isRunning)
        return -1;

    Camera_run_mutex.lock();
    Camera_Run_t _Camera_Run = Camera_run;
    Camera_run_mutex.unlock();

    if(_Camera_Run == CAMERA_STOP)
        return -1;

    int ret = StartInternalThread();

    if(!ret)
    {
        printf("Run thread didn't start !\n");
        isRunning = false;
    }else
    {
        isRunning = true;
    }

    return 0;
}

void Camera::StopRUN()
{
    cv::destroyAllWindows();
    if(!isRunning)
        return ;
    Camera_run_mutex.lock();
    Camera_run = CAMERA_STOP;
    Camera_run_mutex.unlock();
    WaitForInternalThreadToExit();
}

int Camera::getDisparity()
{

      //-- 1. Read the images
    cv::Mat frameL;
    cv::Mat frameR;


    bool bSuccessL = CamLeft.grab(); // read a new frame from video
    bool bSuccessR = CamRight.grab(); // read a new frame from video
    //bSuccessL = CamLeft.read(frameL); // read a new frame from video
    //bSuccessR = CamRight.read(frameR); // read a new fra
    bSuccessL = CamLeft.read(frameL); // read a new frame from video
    bSuccessR = CamRight.read(frameR); // read a new frame from videome from video

     if (!bSuccessL&&!bSuccessR) //if not success, break loop
    {
         qDebug() << "Cannot read a frame from video stream" << endl;
    }
    //Mat imgLeft = imread( argv[1], IMREAD_GRAYSCALE );
    //Mat imgRight = imread( argv[2], IMREAD_GRAYSCALE );

    Mat imgLeft;
    Mat imgRight;

    cvtColor(frameL,imgLeft,CV_BGR2GRAY);
    cvtColor(frameR,imgRight,CV_BGR2GRAY);
    if(0){
        if(IS_IMSHOW_ENABLE)cv::imshow("Left", imgLeft);
        if(IS_IMSHOW_ENABLE)cv::imshow("Right", imgRight);
    }

    //-- And create the image in which we will save our disparities
    Mat imgDisparity16S = Mat( imgLeft.rows, imgLeft.cols, CV_16S );
    Mat imgDisparity8U = Mat( imgLeft.rows, imgLeft.cols, CV_8UC1 );

    if( imgLeft.empty() || imgRight.empty() )
    {
        qDebug() << " --(!) Error reading images ";
        return -1;
    }

    //-- 2. Call the constructor for StereoBM
    //int ndisparities = 16*5;   /**< Range of disparity */
    //int SADWindowSize = 21; /**< Size of the block window. Must be odd */
//    int ndisparities = 16*5;   /**< Range of disparity */
//    int SADWindowSize = 41; /**< Size of the block window. Must be odd */
    int ndisparities = 16*5;   /**< Range of disparity */
    int SADWindowSize = 61; /**< Size of the block window. Must be odd */

    Ptr<StereoBM> sbm = StereoBM::create( ndisparities, SADWindowSize );

    //-- 3. Calculate the disparity image
    sbm->compute( imgLeft, imgRight, imgDisparity16S );

    //-- Check its extreme values

    double minVal; double maxVal;
    Point min, max;

    minMaxLoc( imgDisparity16S, &minVal, &maxVal, &min, &max);

    // -- Crop image

    int PosX = ndisparities-1 + (SADWindowSize-1)/2;
    int PosY = (SADWindowSize-1)/2;
    int W = imgDisparity16S.size().width  - (SADWindowSize-1)/2 - PosX;
    int H = imgDisparity16S.size().height - (SADWindowSize-1)/2 - PosY;

    //qDebug() << "PosX " << PosX << " PosY " << PosY << " W " << W << " H " << H;

    cv::Rect CropTot(PosX,
                     PosY,
                     W,
                     H);

    //cv::Mat CropImage(imgDisparity16S,CropTot);
    cv::Mat CropImage;

    imgDisparity16S(CropTot).copyTo(CropImage);

    QString ToSend = "";

    ToSend = "Min Disp "+QString::number(minVal)+" Max Disp "+QString::number(maxVal)+"\n";
    ToSend.append("Min x "+QString::number(min.x)+"y "+QString::number(min.y)+"\n");
    ToSend.append("Max x "+QString::number(max.x)+"y "+QString::number(max.y)+"\n");
    //ToSend.append("Size height "+QString::number(imgDisparity16S.size().height)+" width "+QString::number(imgDisparity16S.size().width)+"\n");
    //ToSend.append("CropImage height "+QString::number(CropImage.size().height)+" width "+QString::number(CropImage.size().width)+"\n");


    //emit SendText(ToSend,QColor(0,200,200));

    //-- 4. Display it as a CV_8UC1 image
    //imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));
    //if(IS_IMSHOW_ENABLE)cv::imshow( "Disparity", imgDisparity8U );
    CropImage.convertTo( imgDisparity8U, CV_8UC1, 255/(MaxValue - minVal));
    if(IS_IMSHOW_ENABLE)cv::imshow( "CropImage", imgDisparity8U );

    //-- 5. Find 4 Max

    ToSend = "";

    for(int i=0;i<4;i++)
    {

        double minValQuart; double maxValQuart;

        cv::Mat CropImageQuart = CropImage(Rect(i*CropImage.size().width/4,0,CropImage.size().width/4,CropImage.size().height));

        minMaxLoc( CropImageQuart, &minValQuart, &maxValQuart);

        Distance[i] = maxValQuart / MaxValue;

        ToSend.append("Quart " +QString::number(i+1)+"/4 Max = "+QString::number(Distance[i])+"\n");

        //qDebug() << "Quart " << i+1 << "/4 Max = " << Distance[i];
    }

    ToSend.append("\n");

    emit SendDistance(Distance);
    emit SendText(ToSend,QColor(50,200,200));

    sbm.~Ptr();

    return 0;
}
int Camera::getDisparity(bool loop)
{

    if(!isCameraInit)
        if(InitCamera())
            return -1;

    //namedWindow( "Disparity", WINDOW_NORMAL );

    if(loop){
        StopRUN();
        Camera_run_mutex.lock();
        Camera_run = CAMERA_DISPARITY;
        Camera_run_mutex.unlock();
        return StartRUN();
    }else{
        if(isRunning)
            return -1;
        return getDisparity();
    }
    return -1;
}
