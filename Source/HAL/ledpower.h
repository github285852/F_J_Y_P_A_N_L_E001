#ifndef __LEDPOWER_H
#define __LEDPOWER_H

#include "color_light.h"


//ͬ�ȵ��������߻�ⲻ���е㣬�е�ƫ��
//#define OUT_R_DB	1
//#define OUT_G_DB	0.7418
//#define OUT_B_DB	0.2543
//#define OUT_WW_DB	1
//#define OUT_CW_DB	1

//�� CW Ϊ��׼
#define OUT_CW_DB	1
#define OUT_WW_DB	1
#define OUT_R_DB	1   // OUT_CW_DB*1.8
#define OUT_G_DB	(OUT_R_DB/1.348)
#define OUT_B_DB	(OUT_R_DB/3.9314)  // 3.9314


//ͬ������źŰ׵ƶ�RGBʵ�ʵ�����ֵ��ͬһ��ư壬RGB�Ͱ׵�ͨ�������������ܲ�һ����
#define WWCW_RGBK		1


#define A_BOARD_ADDR		1
#define B_BOARD_ADDR		18

void LedPowerOut(RGB  *ledk,float dim,unsigned char pixel);
void AllLedPowerOut(RGB  *ledk,float dim);
void LedPowerOff(unsigned char pixel);
void AllLedPowerOff(void);
void ChanleDataSend(unsigned char addr);
void LedkToCurrent(RGB	*ledk,float dim,unsigned char pixel);
void LedDmxTask(void);

//
extern unsigned short current[5];

#endif


