
#ifndef XC_HEADER_PWM_H
#define	XC_HEADER_PWM_H

enum dir { CLOCKWISE, COUNTERCLOCKWISE, BRAKEVCC, BRAKEGND };
void Init_PWM(void);
void PWM_SetDir(int dir);
void PWM_SetDutyCycle(int dc);

#endif	

