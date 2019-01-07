#include "ledpower.h"
#include "color_light.h"
#include "includes.h"
#include "math.h"
#include "mymath.h"
#define  PIXEL_NUMS	4       


u16 current[LED_NUMS];
u16 fan_pwm;

//通道的混光比例直接转电流输出
void MixLightKToCurrent(float *mixk,float dim,unsigned short *current)
{
	float luk[LED_CHS],lu;
	int i;
	float temp=0,i_temp[LED_CHS];
	u16 lu_max=65535,min;
	u16 max_p=0;
	float p;
	//各通道混光比转流明比，
	for(i=0;i<LED_CHS;i++)
	{
		luk[i] = mixk[i]*mixluf[i];
		temp += luk[i];
	}
	//找出，能够到达最大的流明。lu_max/temp*luk[i] <= maxlumen[i]
	for(i=0;i<LED_CHS;i++)
	{
		if(luk[i]>0)
		{
			min = maxlumen[i]*temp/luk[i];
			if(lu_max>min)
				lu_max = min;
		}
	}
	
	p = 	(float)(lu_max/temp);//归一化
	for(i=0;i<LED_CHS;i++)
	{
		lu  = p*luk[i];
		if(lu>maxlumen[i])
		{
			Debug_printf(">>%s:lu>maxluem[%d]\r\n",__FUNCTION__,i);
		}
		if(lu>0) 
			i_temp[i] 	= fit((polyfit *)&ledfit[i],lu);
		else
			i_temp[i] = 0;
	}
	max_p = (i_temp[RCH]*45 + i_temp[GCH]*52 + i_temp[BCH]*45 + i_temp[ACH]*45 + i_temp[GCH]*45)/1000;
	
	temp = (float)(dim*(lu_max)/temp);//归一化
	//各通道流明转电流输出,绝对输出,0-800mA
	for(i=0;i<LED_CHS;i++)
	{
		lu  = temp*luk[i];
		if(lu>maxlumen[i])
		{
			Debug_printf(">>%s:lu>maxluem[%d]\r\n",__FUNCTION__,i);
		}
		if(lu>0) 
			i_temp[i] 	= fit((polyfit *)&ledfit[i],lu);
		else
			i_temp[i] = 0;
		current[i] = i_temp[i]*65535/800; 
	}
	Debug_printf("Iout(%.4f,%.4f,%.4f,%.4f,,%.4f),max p:%d\r\n",i_temp[0],i_temp[1],i_temp[2],i_temp[3],i_temp[4],max_p);	
}


void ChanleDataSend(u8 addr)
{
	char *p;
	int i;
	u8 sum=0;
	p = &DMX1_TX_BUF[addr];// addr 1 ,11 chanle
	for(i=0;i<5;i++) //地址从一开始
	{
		p[i*2] = ((u16)current[i])>>8; //高八位
	  p[i*2+1] = ((u16)current[i])&0xff;//低八位
	}
	p[10] = fan_pwm>>8; //高八位
	p[11] = fan_pwm&0xff;//低八位
	for(i=0;i<16;i++)//8通道
	{
		sum += p[i];
	}
	p[i] = sum; //校验和
}

/*
INPUT: ledk,LED intensit P，dim，亮度0.0 -1.0;pixel：像素号0，全部量，1,第一个像素
*/
//LED 的比例，和亮度
void LedkToCurrent(float  *ledk,float dim,unsigned char pixel)
{
	u16 current[LED_CHS];
	int i;
	if(dim<=0.1)
		fan_pwm = 0;
	else
		fan_pwm = 30000;
	MixLightKToCurrent((float *)ledk,dim,current);
	DMXChanleDataRefresh(pixel,current);
}

/*
INPUT: ledk,LED intensit P，dim，亮度0.0 -1.0;pixel：像素号
*/
void LedPowerOut(float *ledk,float dim,unsigned char pixel)
{
	LedkToCurrent(ledk,dim,pixel);
}

void LedPowerOff(unsigned char pixel)
{
	int i;
	u16 current[LED_CHS];
	for(i=0;i<5;i++)
	{
		current[i] = 0;
	}
	DMXChanleDataRefresh(pixel,current);
}

void AllLedPowerOut(float *ledk,float dim)
{
	int i;
	float temp;
	LedkToCurrent(ledk,dim,0);
}

void AllLedPowerOff(void)
{
	u16 cuurent[LED_CHS];
	int i;
	for(i=0;i<5;i++)
	{
		current[i] = 0;
	}
	DMXChanleDataRefresh(0,current);
}

void FAN_OUT(unsigned short PWM)
{
	fan_pwm = PWM;
	DMXChanleDataRefresh(0,NULL);
}

void LedDmxTask(void)
{
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
}
