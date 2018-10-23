#include "includes.h"
long ec11_pos[3]={0,0,0};
int ec11_speed[3]={0,0,0};
int Ec11Speed[3]={0,0,0};
unsigned char ec11_check=0;
long test_pos;
float EC11_speed = 0;
void ec11_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line13;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	
	//��ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Period = 65535; //	�Զ���ת��  
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ʱ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(TIM5, ENABLE);  //??TIMx	
}

u32 tim5_over;
void TIM5_IRQHandler(void)   //TIM6�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		 tim5_over++;
	}
}
#define speed_a   0.01
#define SLIDE_SIZE	5

void SlideFilter(float *buf,float value)
{
	int i;
	for(i=0;i<SLIDE_SIZE-1;i++)
	{
		buf[i] = buf[i+1];
	}
	buf[i] = value;
}

//ð������
float SORT_BUF[SLIDE_SIZE];
void MinToMaxSort(float *buf)
{

	int i,j;
	float temp;
	for(i=0;i<SLIDE_SIZE;i++)
	{
		SORT_BUF[i] = buf[i];
	}
	for(i=0;i<SLIDE_SIZE-1;i++)
	{
		for(j=0;j<SLIDE_SIZE-1;j++)
		{
		  if(SORT_BUF[j]>SORT_BUF[j+1])
			{
				temp = SORT_BUF[j];
				SORT_BUF[j] = SORT_BUF[j+1];
				SORT_BUF[j+1] = temp;
			}
		}
	}
}

float SLIDE_BUF[SLIDE_SIZE];
void refresh_seepd(void)
{
	u32 ec11_tim;
	static float last,last_t;
	float temp_speed;
	float tmp_p;
	ec11_tim = tim5_over*65535 + TIM5->CNT;
	temp_speed = 1000000.0/ec11_tim;
	tmp_p = GetSysTime_us()/1000000.0 - last_t;
	
	SlideFilter(SLIDE_BUF,temp_speed);
	MinToMaxSort(SLIDE_BUF);
	EC11_speed = SORT_BUF[SLIDE_SIZE/2];//ȡ�м�ֵ
	
	//�����˲�
	EC11_speed = EC11_speed*(1-speed_a) + last*speed_a;
	//EC11_speed = EC11_speed*(1-tmp_p) + last*tmp_p;
	last = EC11_speed;
	tim5_over = 0;
	TIM5->CNT = 0;
	if(tmp_p>1.0)
		EC11_speed = temp_speed;
	last_t = GetSysTime_us()/1000000.0f;
}

void EXTI1_IRQHandler(void)
{
	static u8 falled=0;
	if(ReadT1APin())//������
	{
		if(ReadT1BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			if(falled==2)
			{
				ec11_pos[0]--;
				ec11_check =1;
				ec11_speed[0]++;
			}
		}
		else 
		{
			if(falled==1)
			{
				ec11_pos[0]++;
				ec11_check =1;
				ec11_speed[0]++;
			}
		}
		falled = 0;
	}
	else
	{
		refresh_seepd();
		if(ReadT1BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			falled = 1;
		}
		else 
		{
			falled = 2;
		}
	}
	EXTI->PR = EXTI_Line1;	
}

void EXTI4_IRQHandler(void)
{
	static u8 falled=0;
	if(ReadT2APin())//������
	{
		if(ReadT2BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			if(falled==2)
			{
				ec11_pos[2]--;
				ec11_check =1;
				ec11_speed[2]++;
			}
		}
		else 
		{
			if(falled==1)
			{
				ec11_pos[2]++;
				ec11_check =1;
				ec11_speed[2]++;
			}
		}
		falled = 0;
	}
	else
	{
		refresh_seepd();
		if(ReadT2BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			falled = 1;
		}
		else 
		{
			falled = 2;
		}
	}
		EXTI->PR = EXTI_Line4;	
}

void EXTI15_10_IRQHandler(void)
{
	static u8 falled=0;
	if(ReadT3APin())//������
	{
		if(ReadT3BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			if(falled==2)
			{
				ec11_pos[1]--;
				ec11_check =1;
				ec11_speed[1]++;
			}
		}
		else 
		{
			if(falled==1)
			{
				ec11_pos[1]++;
				ec11_check =1;
				ec11_speed[1]++;
			}
		}
		falled = 0;
	}
	else
	{
		refresh_seepd();
		if(ReadT3BPin())//A���½����жϣ��ж�B��ĸߵ͵�ƽ������ת��
		{
			falled = 1;
		}
		else 
		{
			falled = 2;
		}
	}
		EXTI->PR = EXTI_Line13;	
}

void ecll_speed_duty(void)
{
	if(ec11_speed[0]>2)
	{
		Ec11Speed[0] = ec11_speed[0];
	}
	if(ec11_speed[1]>2)
	{
		Ec11Speed[1] = ec11_speed[1];
	}
	if(ec11_speed[2]>2)
	{
		Ec11Speed[2] = ec11_speed[2];
	}

	ec11_speed[0] = 0;
	ec11_speed[1] = 0;
	ec11_speed[2] = 0;
}

