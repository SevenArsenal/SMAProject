
#include "MCP3208.h"


MCP3208::MCP3208()
{
    IsInit = false;
}
MCP3208::~MCP3208()
{
	
}

int MCP3208::init()
{
    Vref = 3.3; // Volt
    Resistor_current_A = RESISTOR_CURRENT_A; // Ohm
    Resistor_current_D = RESISTOR_CURRENT_D; // Ohm
    int r = wiringPiSPISetup (SPI_Channel_ADC,SPI_SPEED) ;
    if ( r < 0)
    {
        fprintf (stderr, "initADC : %s\n", strerror (errno)) ;
    }
    IsInit = true;
    return r;
}

int MCP3208::get_raw(ADC_Channel Channel)
{
    if(!IsInit) return -1;
    int Value;

    unsigned char Data[3] = "0";
    Data[0] = 1;
    Data[1] = (8+Channel)<<4;
    Data[2] = 0;

    //printf("Sending  ( %d : %d : %d ) ...\n",(unsigned int) Data[0], (unsigned int) Data[1], (unsigned int) Data[2] );

    int r = wiringPiSPIDataRW (SPI_Channel_ADC, &Data[0], 3);
    if(r<0) return -1;
    //printf("return %d\n",r );

    //printf("Received ( %d : %d : %d )  \n",(unsigned int) Data[0], (unsigned int) Data[1], (unsigned int) Data[2] );

    Value = ((Data[1]&3) << 8) + Data[2];

    return Value;
}

double MCP3208::Raw_to_Volt(unsigned int _Raw)
{
    return (double) _Raw*Vref*1.0/1024;
}

double MCP3208::Raw_to_Current(unsigned int _Raw,SMA_Channel _Channel)
{
    if(_Channel == SMA_CHANNEL_A)
    {
        return (double) (_Raw*Vref*1.0/1024)/Resistor_current_A ;
    }else if(_Channel == SMA_CHANNEL_D)
    {
        return (double) (_Raw*Vref*1.0/1024)/Resistor_current_D ;
    }
    return -1;
}
ADC_Channel MCP3208::SMA_to_ADC(SMA_Channel _Channel)
{
    switch(_Channel)
    {
        case SMA_CHANNEL_A:
            return ADC_CHANNEL_0;
            break;
        case SMA_CHANNEL_D:
            return ADC_CHANNEL_1;
            break;
    }
    return ADC_CHANNEL_0;
}
