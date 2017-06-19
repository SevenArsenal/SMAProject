#ifndef SMA_HEADSET_H
#define SMA_HEADSET_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "wiringPi.h"

#include "commons.h"

#include "MCP4922.h"
#include "MCP3208.h"
#include "sma.h"
#include <mythreadclass.h>


class SMA_Headset : public MyThreadClass
{
private:

    SMA_controller MySMA_controller;
    MCP3208 ADC;
    MCP4922 DAC;

    sma mysma[NB_SMA_PER_HEADSET]; // one channel & one pin

    int MaxCurrent;
    bool IsEnable;

    int isInit;

    int Nb_SMA;
    bool IsOK;

    Signal_t MySignal; // Protected by Key 0

    bool isRunning;
    void InternalThreadEntry();

    double CurrentCoef; //image of position of potentiometer;

public:
    ~SMA_Headset();
    SMA_Headset();
    SMA_Headset(sma _sma1,sma _sma2,sma _sma3);
    int init();
    int init(sma _sma1, sma _sma2, sma _sma3);
    int init(sma _sma[NB_SMA_PER_HEADSET]);

    int set(bool _Value);

    int isOK(); // return true is SMA headset is ok
    const char *isOKstring();
    int StartRUN();
    void StopRUN();
    void checkADC();
    double get_Current(sma _sma);
    int SetSignal(Signal_t _MySignal);
    int SetCurrent(int _Current);
    int CalibCurrent();
    double TestCurrent(unsigned int _sma, double _Current);
    double getCurrentCoef() const;
};

#endif // SMA_HEADSET_H
