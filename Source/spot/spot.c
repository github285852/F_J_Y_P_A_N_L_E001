
#include "includes.h"

//����ת�������ϵ��
polyfit ledfit[5]=
{
{0.0000352531387,-0.00634792071,3.19992143,-2.36879183},   //R
{0.00000484692672,-0.00029122039,1.60218601,-3.36593123},	 //A
{0.000000281251497,0.000543083308,1.07424873,-1.54811916}, //L
{0.00000130881024,0.000568619025,0.799767132,-1.71737890}, //G
{-0.00027246580,0.0567195376,7.75389446,1.10819537}				 //B
};

//����ת������ϵ��(�������Ǹ����ֵ��
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
	{LEDAX_DEFAULT,LEDAY_DEFAULT},//A
	{LEDLX_DEFAULT,LEDLY_DEFAULT},//L
	{LEDGX_DEFAULT,LEDGY_DEFAULT},//G
	{LEDBX_DEFAULT,LEDBY_DEFAULT},//B
};


void DMXChanleDataRefresh(u8 pixel,u16 *current)
{
	char *p;
	int i;
	u16 out_current[LED_CHS];
	u8 sum=0;
	DMX1_TX_BUF[0] = 0;
	p = (char *) &DMX1_TX_BUF[1];
	if(current != NULL)
	{
		out_current[0] =  current[GCH];  //A
		out_current[1] =  current[LCH];  //G
		out_current[2] =  current[BCH];  //L
		out_current[3] =  current[ACH];  //B
		out_current[4] =  current[RCH];  //R
		for(i=0;i<5;i++) //��ַ��һ��ʼ
		{
			p[i*2] = ((u16)out_current[i])>>8; //�߰�λ
			p[i*2+1] = ((u16)out_current[i])&0xff;//�Ͱ�λ
		}
	}
	p[10] = fan_pwm>>8; //�߰�λ
	p[11] = fan_pwm&0xff;//�Ͱ�λ
	for(i=0;i<16;i++)//8ͨ��
	{
		sum += p[i];
	}
	p[i] = sum; //У���
}

