
#include "MCP3208.h"


MCP3208::MCP3208()
{
	
}
MCP3208::~MCP3208()
{
	
}

int MCP3208::init()
{
    int r = wiringPiSPISetup (SPI_Channel_ADC,SPI_SPEED) ;
    if ( r < 0)
    {
        fprintf (stderr, "initADC : %s\n", strerror (errno)) ;
    }
    return r;
}

int MCP3208::get_raw(unsigned int Channel)
{
    int Value;

    if(Channel < 0 || Channel > 7)
        return -1;

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

double MCP3208::Raw_to_Current(unsigned int _Raw, unsigned int _Channel)
{
    if(_Channel == SMA_CHANNEL_A)
    {
        return (double) (_Raw*Vref*1.0/1024)/Resistor_current_A ;
    }else if(_Channel == SMA_CHANNEL_D)
    {
        return (double) (_Raw*Vref*1.0/1024)/Resistor_current_D ;
    }
}
