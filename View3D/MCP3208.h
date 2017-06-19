#ifndef MCP3208_H_
#define MCP3208_H_
/*
 * MCP3208.h:
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

#include "commons.h"

#ifndef DAC_CHANNEL_A

#define DAC_CHANNEL_A 0 	// Vout A
#define DAC_CHANNEL_B 1 	// Vout B

#endif

#ifndef SPI_SPEED

#define SPI_SPEED 500000 	// from 500 000 Hz to 32 Mhz
#define SPI_Channel_ADC 1 	// CE1

#endif

//#define SMA_CHANNEL_A 0 	// PA
//#define SMA_CHANNEL_D 3 	// PD

class MCP3208
{
private:
    bool IsInit;
	
public:


    double Vref; // Volt
    double Resistor_current_A; // Ohm
    double Resistor_current_D; // Ohm

        MCP3208();
        ~MCP3208();
        int init();
        int get_raw(ADC_Channel Channel);
        double Raw_to_Volt(unsigned int _Raw);
        double Raw_to_Current(unsigned int _Raw, SMA_Channel _Channel = SMA_CHANNEL_A);
        ADC_Channel SMA_to_ADC(SMA_Channel _Channel);
};
#endif //MCP3208_H_
