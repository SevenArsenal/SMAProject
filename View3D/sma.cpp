#include "sma.h"
/*
 * sma.h:
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


SMA_controller::SMA_controller()
{

}

SMA_controller::~SMA_controller()
{
    printf("Pull all pin down\n");
    qDebug() << "Pull all pin down\n";
    for(int y=0;y<7;y++)
    {
        pinMode(y, INPUT) ;
        pullUpDnControl(y,PUD_DOWN); ;
    }
    for(int y=21;y<30;y++)
    {
        pinMode(y, INPUT) ;
        pullUpDnControl(y,PUD_DOWN); ;
    }
}

int SMA_controller::set(SMA_Channel _Channel, unsigned int _Pin, unsigned int _Value)
{
    int Return = 0;
    int Gpio_pin = 200;

    if(_Channel != SMA_CHANNEL_A && _Channel != SMA_CHANNEL_D)
        return -1;
    if(_Pin < 1 || _Pin > 8)
        return -1;
    if(_Value > 1)
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

   //printf("SMA_set(_Channel=%d,_Pin=%d,_Value=%d) => digitalWrite(%d,%d) %s\n",_Channel,_Pin,_Value,Gpio_pin, _Value,IsSafe? "":"\n !! but Safety is enable ^^");

    if(this->IsSafe)
        digitalWrite (Gpio_pin, _Value) ;

    return Return;
}

int SMA_controller::set(sma _sma, unsigned int _Value)
{
    return set(_sma.Channel,_sma.Pin,_Value);
}

int SMA_controller::init(bool _IsSafe)
{
    this->IsSafe=_IsSafe;

    if(!this->IsSafe)
        printf("[SMA_controller] !! Safety is enable !!\n") ;

    for(int y=0;y<7;y++)
    {
        pinMode(y, OUTPUT) ;
    }
    for(int y=21;y<30;y++)
    {
        pinMode(y, OUTPUT) ;
    }
    return 1;
}


sma::sma()
{
    IsOK = false;
}

sma::sma(SMA_Channel _Channel, unsigned int _Pin){
    Channel = _Channel;
    Pin = _Pin;
    IsOK = false;
}
