#ifndef MCP4922_H_
#define MCP4922_H_
/*
 * MCP4922.h:
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

#define DAC_CHANNEL_A 0 	// Vout A
#define DAC_CHANNEL_B 1 	// Vout B

#ifndef SPI_SPEED

#define SPI_SPEED 500000 	// from 500 000 Hz to 32 Mhz
#define SPI_Channel_DAC 0 	// CE0

#endif

class MCP4922 
{
	
public:
		MCP4922();
		~MCP4922();
		int init();
		int DAC_set(unsigned int Channel, unsigned int _DACval);
};
#endif //MCP4922_H_
