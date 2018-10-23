#ifndef __PWM16_H
#define __PWM16_H

#include "sys.h"


#define TARR    65535

void PWM16_init(void);
void set_pwm(unsigned char ch,unsigned int duty);

#endif


