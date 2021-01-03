
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpic30.h>

#include "xc.h"
#include "board.h"
#include "adc.h"
#include "quadenc.h"
#include "pwm.h"

int main(void) {
    
    unsigned int AN0_AVG, AN1_AVG, AN2_AVG, AN3_AVG;
    
    Init_PLL();             // Initialise System Clock/PLL
    Init_UART();
    Init_QEI();
    Init_PWM();
    Init_TMR5();
    ANSELE = 0x0;           // Use PORTE as digital IO
    Init_ADC_T3_DMA();
    
    TRISEbits.TRISE0 = 0; 	// Make LED 1 an output
    TRISEbits.TRISE1 = 0;	// Make LED 2 an output
    TRISEbits.TRISE2 = 0;	// Make LED 3 an output
    TRISEbits.TRISE3 = 0;	// Make LED 4 an output
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;

    printf("\r\ndsPIC33EP256MU806 Motor Control Demo\r\n");
    
    while (1) {
        printf("Encoder0x%04X:%04X, ",POS1CNTH,POS1CNTL);
        printf("Velocity %05d, ",VEL1CNT);

        AN0_AVG = (BufferA[0][0] + BufferA[0][1] + BufferA[0][2] + BufferA[0][3]) /4;
        AN1_AVG = (BufferA[1][0] + BufferA[1][1] + BufferA[1][2] + BufferA[1][3]) /4;
        AN2_AVG = (BufferA[2][0] + BufferA[2][1] + BufferA[2][2] + BufferA[2][3]) /4;
        AN3_AVG = (BufferA[3][0] + BufferA[3][1] + BufferA[3][2] + BufferA[3][3]) /4;
        printf("AN0 %04d, AN1 %04d, AN2 %04d, AN3 %04d\r\n", AN0_AVG, AN1_AVG, AN2_AVG, AN3_AVG);
        
        PWM_SetDutyCycle(AN0_AVG / 16);
    }
     
    return 0;
}




