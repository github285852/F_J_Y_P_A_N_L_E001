#include "includes.h"
#include "string.h"
u32 tim_over=0;
u32 break_tim;
//DMXData *DmxData;
//接收缓存区 	
u8 DMX512_RX_BUF[513];  	//接收缓冲,最大513个字节.
//接收到的数据长度
u16 DMX512_RX_CNT=0;   	
u8 TEST_SUM;
u32 SUM_CNT;
void wait_receive_dmx_data(void);
void receiving_dmx_data(void);

void dmx512_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使用复用功能时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA12端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.10  
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line=EXTI_Line6;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = 65535; //	自动重转载
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能时钟
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(TIM6, ENABLE);  //??TIMx	
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
 
	USART_InitStructure.USART_BaudRate = 250000;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//两个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

  USART_Init(USART2, &USART_InitStructure);  //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 

	PDout(4) = 0;	//默认为接收模式
}

u16 tim;
void EXTI9_5_IRQHandler(void)
{
	static u8 last_sum;
	 //if(PAin(2)==0)//
	if(GPIOD->IDR&0x40)//上升沿
	{
			break_tim = tim_over*65535 + TIM6->CNT;
		  if((break_tim>=88)&&(break_tim<=1000000))
			{
				receiving_dmx_data();
				
//				if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
//				{	 
//					USART_ReceiveData(USART2); 	//读取接收到的数据
//				}
				if(last_sum != TEST_SUM)
					SUM_CNT++;
				last_sum = TEST_SUM;
				TEST_SUM = 0;
				tim = break_tim;
				DMX512_RX_CNT =0;
				//屏蔽外部中断10
				//EXTI->IMR &= 0xffffffbf;
				/*
				处理上一帧4us,如果处理不了，先保存自己的缓存数据
				
				*/
			}
	}
	else //下降沿
	{
		tim_over = 0;
		TIM6->CNT=0;
	}
	EXTI->PR = EXTI_Line6;	
}

void TIM6_IRQHandler(void)   //TIM6中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志 
		tim_over++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////接收处理
