#include "includes.h"

//流明转电流拟合系数
polyfit ledfit[5]=
{
{0.0000352531387,-0.00634792071,3.19992143,-2.36879183},   //R
{0.00000484692672,-0.00029122039,1.60218601,-3.36593123},	 //A
{0.000000281251497,0.000543083308,1.07424873,-1.54811916}, //L
{0.00000130881024,0.000568619025,0.799767132,-1.71737890}, //G
{-0.00027246580,0.0567195376,7.75389446,1.10819537}				 //B
};

//混光比转流明比系数(该流明是个相对值）
float mixluf[LED_CHS]=
{
	1.0,  			//R
	1.448448,  	//A
	1.7932,  		//L
	2.4497, 		//G
	0.08496,  	//B
};

float maxlumen[LED_CHS] = 
{
	226,//R
	370,//A
	584.7,//L
	538.6,//G
	74,//B
};

COORD led_coord[LED_NUMS]=
{
	{LEDRX_DEFAULT,LEDRY_DEFAULT},//R
	{LEDWWX_DEFAULT,LEDWWY_DEFAULT},//WW
	{LEDGX_DEFAULT,LEDGY_DEFAULT},//G
	{LEDCWX_DEFAULT,LEDCWY_DEFAULT},//CW
	{LEDBX_DEFAULT,LEDBY_DEFAULT},//B
};


void DMXChanleDataRefresh(u8 pixel,u16 *current)
{
	char *p;
	int i;
	u16 out_current[LED_CHS];
	u8 sum=0;
	DMX1_TX_BUF[0] = 0;
	switch(pixel)
	{
		case 0:
		{
			//A_BOARD
			p = (char *) &DMX1_TX_BUF[1];
			if(current != NULL)
			{
				out_current[0] =  current[CWCH];  
				out_current[1] =  current[RCH];  
				out_current[2] =  current[GCH];  
				out_current[3] =  current[BCH];  
				out_current[4] =  current[WWCH];  
				for(i=0;i<5;i++) //地址从一开始
				{
					p[i*2] = ((u16)out_current[i])>>8; //高八位
					p[i*2+1] = ((u16)out_current[i])&0xff;//低八位
				}
			}
			p[10] = fan_pwm>>8; //高八位
			p[11] = fan_pwm&0xff;//低八位
			for(i=0;i<16;i++)//8通道
			{
				sum += p[i];
			}
			p[i] = sum; //校验和
			//B_BOARD
			p = (char *) &DMX1_TX_BUF[18];
			if(current != NULL)
			{
				out_current[0] =  current[WWCH];  
				out_current[1] =  current[BCH];  
				out_current[2] =  current[GCH]; 
				out_current[3] =  current[RCH];  
				out_current[4] =  current[CWCH];  
				for(i=0;i<5;i++) //地址从一开始
				{
					p[i*2] = ((u16)out_current[i])>>8; //高八位
					p[i*2+1] = ((u16)out_current[i])&0xff;//低八位
				}
			}
			p[10] = fan_pwm>>8; //高八位
			p[11] = fan_pwm&0xff;//低八位
			for(i=0;i<16;i++)//8通道
			{
				sum += p[i];
			}
			p[i] = sum; //校验和

			break;
		}
		case 1:
		{
			//A_BOARD
			p = (char *) &DMX1_TX_BUF[1];
			if(current != NULL)
			{
				out_current[0] =  current[CWCH];  
				out_current[1] =  current[RCH];  
				out_current[2] =  current[GCH];  
				out_current[3] =  current[BCH];  
				out_current[4] =  current[WWCH];  
				for(i=0;i<5;i++) //地址从一开始
				{
					p[i*2] = ((u16)out_current[i])>>8; //高八位
					p[i*2+1] = ((u16)out_current[i])&0xff;//低八位
				}
			}
			p[10] = fan_pwm>>8; //高八位
			p[11] = fan_pwm&0xff;//低八位
			for(i=0;i<16;i++)//8通道
			{
				sum += p[i];
			}
			p[i] = sum; //校验和
			break;
		}
		case 2:
		{
			//B_BOARD
			p = (char *) &DMX1_TX_BUF[18];
			if(current != NULL)
			{
				out_current[0] =  current[WWCH];  
				out_current[1] =  current[BCH];  
				out_current[2] =  current[GCH]; 
				out_current[3] =  current[RCH];  
				out_current[4] =  current[CWCH];  
				for(i=0;i<5;i++) //地址从一开始
				{
					p[i*2] = ((u16)out_current[i])>>8; //高八位
					p[i*2+1] = ((u16)out_current[i])&0xff;//低八位
				}
			}
			p[10] = fan_pwm>>8; //高八位
			p[11] = fan_pwm&0xff;//低八位
			for(i=0;i<16;i++)//8通道
			{
				sum += p[i];
			}
			p[i] = sum; //校验和
			break;
		}
		default:break;
	}
}


