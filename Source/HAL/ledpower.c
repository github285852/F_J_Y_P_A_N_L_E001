#include "ledpower.h"
#include "color_light.h"
#include "includes.h"
#include "math.h"

#define  PIXEL_NUMS	4       
#define Current_MIN  2692   //直流的最小值,  DAC_MIN对应的输出电流和DAC_MAX之比在乘以Current_MAX
#define Current_MAX	65535

#define DMX_LEN	50

u16 current[LED_NUMS];
u16 fan_pwm;
float T1,T;
float TEST_DIM;

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
void ChanleDataChange(RGB  *ledk,float dim,unsigned char pixel)
{
	float temp;
	char buf[50];
	unsigned char sum=0;
	unsigned int dac_data[LED_NUMS],pwm_data[LED_NUMS];
	T1= GetSysTime_us()/1000000.0f;
	dim = pow(dim,1.5);//伽马校正
	if(dim<=0.1)
		fan_pwm = 0;
	else
		fan_pwm = 65535;
	TEST_DIM = dim;
  T = GetSysTime_us()/1000000.0f - T1;
	if(Sys.Config.lightmode==CCT_M)
	{
		ledk->r = ledk->r*2;
		ledk->g = ledk->g*2;
		ledk->b = ledk->b*2;
	}
	else
	{
		ledk->ww = ledk->ww/2;
		ledk->cw = ledk->cw/2;
	}
	temp = ledk->r + ledk->g + ledk->b + ledk->cw + ledk->ww;//设亮度系数位temp
	if(temp != 0)
		temp = (float)(dim*(Current_MAX)/temp);//归一化
	switch(pixel)
	{
		case 0:                   //全部
		{
			current[0] =  ledk->cw * temp;
			current[1] =  ledk->r * temp; 
			current[2] =  ledk->g * temp;
			current[3] =  ledk->b * temp;
			current[4] =  ledk->ww * temp;
			ChanleDataSend(1);
			
			current[0] =  ledk->ww * temp;
			current[1] =  ledk->b * temp; 
			current[2] =  ledk->g * temp;
			current[3] =  ledk->r * temp;
			current[4] =  ledk->cw * temp;
			ChanleDataSend(18);
		}
		case 1: 
		{
			current[0] =  ledk->cw * temp;
			current[1] =  ledk->r * temp; 
			current[2] =  ledk->g * temp;
			current[3] =  ledk->b * temp;
			current[4] =  ledk->ww * temp;
			ChanleDataSend(1);
			break;
		}
		case 2:
			current[0] =  ledk->ww * temp;
			current[1] =  ledk->b * temp; 
			current[2] =  ledk->g * temp;
			current[3] =  ledk->r * temp;
			current[4] =  ledk->cw * temp;
			ChanleDataSend(18);
		break;
		default:break;
	}
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
		current[i] = 0;
	}
	if(pixel==0)
	{
		ChanleDataSend(1);
	}else
	{
		ChanleDataSend(18);
	}
}

void AllLedPowerOut(RGB *ledk,float dim)
{
	int i;
	float temp;
	ChanleDataChange(ledk,dim,0);
//	for(i=0;i<5;i++)
//	{
//		temp = current[i];
//		temp = 900 * temp /65535.0;
//		Debug_printf("I%d = %.2f\r\n",i,temp);
//	}
	
}

void AllLedPowerOff(void)
{
	int i;
	for(i=0;i<5;i++)
	{
		current[i] = 0;
	}
	ChanleDataSend(1);
	ChanleDataSend(18);
}

void FAN_OUT(unsigned short PWM)
{
	fan_pwm = PWM;
	ChanleDataSend(1);
	ChanleDataSend(18);
}

void LedDmxTask(void)
{
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
}