void DMX512_ISR(void)
{
//	Sys.dmx_handle = 1;
//	Sys.Config.lightmode = (LightMode)DMX512_RX_BUF[Sys.Config.dmx.addr];
//	DmxData = (DMXData *)(&DMX512_RX_BUF[Sys.Config.dmx.addr+1]);
//	if(Sys.Config.fan != FAN_DMX)
//	{
//		switch(Sys.Config.lightmode)
//		{
//			case CCT_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.cct.kvn = (unsigned int)(2500 + 7500*DmxData->cct_16.kvn/65535);
//					Sys.Config.cct.grn = (char)(-100 + 200*DmxData->cct_16.grn/65535);
//					Sys.Config.cct.dim = (float)(DmxData->cct_16.dim/65535.0);
//				}
//				else
//				{
//					Sys.Config.cct.kvn = (unsigned int)(2500 + 7500*DmxData->cct_8.kvn/255);
//					Sys.Config.cct.grn = (char)(-100 + 200*DmxData->cct_8.grn/255);
//					Sys.Config.cct.dim = (float)(DmxData->cct_8.dim/255);
//				}
//			}
//			break;
//			case HSI_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.hsi.h = (int)(359*DmxData->hsi_16.hub/65535);
//					Sys.Config.hsi.s = (float)(DmxData->hsi_16.sat/65535);
//					Sys.Config.hsi.i = (float)(DmxData->hsi_16.dim/65535);
//				}
//				else
//				{
//					Sys.Config.hsi.h = (int)(359*DmxData->hsi_8.hub/255);
//					Sys.Config.hsi.s = (float)(DmxData->hsi_8.sat/255);
//					Sys.Config.hsi.i = (float)(DmxData->hsi_8.dim/255);
//				}
//			}
//			break;
//			case RGB_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.rgb.r = DmxData->rgb_16.r;
//					Sys.Config.rgb.g = DmxData->rgb_16.g;
//					Sys.Config.rgb.b = DmxData->rgb_16.b;
//				}
//				else
//				{
//					Sys.Config.rgb.r = DmxData->rgb_8.r*65535/255;
//					Sys.Config.rgb.g = DmxData->rgb_8.g*65535/255;
//					Sys.Config.rgb.b = DmxData->rgb_8.b*65535/255;
//				}
//			}
//			break;
//			case GEL_M:
//			{
//				Sys.Config.gel = DmxData->gel.num;
//			}
//			break;
//			case SCENE_M:
//			{
//				Sys.Config.scene = DmxData->scene.num;
//			}
//			break;
//		}
//	}
//	else
//	{
//		switch(Sys.Config.lightmode)
//		{
//			case CCT_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.cct.kvn = 2500 + 7500*DmxData->cct_fan_16.kvn/65535;
//					Sys.Config.cct.grn = -100 + 200*DmxData->cct_fan_16.grn/65535;
//					Sys.Config.cct.dim = DmxData->cct_fan_16.dim/65535;
//					Sys.fan_pwm =  DmxData->cct_fan_16.fan;
//				}
//				else
//				{
//					Sys.Config.cct.kvn = 2500 + 7500*DmxData->cct_fan_8.kvn/255;
//					Sys.Config.cct.grn = -100 + 200*DmxData->cct_fan_8.grn/255;
//					Sys.Config.cct.dim = DmxData->cct_fan_8.dim/255;
//					Sys.fan_pwm =  65535*DmxData->cct_fan_8.fan/255;
//				}
//			}
//			break;
//			case HSI_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.hsi.h = 359*DmxData->hsi_fan_16.hub/65535;
//					Sys.Config.hsi.s = DmxData->hsi_fan_16.sat/65535;
//					Sys.Config.hsi.i = DmxData->hsi_fan_16.dim/65535;
//					Sys.fan_pwm =  DmxData->hsi_fan_16.fan;
//				}
//				else
//				{
//					Sys.Config.hsi.h = 359*DmxData->hsi_fan_8.hub/255;
//					Sys.Config.hsi.s = DmxData->hsi_fan_8.sat/255;
//					Sys.Config.hsi.i = DmxData->hsi_fan_8.dim/255;
//					Sys.fan_pwm =  65535*DmxData->hsi_fan_8.fan/255;
//				}
//			}
//			break;
//			case RGB_M:
//			{
//				if(Sys.Config.dmx._16bits)
//				{
//					Sys.Config.rgb.r = DmxData->rgb_fan_16.r;
//					Sys.Config.rgb.g = DmxData->rgb_fan_16.g;
//					Sys.Config.rgb.b = DmxData->rgb_fan_16.b;
//					Sys.fan_pwm =  DmxData->rgb_fan_16.fan;
//				}
//				else
//				{
//					Sys.Config.rgb.r = DmxData->rgb_fan_8.r*65535/255;
//					Sys.Config.rgb.g = DmxData->rgb_fan_8.g*65535/255;
//					Sys.Config.rgb.b = DmxData->rgb_fan_8.b*65535/255;
//					Sys.fan_pwm =  65535*DmxData->rgb_fan_8.fan/255;
//				}
//			}
//			break;
//			case GEL_M:
//			{
//				Sys.Config.gel = DmxData->gel_fan.num;
//				Sys.fan_pwm =  65535*DmxData->gel_fan.fan/255;
//			}
//			break;
//			case SCENE_M:
//			{
//				Sys.Config.scene = DmxData->scene_fan.num;
//				Sys.fan_pwm =  65535*DmxData->scene_fan.fan/255;
//			}
//			break;
//		}
//	}
}

#define USART_REC_LEN  			200  
u8  USART_RX_BUF[USART_REC_LEN]; 
u16 USART_RX_STA;  

void uart_resiver(u8 Res)
{
	if((USART_RX_STA&0x8000)==0)
	{
		if(USART_RX_STA&0x4000)
		{
			if(Res!=0x0a)USART_RX_STA=0;
			else USART_RX_STA|=0x8000;	
		}
		else 
		{	
			if(Res==0x0d)
				USART_RX_STA|=0x4000;
			else
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))
					USART_RX_STA=0;  
			}		 
		}
	} 
}
void rs485_send_str(unsigned char *str)
{
	PDout(4) = 1;
	while(*str != 0)
	{
		while((USART2->SR&0X40)==0);//????,??????   
    USART2->DR = *str;     
		str++;
	}		
	while((USART2->SR&0X40)==0);//????,??????   
	PDout(4) = 0;
}

