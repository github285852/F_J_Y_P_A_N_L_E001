#ifndef __LEDPOWER_H
#define __LEDPOWER_H

#include "color_light.h"


//同等电流，两者混光不再中点，有点偏移
//#define OUT_R_DB	1
//#define OUT_G_DB	0.7418
//#define OUT_B_DB	0.2543
//#define OUT_WW_DB	1
//#define OUT_CW_DB	1

//以 CW 为基准
#define OUT_CW_DB	1
#define OUT_WW_DB	1
#define OUT_R_DB	1   // OUT_CW_DB*1.8
#define OUT_G_DB	(OUT_R_DB/1.348)
#define OUT_B_DB	(OUT_R_DB/3.9314)  // 3.9314


//同等输出信号白灯对RGB实际电流比值（同一块灯板，RGB和白灯通道，最大电流可能不一样）
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


