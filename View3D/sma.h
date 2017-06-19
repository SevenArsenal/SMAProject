#ifndef SMA_H
#define SMA_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "commons.h"
#include "QDebug"

class sma
{
public:
    SMA_Channel Channel;
    unsigned int Pin;
    bool IsOK;

    sma();
    sma(SMA_Channel _Channel,unsigned int _Pin);
};

class SMA_controller
{
public:
    SMA_controller(); // by default IsSafe = 1 => Command emable
    ~SMA_controller(); // destructor with all gpio low
    int set(SMA_Channel _Channel, unsigned int _Pin, unsigned int _Value);
    int set(sma _sma, unsigned int _Value);
    int init(bool _IsSafe = false);

private:
    bool IsSafe;// If IsSafe = 1 => Command emable
};

#endif // SMA_H
