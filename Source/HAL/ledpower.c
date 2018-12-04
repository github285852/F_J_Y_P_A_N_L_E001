#include "ledpower.h"
#include "color_light.h"
#include "includes.h"
#include "math.h"
#include "mymath.h"
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
void LedkToCurrent(RGB  *ledk,float dim,unsigned char pixel)
{
	int i;
	float temp;
	char buf[50];
	unsigned char sum=0;
	unsigned int dac_data[LED_NUMS],pwm_data[LED_NUMS];
	T1= GetSysTime_us()/1000000.0f;
	//dim = pow(dim,1.5);//伽马校正
	if(dim<=0.1)
		fan_pwm = 0;
	else
		fan_pwm = 65535;
	TEST_DIM = dim;
  T = GetSysTime_us()/1000000.0f - T1;
	if((Sys.Config.lightmode == CCT_M)||			\
		( Sys.Config.lightmode == DMX_M &&( Sys.Config.dmx.mode==DMX_M2 ||Sys.Config.dmx.mode==DMX_M7 ||Sys.Config.dmx.mode==DMX_M12 ) ))
	{
		ledk->r = ledk->r*WWCW_RGBK*OUT_R_DB;
		ledk->g = ledk->g*WWCW_RGBK*OUT_G_DB;
		ledk->b = ledk->b*WWCW_RGBK*OUT_B_DB;
		
		ledk->ww = ledk->ww*OUT_WW_DB;
		ledk->cw = ledk->cw*OUT_WW_DB;
	}
	else
	{
		ledk->r = ledk->r*OUT_R_DB;
		ledk->g = ledk->g*OUT_G_DB;
		ledk->b = ledk->b*OUT_B_DB;
		
		ledk->ww = ledk->ww/WWCW_RGBK*OUT_WW_DB;
		ledk->cw = ledk->cw/WWCW_RGBK*OUT_CW_DB;
	}
	temp = ledk->r + ledk->g + ledk->b + ledk->cw + ledk->ww;//设亮度系数位temp
	if(temp != 0)
	{
		if((Sys.Config.lightmode == CCT_M)||			\
		( Sys.Config.lightmode == DMX_M &&( Sys.Config.dmx.mode==DMX_M2 ||Sys.Config.dmx.mode==DMX_M7 ||Sys.Config.dmx.mode==DMX_M12 ) ))
			temp = (float)(dim*(Current_MAX*2)/temp);//归一化
		else
			temp = (float)(dim*(Current_MAX)/temp);//归一化
	}
	if((ledk->r * temp)>65535)
	{
		Debug_printf(">>%s:Angle led out is master,LEDR\r\n",__FUNCTION__);
	}
	if((ledk->g * temp)>65535)
	{
		Debug_printf(">>%s:Angle led out is master,LEDG\r\n",__FUNCTION__);
	}
	if((ledk->b  * temp)>65535)
	{
		Debug_printf(">>%s:Angle led out is master,LEDB\r\n",__FUNCTION__);
	}
	if((ledk->ww * temp)>65535)
	{
		Debug_printf(">>%s:Angle led out is master,LEDWW\r\n",__FUNCTION__);
	}
	if((ledk->cw * temp)>65535)
	{
		Debug_printf(">>%s:Angle led out is master,LEDCW\r\n",__FUNCTION__);
	}
	
	switch(pixel)
	{
		case 0:                   //全部
		{
			current[0] =  LIMIT(ledk->ww * temp,0,65535);
			current[1] =  LIMIT(ledk->b * temp,0,65535); 
			current[2] =  LIMIT(ledk->g * temp,0,65535);
			current[3] =  LIMIT(ledk->r * temp,0,65535);
			current[4] =  LIMIT(ledk->cw * temp,0,65535);
			ChanleDataSend(B_BOARD_ADDR);
			
			
			current[0] =  LIMIT(ledk->cw * temp,0,65535);
			current[1] =  LIMIT(ledk->r * temp,0,65535); 
			current[2] =  LIMIT(ledk->g * temp,0,65535);
			current[3] =  LIMIT(ledk->b * temp,0,65535);
			current[4] =  LIMIT(ledk->ww * temp,0,65535);
			ChanleDataSend(A_BOARD_ADDR);
			

			break;
		}
		case 1: 
		{
			current[0] =  LIMIT(ledk->cw * temp,0,65535);
			current[1] =  LIMIT(ledk->r * temp,0,65535); 
			current[2] =  LIMIT(ledk->g * temp,0,65535);
			current[3] =  LIMIT(ledk->b * temp,0,65535);
			current[4] =  LIMIT(ledk->ww * temp,0,65535);
			ChanleDataSend(1);
			break;
		}
		case 2:
			current[0] =  LIMIT(ledk->ww * temp,0,65535);
			current[1] =  LIMIT(ledk->b * temp,0,65535); 
			current[2] =  LIMIT(ledk->g * temp,0,65535);
			current[3] =  LIMIT(ledk->r * temp,0,65535);
			current[4] =  LIMIT(ledk->cw * temp,0,65535);
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
	LedkToCurrent(ledk,dim,pixel);

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
	LedkToCurrent(ledk,dim,0);
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