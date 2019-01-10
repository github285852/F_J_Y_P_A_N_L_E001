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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DMX_USART,ENABLE);//使能DMX_USART时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DMX_TIM, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使用复用功能时钟
	#if DMX_USART_GPIO_REMAP
	GPIO_PinRemapConfig(GPIO_Remap_DMX_USART,ENABLE);
	#endif
	GPIO_InitStructure.GPIO_Pin = DMX_RXEN_PIN;				 //PA12端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DMX_RXEN_GPIO, &GPIO_InitStructure);
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin =	DMX_TX_PIN; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(DMX_TX_GPIO, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = DMX_RX_PIN;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(DMX_RX_GPIO, &GPIO_InitStructure);//初始化GPIOA.10  
	
	GPIO_EXTILineConfig(DMX_GPIO_PortSource,DMX_EXTI_PinSource);
	EXTI_InitStructure.EXTI_Line= DMX_EXTI_Line;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMX_EXTI_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = 65535; //	自动重转载
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(DMX_TIM, &TIM_TimeBaseStructure); //
	TIM_ITConfig(DMX_TIM,TIM_IT_Update,ENABLE ); //使能时钟
	NVIC_InitStructure.NVIC_IRQChannel = DMX_TIM_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(DMX_TIM, ENABLE);  //??TIMx	
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DMX_USART,ENABLE);//复位串口
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DMX_USART,DISABLE);//停止复位
 
	USART_InitStructure.USART_BaudRate = 250000;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//两个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

  USART_Init(DMX_USART, &USART_InitStructure);  //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = DMX_USART_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(DMX_USART, USART_IT_RXNE, ENABLE);//开中断
	USART_Cmd(DMX_USART, ENABLE);                    //使能串口 

	DMX_RXEN;	//默认为接收模式
}

u16 tim;
void DMX_EXTI_IRQHandler(void)
{
	static u8 last_sum;
	 //if(PAin(2)==0)//
	if(DMX_RX_GPIO->IDR&DMX_RX_PIN)//上升沿
	{
			break_tim = tim_over*65535 + DMX_TIM->CNT;
		  if((break_tim>=88)&&(break_tim<=1000000))
			{
				receiving_dmx_data();
				
//				if(USART_GetITStatus(DMX_USART, USART_IT_RXNE) != RESET) //接收到数据
//				{	 
//					USART_ReceiveData(DMX_USART); 	//读取接收到的数据
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
		DMX_TIM->CNT=0;
	}
	EXTI->PR = DMX_EXTI_Line;	
}

void DMX_TIM_IRQHandler(void)   //TIM6中断
{
	if (TIM_GetITStatus(DMX_TIM, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(DMX_TIM, TIM_IT_Update);  //清除TIMx更新中断标志 
		tim_over++;
	}
}


void DMX_USART_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetFlagStatus(DMX_USART,USART_FLAG_NE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_NE); 
		USART_ReceiveData(DMX_USART);    
	}
	else if(USART_GetFlagStatus(DMX_USART,USART_FLAG_FE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_FE); 
		USART_ReceiveData(DMX_USART);    
	}
	else if(USART_GetFlagStatus(DMX_USART,USART_FLAG_ORE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_ORE); 
		USART_ReceiveData(DMX_USART);    
	}	
	else if(USART_GetITStatus(DMX_USART, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		res =USART_ReceiveData(DMX_USART); 	//读取接收到的数据
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

