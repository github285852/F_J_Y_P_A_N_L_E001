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
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); //������ӳ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART3ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535; //	�Զ���ת��
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ʱ
	TIM_Cmd(TIM5, ENABLE);  //??TIMx	
	
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
 
	USART_InitStructure.USART_BaudRate = baudrate;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure);  //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

	USART2->CR3 |=  0x80;//ʹ��UART3 DMA����
	DMA1_Channel7->CPAR = (u32)&USART2->DR;
	DMA1_Channel7->CMAR = (u32)DMX1_TX_BUF;
	
	DMA1_Channel7->CCR = 0x0090;    //8λ�������ַ���䣬�洢�����������ȹر�
	DMA1_Channel7->CNDTR = 0;       //����512��
	//DMA1_Channel2->CCR |= 0x01;     //��������
	//DMA1_Channel2->CCR |= 0x02; //����DMA�����ж�
}
// BUF �ڴ������֮ǰ�����ܱ�
void uart3_dma_send(u8 *buf,u16 len)
{
	int retry;
	while((USART3->SR&0X40)==0);//���ͽ���
	while(DMA1_Channel7->CNDTR)//�ȴ���һ�δ������
	{
		retry++;
		if(retry>200)	return;
	}
	DMA1_Channel7->CCR &= ~DMA_CCR1_EN;//�ر�ͨ��
  DMA1_Channel7->CMAR = (u32)buf;
	DMA1_Channel7->CCR = 0x0090;    //8λ�������ַ���䣬�洢�����������ȹر�
	DMA1_Channel7->CNDTR = len;   //����512��
	DMA1_Channel7->CCR |= DMA_CCR1_EN;     //��������
}
// BUF �ڴ������֮ǰ�����ܱ�
void DMX1_Send(u8 *buf,u16 len)
{
	int retry;
	float temp;
	u8 *p = buf+1;
	while(DMA1_Channel2->CNDTR)//�ȴ���һ�δ������
	{
		retry++;
		if(retry>200)	return;
	}
	PDout(4) = 1;//����
	GPIOD->CRL &= ~(0x03<<20); // ͨ���������
	PDout(5) = 0;
	delay_us(190);
//	TIM5->CNT=0;
//	while(TIM5->CNT<100);
	PDout(5) = 1;
	delay_us(12);
	GPIOD->CRL |= (0x02<<20); // �����������
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
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
	}
} 

