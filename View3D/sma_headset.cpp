#include "sma_headset.h"

void SMA_Headset::InternalThreadEntry()
{
    while(isRunning)
    {
        //printf("Run thread Alive !\n");

        piLock(0);

        Signal_t TemSignal = MySignal;

        piUnlock(0);

        //printf("Run thread (%d) Alive !\n",this->mysma[0].Pin);
        //printf("Run thread (%d) Alive !\n",pthread_self());

        if(TemSignal.Frequency!=0)
        {

            int ret =0;

            unsigned int Periode    = 0;
            unsigned int TimeHigh   = 0;
            unsigned int TimeLow    = 0;


            Periode = int(1*1000000/TemSignal.Frequency); // 0.92
            TimeHigh = int( (Periode*TemSignal.DutyCycle*1.0/100) +0.5 );
            TimeLow = Periode - TimeHigh;

            //printf("Periode is %d us\n",Periode);
            //printf("TimeHigh is %d us\n",TimeHigh);
            //printf("TimeLow is %d us\n",TimeLow);


            this->SetCurrent(MaxCurrent);

            int i = 0;

            while ((i<(int)TemSignal.DuringTime*1000) && isRunning)
                  {
                //printf("All SMA on\n");
                    ret += this->set(1);
                    delayMicroseconds(TimeHigh) ;
                //printf("All SMA off\n");
                    ret += this->set(0);
                    delayMicroseconds(TimeLow) ;
                    i+=TimeHigh+TimeLow;

                    if(ret != 0)
                        printf("ERROR on SET thread\n");
                  }

            this->SetCurrent(0);
        }

        piLock(0);

        MySignal.DuringTime = 0;
        MySignal.DutyCycle  = 0;
        MySignal.Frequency  = 0;

        piUnlock(0);


        this->set(0);


       delay(1);
    }

    piLock(0);

    MySignal.DuringTime = 0;
    MySignal.DutyCycle  = 0;
    MySignal.Frequency  = 0;

    piUnlock(0);

    this->set(0);

}
double SMA_Headset::getCurrentCoef() const
{
    return CurrentCoef;
}

SMA_Headset::~SMA_Headset()
{
    printf("Close Headset thread\n");
    StopRUN();

    printf("Close SMA \n");
    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
    {
        MySMA_controller.set(mysma[i],0);
    }
}

SMA_Headset::SMA_Headset()
{
    isInit = 0;
    CurrentCoef = CURRENT_COEF;
}

SMA_Headset::SMA_Headset(sma _sma1, sma _sma2, sma _sma3)
{
    isInit = 0;

    CurrentCoef = 0;

    mysma[0] = _sma1;
    mysma[1] = _sma2;
    mysma[2] = _sma3;

}

int SMA_Headset::init()
{

    piLock(0);

    MySignal.DuringTime = 0;
    MySignal.DutyCycle  = 0;
    MySignal.Frequency  = 0;

    piUnlock(0);


    int ret =0;
    //printf ("Init start\n") ;
    //qDebug() << "Init start" ;
    // Initialisation DAC
    if ( DAC.init() < 0) ret = -1;

    // Initialisation ADC
    if ( ADC.init() < 0) ret = -1;

    // Initialisation GPIO
    if ( wiringPiSetup () < 0)
    {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        ret = -1;
    }

    // Initialisation SMA
    if ( MySMA_controller.init(Is_Safe_Output) < 0) ret = -1;

    //printf ("Init return %d\n",ret) ;

    if(ret==0) isInit = true;
    else isInit = false;
    return ret;
}
int SMA_Headset::init(sma _sma1, sma _sma2, sma _sma3)
{

    mysma[0] = _sma1;
    mysma[1] = _sma2;
    mysma[2] = _sma3;

    return init();
}

int SMA_Headset::init(sma _sma[NB_SMA_PER_HEADSET])
{

    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
    {
        mysma[i] = _sma[i];
    }

    return init();
}

int SMA_Headset::set(bool _Value)
{

    if (!isInit) return -1;

    int ret =0;

    //if (!IsOK) printf("This SMA (%d) is not ok -_-' \n",this->mysma[0].Pin);
    //printf("MySMA_controller.set(%d,%d) \n",mysma[0].Pin,_Value);

    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
    {
        ret += MySMA_controller.set(mysma[i],_Value);
    }

    return ret;
}
double SMA_Headset::get_Current(sma _sma)
{

    SMA_Channel Channel = _sma.Channel;

    int ADC_raw = ADC.get_raw(ADC.SMA_to_ADC(Channel));

    double ADC_Value = ADC.Raw_to_Current(ADC_raw,Channel);

    return ADC_Value;

    //return ADC.Raw_to_Current(ADC.get_raw(ADC.SMA_to_ADC(_sma.Channel),_sma.Channel)
}
void SMA_Headset::checkADC()
{

    ADC.init();

    printf("ADC channel check\n");
    for(int i = 0;i<8;i++)
    {
        //printf(" -channel %d => %5.1d\n",i,ADC.get_raw((ADC_Channel)i));
        qDebug() << " -channel "<< (ADC_Channel)i <<" => "<<ADC.get_raw((ADC_Channel)i);
    }
}
// return true is SMA headset is ok
int SMA_Headset::isOK() // return true is SMA headset is ok
{
    if (!isInit) return -1;

    Nb_SMA = 0;
    IsOK = false;

    SetCurrent(500);

    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
    {
        MySMA_controller.set(mysma[i],1);

        int ADC_raw = ADC.get_raw(ADC.SMA_to_ADC(mysma[i].Channel));

        double ADC_Value = ADC.Raw_to_Current(ADC_raw,mysma[i].Channel);

        MySMA_controller.set(mysma[i],0);

        if(ADC_Value*1000 >SMA_THRESHOLD)
        {
            Nb_SMA++;
            mysma[i].IsOK = true;
        }else
        {
            mysma[i].IsOK = false;
        }
        printf(" -channel %d pin %d => %4.0lf mA %s\n",mysma[i].Channel, mysma[i].Pin ,ADC_Value*1000, (ADC_Value*1000 >SMA_THRESHOLD)? " => SMA":"" );
        delay(100);
    }

    if(Nb_SMA == 3)
    {
        IsOK = true;
    }
    printf("This SMA_headset is %s (%d/%d)\n",IsOK? "OK":"not OK",Nb_SMA,NB_SMA_PER_HEADSET);

    SetCurrent(0);

    return IsOK;
}

