#include "includes.h"

void TIM2_PWM_Init(u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��    RCC_APB2Periph_GPIOB |
 
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);//��ȫ��ӳ��
	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;  //GPIO_Pin_3|GPIO_Pin_10|
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	/*
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
	//��ʼ��TIM2 Channel2 PWMģʽ	 
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
 	//��ʼ��TIM2 Channel3 PWMģʽ	 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
*/
	//��ʼ��TIM2 Channel4 PWMģʽ	 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
  
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
	
	TIM_SetCompare4(TIM2,1);
}

void mbi5041_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM2_PWM_Init(1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_15;				 //PD7�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	SDI_L;
	CLK_L;
	LE_L;
	LE2_L;
}

unsigned int read_status_reg(void)
{
	int i,bits=0;
	unsigned char reg=0;
	CLK_L;
	for(i=0;i<11;i++)
	{
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_H;
	delay_us(1);
	for(;i<16;i++)
	{
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_L;
	delay_us(10);
	if(SDO_IN)
	{
		reg |= 0x01;
	}
	for(i=0;i<15;i++)
	{
		reg <<= 1;
		CLK_H;
		delay_us(10);
		if(SDO_IN)
		{
			reg |= 0x01;
		}
		CLK_L;
		delay_us(10);
	}
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	return reg;
}
/*
void write_status_reg(unsigned int data)
{
	int i;
	//ʹ��д��״̬�Ĵ���
	CLK_H;
	delay_us(10);
	CLK_L;
	delay_us(10);
	LE_H;
	for(i=1;i<16;i++)
	{
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_L;
	
	//��ʼ��������
	for(i=0;i<5;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_H; //д�����ָ��
	for(;i<16;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_L;
}
*/
void write_data(unsigned int data,unsigned char num)
{
	int i;
	//��ʼ��������
	num  = 16 - num;
	for(i=0;i<num;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	LE_H; 
	delay_us(1);
	for(;i<15;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	if(data&0x8000)
	{
		SDI_H;
	}
	else
	{
		SDI_L;
	}
	data <<= 1;
	CLK_H;
	delay_us(10);
	LE_L;
	delay_us(1);
	CLK_L;
	delay_us(10);
}

void write_data2(unsigned int data,unsigned char num)
{
	int i;
	//��ʼ��������
	num  = 16 - num;
	for(i=0;i<num;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	if(num==16)
		return ;
	LE2_H; 
	delay_us(1);
	for(;i<15;i++)
	{
		if(data&0x8000)
		{
			SDI_H;
		}
		else
		{
			SDI_L;
		}
		data <<= 1;
		CLK_H;
		delay_us(10);
		CLK_L;
		delay_us(10);
	}
	if(data&0x8000)
	{
		SDI_H;
	}
	else
	{
		SDI_L;
	}
	data <<= 1;
	CLK_H;
	delay_us(10);
	LE2_L;
	delay_us(1);
	CLK_L;
	delay_us(10);
}

void write_status_reg(unsigned int data)
{
	write_data(0x00,14);
	write_data(data,10);
}

unsigned int test_pwm;
//ͨ��ӳ��
void set_all_pwm(unsigned int **buf)
{
	int i;
	for(i=15;i>0;i--)
	{
		test_pwm = *buf[i];
		write_data(*buf[i],1);
	}
	write_data(*buf[0],2);
	
	//���õڶ�Ƭ5041
	write_data2(*buf[31],0);
	for(i=30;i>15;i--)
	{
		write_data2(*buf[i],1);
	}
	write_data2(*buf[i],2);
}

void mbi5041_set_pwm(unsigned int *buf)
{
	//unsigned int status = 0;
	int i,j;
/*
	status = read_status_reg();
	if(status&0x8000)
	{
		for(i=15;i>=0;i++)
		{
			test_pwm = buf[i];
			write_data(buf[i],1);
		}
		write_data(buf[0],2);
	}
	else
	*/
	 //���õ�һƬ5041
	{
		for(i=15;i>0;i--)
		{
			write_data(buf[i],1);
		}
		write_data(buf[0],2);
	}
	
	//���õڶ�Ƭ5041
	write_data2(buf[31],0);
	for(i=30;i>15;i--)
	{
		write_data2(buf[i],1);
	}
	write_data2(buf[i],2);
}

unsigned char set_current_G(float g)
{
	unsigned char d;
	unsigned int data = 0x00;
	if(g>=1)
	{
		d = (unsigned char)((65*g-33)/3.0);
		data |= 0x01<<9;
		data |= d<<4;
	}
	else
	{
		d = (unsigned char)((256*g-32)/3.0);
		data |= d<<4;
	}
	if(d<32)
	{
		write_status_reg(data);
		return 0;
	}
	return 1;
}

void set_current_D(char d)
{
	unsigned char data=0;
	if(d<64)
	{
		data  = d<<4;
		write_status_reg(data);
	}
}

unsigned int pwm0 = 0;
unsigned int *pwm_table[32] = {
RGBPwm+3, //Y
RGBPwm+4, //W
RGBPwm+2,//b
RGBPwm+1,//g
RGBPwm,//r
	
RGBPwm+3, //Y
RGBPwm+4, //W
RGBPwm+2,//b
RGBPwm+1,//g
RGBPwm,//r
	
RGBPwm+3, //Y
RGBPwm+4, //W
RGBPwm+2,//b
RGBPwm+1,//g
RGBPwm,//r
	
RGBPwm+3, //Y
RGBPwm+4, //W
RGBPwm+2,//b
RGBPwm+1,//g
RGBPwm,//r

&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
&pwm0,
};

void mbi5041_test(void)
{
	set_all_pwm(pwm_table);
}

void  PWM_Test(void)
{
	int i;
	memset(RGBPwm,0,sizeof(pRGBPwm));
	for(i=0;i<5;i++)
	{
		RGBPwm[i] = 60000;
		set_all_pwm(pwm_table);
		delay_ms(1000);
		memset(RGBPwm,0,sizeof(pRGBPwm));
		set_all_pwm(pwm_table);
		delay_ms(1000);
	}

}