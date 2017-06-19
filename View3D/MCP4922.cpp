
#include "MCP4922.h"


MCP4922::MCP4922()
{
    IsInit = false;
	
}
MCP4922::~MCP4922()
{

    printf("KILL DAC");

    DAC_set(DAC_CHANNEL_A,0);
    DAC_set(DAC_CHANNEL_B,0);
}

int MCP4922::init()
{
	int r = wiringPiSPISetup (SPI_Channel_DAC,SPI_SPEED) ;
	if ( r < 0)
	{
		fprintf (stderr, "initDAC : %s\n", strerror (errno)) ;
	}
    IsInit = true;
	return r;
}


int MCP4922::DAC_set(DAC_Channel _Channel, unsigned int _DACval)
{
    if(!IsInit) return -1;
	int Return = 0;
	
	int DAC_buffered = 0;
	int DAC_gain = 1;
	int DAC_shutdown = 1;

	if(_DACval > ((1<<12)-1))
		return -1;
	
	unsigned char Data[2] = "0";
	
    Data[0] += (_Channel & 0x01)		<< 7 ; 	// X000 0000
	Data[0] += (DAC_buffered & 0x01)	<< 6;	// 0X00 0000
	Data[0] += (!DAC_gain & 0x01)	<< 5;		// 00X0 0000
	Data[0] += (!DAC_shutdown & 0x01)	<< 4;	// 000X 0000
	
	Data[0] += ((_DACval>>8) & 0x0F);
	
	Data[1] += _DACval & 0xFF ;
	
    //printf("Sending  ( %d : %d ) ...\n",(unsigned int) Data[0], (unsigned int) Data[1] );

    int r = wiringPiSPIDataRW (SPI_Channel_DAC, &Data[0], 2);
    if(r<0) return -1;
	//printf("return %d\n",r );
	
	//printf("Received ( %d : %d )  \n",(unsigned int) Data[0], (unsigned int) Data[1] );
	
	
    return Return;
}

int MCP4922::DAC_set(SMA_Channel _Channel, unsigned int _DACval)
{
   return DAC_set(SMA_to_DAC( _Channel),_DACval);
}

DAC_Channel MCP4922::SMA_to_DAC(SMA_Channel _Channel)
{
    switch(_Channel)
    {
        case SMA_CHANNEL_A:
            return DAC_CHANNEL_A;
            break;
        case SMA_CHANNEL_D:
            return DAC_CHANNEL_B;
            break;
    }
    return DAC_CHANNEL_A;
}