const char *SMA_Headset::isOKstring()
{
    if (!isInit) return "ERROR";

    char retstring[500] = "";

    Nb_SMA = 0;
    IsOK = false;

    SetCurrent(500);

    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
    {
        MySMA_controller.set(mysma[i],1);

        int ADC_raw = ADC.get_raw(ADC.SMA_to_ADC(mysma[i].Channel));

        double ADC_Value = ADC.Raw_to_Current(ADC_raw,mysma[i].Channel);

        MySMA_controller.set(mysma[i],0);

        if(ADC_Value*1000 >SMA_THRESHOLD)
        {
            Nb_SMA++;
            mysma[i].IsOK = true;
        }else
        {
            mysma[i].IsOK = false;
        }
        sprintf(retstring,"%s -channel %d pin %d => %4.0lf mA %s\n",retstring,mysma[i].Channel, mysma[i].Pin ,ADC_Value*1000, (ADC_Value*1000 >SMA_THRESHOLD)? " => SMA":"" );
        delay(100);
    }

    if(Nb_SMA == NB_SMA_PER_HEADSET)
    {
        IsOK = true;
    }
    sprintf(retstring,"%sThis SMA_headset is %s (%d/%d)\n",retstring,IsOK? "OK":"not OK",Nb_SMA,NB_SMA_PER_HEADSET);

    SetCurrent(0);

    return retstring;
}

int SMA_Headset::StartRUN()
{

    if(isRunning)
        return -1;

    isRunning = false;
/*
    if(!IsOK){
        printf("This SMA (%d) is not ok (%d/%d) -_-' \n",this->mysma[0].Pin,Nb_SMA,NB_SMA_PER_HEADSET);
        return isRunning;
    }
*/

    int ret = StartInternalThread();

    if(!ret)
    {
        printf("Run thread didn't start !\n");
        isRunning = false;
    }else
    {
        isRunning = true;
    }

    return isRunning;
}

void SMA_Headset::StopRUN()
{
    if(!isRunning)
    return;
    isRunning = false;
    WaitForInternalThreadToExit();
}

int SMA_Headset::SetSignal(Signal_t _MySignal)
{


    if(_MySignal.Frequency<=500 && _MySignal.Frequency >=10)
    {
      //printf("Frequency is %d Hz\n",_MySignal.Frequency);
    }
    else
    {
      printf("ERROR on Frequency\n");
      return -1;
    }
    if(_MySignal.DutyCycle<=50){
        //printf("DutyCycle is %d %%\n",_MySignal.DutyCycle);
    }
    else
    {
      printf("	ERROR on DutyCycle\n");
      return -1;
    }

    piLock(0);

    MySignal = _MySignal;

    piUnlock(0);
/*
    printf("MySignal.DuringTime     is %d us\n",MySignal.DuringTime);
    printf("MySignal.DutyCycle      is %d us\n",MySignal.DutyCycle);
    printf("MySignal.Frequency      is %d us\n",MySignal.Frequency);
*/
    return 0;
}

int SMA_Headset::SetCurrent(int _Current)
{
    unsigned int DAC_Current = (unsigned int)_Current*CurrentCoef;

    int ret = 0;

    for(int i=0;i<NB_SMA_PER_HEADSET;i++)
        ret += DAC.DAC_set(mysma[i].Channel,DAC_Current);

    return ret;
}

int SMA_Headset::SetMAXCurrent(int _Current)
{
    if(_Current<0||_Current>2000)
        return -1;

    MaxCurrent = _Current;
    return 0;

}

int SMA_Headset::CalibCurrent()
{
    // the aim is to find the value of CurrentCoef
    // But not enable now
    // Use TestCurrent instead and try to get return error close to 0
    return -1;
}

double SMA_Headset::TestCurrent(unsigned int _sma,double _Current)
{

    if (_sma >= NB_SMA_PER_HEADSET)
        return -1;

    unsigned int DAC_Current = (unsigned int)_Current*CurrentCoef;

    qDebug() << " Test current on -channel "<<mysma[_sma].Channel<<" pin" << mysma[_sma].Pin;

    int ret = 0;

    ret += DAC.DAC_set(mysma[_sma].Channel,DAC_Current);    // Turn on DAC

    ret += MySMA_controller.set(mysma[_sma],1);             // Turn on SMA

    double RealCurrent = get_Current(mysma[_sma]);          // Get ADC

    ret += MySMA_controller.set(mysma[_sma],0);             // Turn off SMA

    ret += DAC.DAC_set(mysma[_sma].Channel,0);              // Turn off DAC

    qDebug() << "RealCurrent = "<< RealCurrent << "DAC_Current = "<< DAC_Current;

    if(ret != 0)                        // If something wrong
        return -1;

    return _Current - (RealCurrent*1000);  // Return error
}
