/*
dsPIC33EP256MC806 Motor Control
Copyright (C) 2014 Craig Peacock

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "xc.h"
#include "board.h"
#include "pwm.h"

void Init_PWM(void)
{
    // Uses PWM1L (RE0) 
    INA_DIR = 0;
    INB_DIR = 0;
    INA = 1;
    INB = 0;
    
    // Set up PWM Time Base
    PTCONbits.PTEN = 1;         // Enable PWM Module

    // Set up Primary Master PWM Time Base
    // Tcy = 8MHz / 2 Prescaler / 255 = 15.625kHz switching frequency
    // PWM Module 1:
    PTCON2bits.PCLKDIV = 0b001; // PWM Time Base Input Clock Prescaler is 1:2
    PTPER = 255;                // PWM Time Base Register will count up and reset on match

    // PWM Module 1:
    // Use primary master time base for sync & clock source
    // SDCx registers provide duty cycle information
    IOCON1bits.PMOD = 0b11;     // PWM I/O Pair in Independent Output Mode
    IOCON1bits.PENL = 1;        // PWM1L Pin Enabled (RE0)
    // Set up initial Duty Cycle to 50%
    SDC1 = 50;                 // Control's PWM1L Output Pin  
}

void PWM_SetDutyCycle(int dc)
{
    //Acceptable value 0 to 255.
    SDC1 =  dc;
}

void PWM_SetDir(int dir)
{
    switch(dir) {
        case CLOCKWISE:
            INA = 1;
            INB = 0;
            break;
        case COUNTERCLOCKWISE:
            INA = 0;    
            INB = 1;
            break;
        case BRAKEVCC:
            INA = 1;
            INB = 1;
            break;
        case BRAKEGND:
            INA = 0;
            INB = 0;
            break;
    }
}