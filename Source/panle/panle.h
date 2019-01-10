#ifndef __PANLE_H
#define __PANLE_H

#include "includes.h"
#include "panle_menutask.h"

#define LEDWWX_DEFAULT	0.451703       //0.4438
#define LEDWWY_DEFAULT	0.4000       //3958
#define LEDCWX_DEFAULT	0.3247      //0.3069
#define LEDCWY_DEFAULT	0.3228      //0.3044
#define LEDWX_DEFAULT		0.3278
#define LEDWY_DEFAULT		0.3332
#define LEDRX_DEFAULT		0.6829
#define LEDRY_DEFAULT		0.3156
#define LEDGX_DEFAULT		0.2225
#define LEDGY_DEFAULT		0.7305
#define LEDBX_DEFAULT		0.14317 //
#define LEDBY_DEFAULT		0.038277
#define LEDYX_DEFAULT		0.5722//Y //0.00783	//¡£
#define LEDYY_DEFAULT		0.4248//Y0.4733	// .
#define LEDLX_DEFAULT		0.4189
#define LEDLY_DEFAULT		0.5486
#define LEDAX_DEFAULT		0.4189
#define LEDAY_DEFAULT		0.5486

#define RX 	LEDRX_DEFAULT	// 0.7
#define RY 	LEDRY_DEFAULT	// 0.28
#define GX	LEDGX_DEFAULT//0.08
#define GY	LEDGY_DEFAULT//0.83
#define BX	LEDBX_DEFAULT	//0.1
#define BY	LEDBY_DEFAULT//0.1

#define RCH		0
#define WWCH	1
#define GCH		2
#define CWCH	3
#define BCH		4

#define S1			0x0080
#define S2			0x0100
#define S3			0x0200
#define S4			0x0400
#define S5			0x0800
#define S6			0x1000
#define S7			0x2000
#define S8			0x4000
#define S9			0x8000
#define S10			0x0008
#define S11			0x0040
#define S12			0x0001

#define PSELECT_KEY			S1


#define DEVICE_NAME			"Spot"
#define DEVICE_VER			"0001"
#define PIXELS					1





extern polyfit ledfit[5];
extern float mixluf[LED_CHS];
extern float maxlumen[LED_CHS];
extern COORD led_coord[LED_NUMS];

void DMXChanleDataRefresh(u8 pixel,u16 *current);

#endif




