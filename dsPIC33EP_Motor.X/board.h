  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define LED1 	LATEbits.LATE0
#define LED2	LATEbits.LATE1
#define LED3 	LATEbits.LATE2
#define LED4	LATEbits.LATE3

#define SW1 	PORTEbits.RE4
#define SW2     PORTEbits.RE5
#define SW3     PORTEbits.RE6
#define SW4     PORTEbits.RE7

#define INA     LATDbits.LATD8
#define INB     LATDbits.LATD9
#define INA_DIR TRISDbits.TRISD8
#define INB_DIR TRISDbits.TRISD9

void Init_PLL(void);
void Init_AUXPLL(void);
void Init_UART(void);

#endif	/* XC_HEADER_TEMPLATE_H */

