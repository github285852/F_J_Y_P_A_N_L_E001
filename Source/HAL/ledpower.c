#include "ledpower.h"
#include "color_light.h"
#include "includes.h"
#include "math.h"

#define  PIXEL_NUMS	4       
#define Current_MIN  2692   //ֱ������Сֵ,  DAC_MIN��Ӧ�����������DAC_MAX֮���ڳ���Current_MAX
#define Current_MAX	65535

#define DMX_LEN	50

u16 current[LED_NUMS];
u16 fan_pwm;
float T1,T;
float TEST_DIM;
/*
INPUT: ledk,LED intensit P��dim������0.0 -1.0;pixel�����غ�
*/
//LED �ı�����������
void ChanleDataChange(RGB  *ledk,float dim,unsigned char pixel)
{
	float temp;
	char buf[50];
	unsigned char sum=0;
	unsigned int dac_data[LED_NUMS],pwm_data[LED_NUMS];
	T1= GetSysTime_us()/1000000.0f;
	//dim = dim*0.9;
	dim = pow(dim,1.5);//٤��У��
	TEST_DIM = dim;
  T = GetSysTime_us()/1000000.0f - T1;
	temp = ledk->r + ledk->g + ledk->b + ledk->cw + ledk->ww;//������ϵ��λtemp
	if(temp != 0)
		temp = (float)(dim*(Current_MAX)/temp);//��һ��
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
			current[0] = 	ledk->ww * temp;
			current[1] =  ledk->cw * temp;    
			current[2] =  ledk->r * temp;
			current[3] =  ledk->g * temp;
			current[4] =  ledk->b * temp; 
		break;
		case 3:
		{	
			current[0] = 	ledk->cw * temp;//���
			current[1] =  ledk->r * temp;    
			current[2] =  ledk->g * temp;
			current[3] =  ledk->b * temp;
			current[4] =  ledk->ww * temp; //ů��
			break;
		}
		
	}
}

void ChanleDataSend(u8 addr)
{
	char *p;
	int i;
	u8 sum;
	p = &DMX1_TX_BUF[addr];// addr 1 ,11 chanle
	for(i=0;i<5;i++) //��ַ��һ��ʼ
	{
		p[i*2] = ((u16)current[i])>>8; //�߰�λ
	  p[i*2+1] = ((u16)current[i])&0xff;//�Ͱ�λ
	}
	p[10] = fan_pwm>>8; //�߰�λ
	p[11] = fan_pwm&0xff;//�Ͱ�λ
	for(i=0;i<16;i++)//8ͨ��
	{
		sum += p[i];
	}
	p[i] = sum; //У���
}


/*
INPUT: ledk,LED intensit P��dim������0.0 -1.0;pixel�����غ�
*/
void LedPowerOut(RGB *ledk,float dim,unsigned char pixel)
{
	ChanleDataChange(ledk,dim,3);
	if(pixel==0)
	{
		ChanleDataSend(1);
	}else
	{
		ChanleDataSend(18);
	}
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
	ChanleDataChange(ledk,dim,3);
	ChanleDataSend(1);
	ChanleDataSend(18);
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