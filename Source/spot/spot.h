#ifndef __SPOT_H
#define __SPOT_H

#include "includes.h"
#include "spot_menutask.h"

//LED Config
#define LEDCWX_DEFAULT	0.3247       //0.4438
#define LEDCWY_DEFAULT	0.3247       //3958
#define LEDWWX_DEFAULT	0.4558      //0.3069
#define LEDWWY_DEFAULT	0.4046      //0.3044
#define LEDWX_DEFAULT	  0.3278
#define LEDWY_DEFAULT		0.3332
#define LEDRX_DEFAULT		0.6906
#define LEDRY_DEFAULT		0.3056
#define LEDGX_DEFAULT		0.2257
#define LEDGY_DEFAULT		0.7196
#define LEDBX_DEFAULT		0.1554 //
#define LEDBY_DEFAULT		0.0251
#define LEDYX_DEFAULT		0.5722//Y //0.00783	//¡£
#define LEDYY_DEFAULT		0.4248//Y0.4733	// .
#define LEDAX_DEFAULT		0.5523
#define LEDAY_DEFAULT		0.4321
#define LEDLX_DEFAULT		0.4189
#define LEDLY_DEFAULT		0.5486

#define RCH		0
#define ACH		1
#define LCH		2
#define GCH	3
#define BCH	4

//key config

#define S1			0x0008
#define S2			0x2000
#define S3			0x1000
#define S4			0x0800
#define S5			0x0400
#define S6			0x0200
#define S7			0x0100
#define S8			0x0080

#define PSELECT_KEY			S1


#define DEVICE_NAME			"Spot"
#define DEVICE_VER			"0001"
#define PIXELS					1





extern polyfit ledfit[5];
extern float mixluf[LED_CHS];
extern float maxlumen[LED_CHS];
extern COORD led_coord[LED_NUMS];

void DMXChanleDataRefresh(u8 pixel,u16 *current);
void spot_init(void);


#endif
