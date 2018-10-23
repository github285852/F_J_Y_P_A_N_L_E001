#ifndef __MBI5041_H
#define __MBI5041_H

#include "stm32f10x.h"
#include "sys.h"


#define CLK_H	PBout(13) = 1
#define CLK_L PBout(13) = 0
#define SDI_H PBout(15) = 1
#define SDI_L PBout(15) = 0
#define	LE_H  PBout(12) = 1
#define LE_L	PBout(12) = 0

#define	LE2_H	PBout(10) = 1
#define LE2_L	PBout(10) = 0

#define SDO_IN PBin(14)

void mbi5041_init(void);
void mbi5041_test(void);
void mbi5041_set_pwm(unsigned int *buf);
void PWM_Test(void);

#endif



