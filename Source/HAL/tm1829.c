#include "sys.h"
#include "tm1829.h"
#include "delay.h"
#include "core_cm3.h"


void tm1289_send_24bits(unsigned long data)
{
	int i;
	data <<= 8;
	for(i=0;i<24;i++)
	{
		DIN_L;
		if(data&0x80000000)
		{
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			DIN_H;
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
		}
		else
		{
			__NOP();
			__NOP();
			__NOP();	
			__NOP();
			__NOP();
			__NOP();	
			DIN_H;
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();	
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();	
			__NOP();	
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();	
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();	
		}
		data <<= 1;
	}
}

u8 tm1829_data[12];
void tm1289_update_chanle(u8 *buf)
{
	u32 tm1289[TM1289_NUMS];
	int i;
	for(i=0;i<TM1289_NUMS;i++)
	{
		tm1289[i] = buf[i*3]<<16 |  buf[i*3+1]<<8 | buf[i*3+2];
	}
	INTX_DISABLE();
	for(i=0;i<TM1289_NUMS;i++)
	{
		tm1289_send_24bits(tm1289[i]);
	}
	INTX_ENABLE();
}



