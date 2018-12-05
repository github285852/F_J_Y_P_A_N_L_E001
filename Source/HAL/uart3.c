#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

unsigned char DMX1_TX_BUF[513];
void uart3_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); //部分重映像
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART3时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535; //	自动重转载
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能时
	TIM_Cmd(TIM5, ENABLE);  //??TIMx	
	
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
 
	USART_InitStructure.USART_BaudRate = baudrate;	//波特率设置
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

	USART2->CR3 |=  0x80;//使能UART3 DMA发送
	DMA1_Channel7->CPAR = (u32)&USART2->DR;
	DMA1_Channel7->CMAR = (u32)DMX1_TX_BUF;
	
	DMA1_Channel7->CCR = 0x0090;    //8位，外设地址不变，存储递增，必须先关闭
	DMA1_Channel7->CNDTR = 0;       //传输512次
	//DMA1_Channel2->CCR |= 0x01;     //开启传输
	//DMA1_Channel2->CCR |= 0x02; //开启DMA传输中断
}
// BUF 在传输完成之前，不能变
void uart3_dma_send(u8 *buf,u16 len)
{
	int retry;
	while((USART3->SR&0X40)==0);//发送结束
	while(DMA1_Channel7->CNDTR)//等待上一次传输完成
	{
		retry++;
		if(retry>200)	return;
	}
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN;//关闭通道
  DMA1_Channel7->CMAR = (u32)buf;
	DMA1_Channel7->CCR = 0x0090;    //8位，外设地址不变，存储递增，必须先关闭
	DMA1_Channel7->CNDTR = len;   //传输512次
	DMA1_Channel7->CCR |= DMA_CCR1_EN;     //开启传输
}
// BUF 在传输完成之前，不能变
void DMX1_Send(u8 *buf,u16 len)
{
	int retry;
	float temp;
	u8 *p = buf+1;
	while(DMA1_Channel2->CNDTR)//等待上一次传输完成
	{
		retry++;
		if(retry>200)	return;
	}
	PDout(4) = 1;//发送
	GPIOD->CRL &= 0xFF3FFFFF;// 通用推挽输出
	PDout(5) = 0;
	delay_us(190);
//	TIM5->CNT=0;
//	while(TIM5->CNT<100);
	PDout(5) = 1;
	delay_us(12);
	GPIOD->CRL |= (0x02<<22); // 复用推挽输出
	uart3_dma_send(buf,len);
}
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetFlagStatus(USART2,USART_FLAG_NE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_NE); 
		USART_ReceiveData(USART2);    
	}
	else if(USART_GetFlagStatus(USART2,USART_FLAG_FE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_FE); 
		USART_ReceiveData(USART2);    
	}
	else if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_ORE); 
		USART_ReceiveData(USART2);    
	}	
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		res =USART_ReceiveData(USART2); 	//读取接收到的数据
	}
} 

