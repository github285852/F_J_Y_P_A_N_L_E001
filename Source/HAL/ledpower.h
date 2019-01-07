#ifndef __LEDPOWER_H
#define __LEDPOWER_H

#include "spot.h"
#define DMX_LEN	50

//同等电流，两者混光不再中点，有点偏移
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
//以 CW 为基准
#define OUT_CW_DB	1
#define OUT_WW_DB	1
#define OUT_R_DB	1   // OUT_CW_DB*1.8
#define OUT_G_DB	(OUT_R_DB/1.348)
#define OUT_B_DB	(OUT_R_DB/3.9314)  // 3.9314

#endif
//同等输出信号白灯对RGB实际电流比值（同一块灯板，RGB和白灯通道，最大电流可能不一样）
#define WWCW_RGBK		1


#define A_BOARD_ADDR		1
#define B_BOARD_ADDR		18

void LedPowerOut(float  *ledk,float dim,unsigned char pixel);
void AllLedPowerOut(float  *ledk,float dim);
void LedPowerOff(unsigned char pixel);
void AllLedPowerOff(void);
void ChanleDataSend(unsigned char addr);
void LedkToCurrent(float	*ledk,float dim,unsigned char pixel);
void LedDmxTask(void);
void MixLightKToCurrent(float *mixk,float dim,unsigned short *current);

//
extern unsigned short current[5];
extern unsigned short fan_pwm;

#endif