unsigned int pwm[32]={0,0};
void uart_duty(void)
{
	unsigned char *p,*base_p;
	unsigned char cmd;
	unsigned char buf[50];
	unsigned int h;
	unsigned char s,i;
	unsigned temp;
	float temp_f;
	u16 temp_u16;
	if(USART_RX_STA&0x8000)
	{
		USART_RX_BUF[USART_RX_STA&0X3FFF]=0;
		p = strstr((const char*)USART_RX_BUF,(const char*)"PWM");
		if(p != NULL)
		{
			p += 3;
			cmd = atoi((const char*)p);
			if(cmd<32)
			{
				p = strstr((const char*)USART_RX_BUF,(const char*)":");
				if(p != NULL)
				{
					pwm[cmd] = atoi((const char*)(p+1));
				//	mbi5041_set_pwm(pwm);

				}
				Debug_printf("PWM%d=%d Done\r\n",cmd,pwm[cmd]);
			}
			else
				Debug_printf("ERROR\r\n");
		}
		p = strstr((const char*)USART_RX_BUF,(const char*)"HSI:");
		if(p!=NULL)
		{
			h = atoi((const char*)(p+4));
			if(h>=0&&(h<=360))
			{
				p = strstr((const char*)p,(const char*)",");
				if(p!=NULL)
				{
					s = atoi((const char*)(p+1));
					if((s>=0)&&(s<=100))
					{
						p = strstr((const char*)(p+1),(const char*)",");
						if(p!=NULL)
						{
							i = atoi((const char*)(p+1));
							if((i>=0)&&(i<=100))
							{
								Sys.Config.hsi.h = h;
								Sys.Config.hsi.s =(float)(s/100.0);
								Sys.Config.hsi.i =(float)(i/100.0);
								ColorLightHSIOut(Sys.Config.hsi,0);
							}
							else
							{
								Debug_printf("I ERROR\r\n");
							}
						}
					}
					else
					{
						Debug_printf("S ERROR\r\n");
					}
				}
			}
			else
			{
				Debug_printf("H ERROR\r\n");
			}
		}
		
		USART_RX_STA=0;  
	}
}

void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetFlagStatus(USART2,USART_FLAG_NE)==SET)
	{
		//USART_ClearFlag(USART2,USART_FLAG_NE); 
		USART_ReceiveData(USART2);    
	}
	else if(USART_GetFlagStatus(USART2,USART_FLAG_FE)==SET)
	{
		//USART_ClearFlag(USART2,USART_FLAG_FE); 
		USART_ReceiveData(USART2);    
	}
	else if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		//USART_ClearFlag(USART2,USART_FLAG_ORE); 
		USART_ReceiveData(USART2);    
	}	
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		res =USART_ReceiveData(USART2); 	//读取接收到的数据
		//uart_resiver(res);
		TEST_SUM += res;
		if(DMX512_RX_CNT<513)
		{
			DMX512_RX_BUF[DMX512_RX_CNT]=res;		//记录接收到的值
			DMX512_RX_CNT++;						//接收数据增加1 
			if(DMX512_RX_CNT==513)
			{
				DMX512_RX_CNT = 0;
				//wait_receive_dmx_data();
				
				//打开外部中断10
				//	EXTI->IMR |= 0x00000040;
			}
		}
	}

} 

u32 baude=250000,baude_last=250000;
void change_baude(void)
{
	if(baude_last!=baude)
	{
		USART_InitTypeDef USART_InitStructure;
		USART_InitStructure.USART_BaudRate = baude;	//波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
		USART_InitStructure.USART_StopBits = USART_StopBits_2;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

		USART_Init(USART1, &USART_InitStructure);  //初始化串口
		baude_last = baude;
	}
}




