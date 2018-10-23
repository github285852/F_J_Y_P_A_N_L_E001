#include "ledpower.h"
#include "color_light.h"
#include "includes.h"
#include "math.h"

#define  PIXEL_NUMS	4       
#define Current_MIN  2692   //直流的最小值,  DAC_MIN对应的输出电流和DAC_MAX之比在乘以Current_MAX
#define Current_MAX	65535

#define DMX_LEN	20

u16 current[LED_NUMS];
float T1,T;
float TEST_DIM;
/*
INPUT: ledk,LED intensit P，dim，亮度0.0 -1.0;pixel：像素号
*/
//LED 的比例，和亮度
void ChanleDataChange(RGB  *ledk,float dim,unsigned char pixel)
{
	float temp;
	char buf[50];
	unsigned char sum=0;
	char *p;
	unsigned int dac_data[LED_NUMS],pwm_data[LED_NUMS];
	int i;
	T1= GetSysTime_us()/1000000.0f;
	//dim = dim*0.9;
	dim = pow(dim,1.5);//伽马校正
	TEST_DIM = dim;
  T = GetSysTime_us()/1000000.0f - T1;
	temp = ledk->r + ledk->g + ledk->b + ledk->cw + ledk->ww;//设亮度系数位temp
	if(temp != 0)
		temp = (float)(dim*(Current_MAX)/temp);//归一化
	switch(pixel)
	{
		case 0:
		case 1: 
		{
			current[0] =  ledk->cw * temp;
			current[1] =  ledk->ww * temp; 
			current[2] =  ledk->b * temp;
			current[3] =  ledk->g * temp;
			current[4] =  ledk->r * temp;
			break;
		}
		case 2:
		case 3:
		{	
			current[0] = 	ledk->cw * temp;//冷白
			current[1] =  ledk->r * temp;    
			current[2] =  ledk->g * temp;
			current[3] =  ledk->b * temp;
			current[4] =  ledk->ww * temp; //暖白
			break;
		}
	}
	p = &DMX1_TX_BUF[1];// addr 1 ,11 chanle
	for(i=0;i<5;i++) //地址从一开始
	{
		p[i*2] = ((u16)current[i])>>8; //高八位
	  p[i*2+1] = ((u16)current[i])&0xff;//低八位
	}
	for(i=0;i<10;i++)
	{
		sum += p[i];
	}
	p[i] = sum; //校验和
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
}
/*
INPUT: ledk,LED intensit P，dim，亮度0.0 -1.0;pixel：像素号
*/
void LedPowerOut(RGB *ledk,float dim,unsigned char pixel)
{
	ChanleDataChange(ledk,dim,pixel);
}

void LedPowerOff(unsigned char pixel)
{
	int i;
	for(i=0;i<5;i++)
	{
		DMX1_TX_BUF[i*2] = 0;
	  DMX1_TX_BUF[i*2+1] = 0;
	}
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
}

void AllLedPowerOut(RGB *ledk,float dim)
{
	ChanleDataChange(ledk,dim,3);
}

void AllLedPowerOff(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		DMX1_TX_BUF[i*2] = 0;
	  DMX1_TX_BUF[i*2+1] = 0;
	}
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
}

