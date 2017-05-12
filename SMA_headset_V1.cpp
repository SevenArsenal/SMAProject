/*
 * SMA_headset_V1.cpp:
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 12.05.2017
 * 
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiSPI.h>

#include "MCP4922.h"
#include "MCP3208.h"
#include "sma.h"

#define SAFETY 0 	// if 1 output will NOT be command



#define SMA_THRESHOLD 300 	// use to detect SMA



/// ********************************************************************

MCP4922 DAC;

MCP3208 ADC;

/// ********************************************************************


// *********************************************************************
// Function Pre Declaration
int SMA_set(unsigned int _Channel, unsigned int _Pin, unsigned int _Value);
// *********************************************************************
// Initialisation
int Init()
{
	int ret =0;
	//printf ("Init start\n") ;
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

    for(int y=0;y<7;y++)
    {
        pinMode(y, OUTPUT) ;
    }
    for(int y=21;y<30;y++)
    {
        pinMode(y, OUTPUT) ;
    }
	//printf ("Init return %d\n",ret) ;
}

// *********************************************************************
// Main
int main ()
{
	printf ("\nSPI_SMAcheck Start\n%s\n",SAFETY? "\n !! Safety is enable !!":"") ;

	if ( Init() < 0) return 1;
	
printf("\nSMA_CHANNEL_A \n");  

	for (int i = 1 ; i < 9 ; i++)
	{

		SMA_set(SMA_CHANNEL_A,i,1);
        int ADC_raw = ADC.get_raw(ADC_SMA_CURRENT_CHANNELA);
        double ValueADC_Current = ADC.Raw_to_Current(ADC_raw);
		SMA_set(SMA_CHANNEL_A,i,0);
		

		printf(" -pin %d => ADC return => %4.0lf mA %s\n",i ,ValueADC_Current*1000, (ValueADC_Current*1000 >SMA_THRESHOLD)? " => SMA":"" );  
		delay(100);
	}
printf("\nSMA_CHANNEL_D \n");  

	for (int i = 1 ; i < 9 ; i++)
	{

		SMA_set(SMA_CHANNEL_D,i,1);
        int ADC_raw = ADC.get_raw(ADC_SMA_CURRENT_CHANNELD);
        double ValueADC_Current = ADC.Raw_to_Current(ADC_raw);
		SMA_set(SMA_CHANNEL_D,i,0);

		printf(" -pin %d => ADC return => %4.0lf mA %s\n",i ,ValueADC_Current*1000, (ValueADC_Current*1000 >SMA_THRESHOLD)? " => SMA":"" );  
		delay(100);
	}


	printf ("\nSPI_SMAcheck Stop\n") ;
	return 0 ;	
}

// *********************************************************************
// Function Declaration


int SMA_set(unsigned int _Channel, unsigned int _Pin, unsigned int _Value)
{
	int Return = 0;
	int Gpio_pin = 200;
	
	if(_Channel != SMA_CHANNEL_A && _Channel != SMA_CHANNEL_D)
		return -1;
	if(_Pin < 1 || _Pin > 8)
		return -1;
	if(_Value < 0 || _Value > 1)
		return -1;
	
	if(_Channel == SMA_CHANNEL_A)
	{
		Gpio_pin = _Pin-1;
	}else if(_Channel == SMA_CHANNEL_D)
	{
		Gpio_pin = _Pin + 7;
	}
	
	if(Gpio_pin > 6)
	{
		Gpio_pin += 14;
	}
	
	//printf("SMA_set(_Channel=%d,_Pin=%d,_Value=%d) => digitalWrite(%d,%d) %s\n",_Channel,_Pin,_Value,Gpio_pin, _Value,SAFETY? "\n !! but Safety is enable ^^":"");
	
	if(!SAFETY)
		digitalWrite (Gpio_pin, _Value) ;
		
	return Return;
}

