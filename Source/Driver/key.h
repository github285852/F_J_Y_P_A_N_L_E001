#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define press_time_min    500

#define click_time_min		3

#define click_time_max		30

#define KEY1			0x01
#define KEY2			0x08
#define KEY3			0x40
#define KEY4			0x200
#define KEY5			0x400
#define KEY6			0x800
#define KEY7			0x1000
#define KEY8			0x2000
#define KEY9			0x4000

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



void key_service(void);
	
	
extern volatile unsigned char key_status; 
extern unsigned int key_value;
#endif
