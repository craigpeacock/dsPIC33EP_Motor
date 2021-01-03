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
#include "adc.h"

__eds__ int BufferA[4][4] __attribute__((eds,aligned(32)));

void __attribute__((interrupt, auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0;    // Clear DMA Interrupt Flag
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;      // Reset Timer 3 Interrupt Flag
}

void Init_ADC_T3_DMA(void)
{
    // Set up ADC1
    ANSELB = 0x000F;            // Set AN0 - AN3
    AD1CSSL = 0x000F;           // Scan channels AN0 to AN3
    AD1CSSH = 0x0000;
    AD1CHS0 = 0x0000;           // Select AN0

    AD1CON1bits.AD12B   = 1; 	// ADC operating in 12 bit mode, 1 channel
    AD1CON1bits.SSRC    = 2; 	// Timer 3 compare ends sampling and starts conversion
    AD1CON1bits.ASAM    = 1; 	// Sampling occurs immediately after last conversion
    AD1CON1bits.ADDMABM = 0; 	// DMA Buffers are written in Scatter/Gather mode.
    AD1CON2bits.CSCNA   = 1; 	// Scan Inputs
    AD1CON2bits.SMPI    = 3; 	// Increment Rate for DMA (No. Channel - 1)
    AD1CON3 = 0x1F00;           // TAD = 8 x TCY ???????????
    AD1CON4bits.DMABL = 2;      // Allocate 4 words of buffer to each analog input (Scatter Mode)
    AD1CON4bits.ADDMAEN = 1;    // Conversion results stored in ADCxBUF0 register, for transfer to RAM using DMA
    IFS0bits.AD1IF = 0;         // Reset ADC Interrupt Flag
    //IEC0bits.AD1IE = 1;       // Enable ADC Interrupts
    AD1CON1bits.ADON = 1;       // Turn ADC1 On

    // Set up Timer3
    // Timer 3 compare ends sampling and starts conversion
    // Fcy = 40MHz / 8 / 50000 = 10mS
    T3CONbits.TCKPS = 0b01;     // 1:8 Pre-scaler
    PR3 = 50000;
    IEC0bits.T3IE = 1;          // Enable Timer3 Interrupt
    IFS0bits.T3IF = 0;          // Reset Timer 3 Interrupt Flag
    T3CONbits.TON = 1;          // Start Timer 3

    // Set up DMA Transfers
    // DMA automatically transfers ADC values to Buffer A
    DMA0REQbits.IRQSEL = 13; 	// ADC1
    DMA0STAH = 0x0000;
    DMA0STAL = __builtin_dmaoffset(BufferA);
    //DMA0STBH = 0x0000;
    //DMA0STBL = __builtin_dmaoffset(BufferB);
    DMA0PAD = (volatile unsigned int)&ADC1BUF0;
    DMA0CNT = 15;               // DMA Transfer Count (4 Channels x 4 Bytes - 1)
    DMA0CONbits.AMODE = 2;      // Configure DMA for Peripheral Indirect Mode
    DMA0CONbits.MODE = 0;       // Continuous, Ping-Pong modes disabled
    IFS0bits.DMA0IF = 0;        // Clear the DMA interrupt flag bit
    IEC0bits.DMA0IE = 1;        // Set the DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;       // Enable DMA
}