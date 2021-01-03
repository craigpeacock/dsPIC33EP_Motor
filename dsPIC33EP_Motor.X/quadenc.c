
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
#include "quadenc.h"

void Init_QEI(void)
{
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));  // Unlock Peripheral Pin Select Registers
    RPINR14bits.QEA1R = 66;                     // Assign QEA1
    RPINR14bits.QEB1R = 67;                     // Assign QEB1
    __builtin_write_OSCCONL(OSCCON | (1<<6));   // Lock Peripheral Pin Select Registers
    
    QEI1CONbits.QEIEN = 1;                      // Enable Quadrature Encoder Interface Module
}

void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void)
{
    if (LED4) LED4 = 0;
    else      LED4 = 1;
    //Velocity = VEL1CNT;

    IFS1bits.T5IF = 0;      //Clear Timer5 interrupt flag
}

void Init_TMR5(void)
{
    // Set Up Timer 4/5
    // Fcy = 40MHz / 400000 = 10mS
    T5CONbits.TON = 0;      // Stop any 16-bit Timer3 operation
    T4CONbits.TON = 0;      // Stop any 16/32-bit Timer3 operation
    T4CONbits.T32 = 1;      // Enable 32-bit Timer mode
    T4CONbits.TCS = 0;      // Select External clock
    T4CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR5 = 0x00;            // Clear 32-bit Timer (msw)
    TMR4 = 0x00;            // Clear 32-bit Timer (lsw)
    PR5 = 0x0006;           // Load 32-bit period value (msw)
    PR4 = 0x1A80;           // Load 32-bit period value (lsw)
    IPC7bits.T5IP = 0x01;   // Set Timer5 Interrupt Priority Level
    IFS1bits.T5IF = 0;      // Clear Timer5 Interrupt Flag
    IEC1bits.T5IE = 1;      // Enable Timer5 interrupt
    T4CONbits.TON = 1;      // Start 32-bit Timer
}
