#ifndef __LEDPOWER_H
#define __LEDPOWER_H

//#include "spot.h"
#define DMX_LEN	50

//ͬ�ȵ��������߻�ⲻ���е㣬�е�ƫ��
//#define OUT_R_DB	1
//#define OUT_G_DB	0.7418
//#define OUT_B_DB	0.2543
//#define OUT_WW_DB	1
//#define OUT_CW_DB	1

#ifdef SPOT01
#define OUT_R_DB	1
#define OUT_A_DB	1.448448
#define OUT_L_DB	1.7932
#define OUT_G_DB	2.4497
#define OUT_B_DB	0.08496

#else
//�� CW Ϊ��׼
#define OUT_CW_DB	1
#define OUT_WW_DB	1
#define OUT_R_DB	1   // OUT_CW_DB*1.8
#define OUT_G_DB	(OUT_R_DB/1.348)
#define OUT_B_DB	(OUT_R_DB/3.9314)  // 3.9314

#endif
//ͬ������źŰ׵ƶ�RGBʵ�ʵ�����ֵ��ͬһ��ư壬RGB�Ͱ׵�ͨ�������������ܲ�һ����
#define WWCW_RGBK		1


#define A_BOARD_ADDR		1
#define B_BOARD_ADDR		18

void LedPowerOut(float  *ledk,float dim,unsigned char pixel);
void AllLedPowerOut(float  *ledk,float dim);
void LedPowerOff(unsigned char pixel);
void AllLedPowerOff(void);
void LedkToCurrent(float	*ledk,float dim,unsigned char pixel);
void LedDmxTask(void);
void MixLightKToCurrent(float *mixk,float dim,unsigned short *current);

//
extern unsigned short current[5];
extern unsigned short fan_pwm;

#endif


