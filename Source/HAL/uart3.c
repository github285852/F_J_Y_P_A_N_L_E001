#include "stm32f10x.h"
#include "sys.h"

unsigned char DMX1_TX_BUF[513];
void uart3_init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE); //������ӳ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535; //	�Զ���ת��
	TIM_TimeBaseStructure.TIM_Prescaler = 71; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ʱ
	TIM_Cmd(TIM5, ENABLE);  //??TIMx	
	
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ
 
	USART_InitStructure.USART_BaudRate = baudrate;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure);  //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

	USART3->CR3 |=  0x80;//ʹ��UART3 DMA����
	DMA1_Channel2->CPAR = (u32)&USART3->DR;
	DMA1_Channel2->CMAR = (u32)DMX1_TX_BUF;
	
	DMA1_Channel2->CCR = 0x0090;    //8λ�������ַ���䣬�洢�����������ȹر�
	DMA1_Channel2->CNDTR = 0;       //����512��
	//DMA1_Channel2->CCR |= 0x01;     //��������
	//DMA1_Channel2->CCR |= 0x02; //����DMA�����ж�
}
// BUF �ڴ������֮ǰ�����ܱ�
void uart3_dma_send(u8 *buf,u16 len)
{
	int retry;
	while((USART3->SR&0X40)==0);//���ͽ���
	while(DMA1_Channel2->CNDTR)//�ȴ���һ�δ������
	{
		retry++;
		if(retry>200)	return;
	}
	DMA1_Channel2->CCR &= ~DMA_CCR1_EN;//�ر�ͨ��
  DMA1_Channel2->CMAR = (u32)buf;
	DMA1_Channel2->CCR = 0x0090;    //8λ�������ַ���䣬�洢�����������ȹر�
	DMA1_Channel2->CNDTR = len;   //����512��
	DMA1_Channel2->CCR |= DMA_CCR1_EN;     //��������
}
// BUF �ڴ������֮ǰ�����ܱ�
void DMX1_Send(u8 *buf,u16 len)
{
	int retry;
	int i;
	float temp;
	u8 *p = buf+1;
//	for(i=0;i<5;i++)
//	{
//		temp = p[2*i+1] |(p[2*i]<<8);
//		temp = 900 * temp /65535.0;
//		Debug_printf("I%d = %.2f\r\n",i,temp);
//	}
	while(DMA1_Channel2->CNDTR)//�ȴ���һ�δ������
	{
		retry++;
		if(retry>200)	return;
	}
	GPIOC->CRH &= ~(0x03<<10); // ͨ���������
	PCout(10) = 0;
	delay_us(190);
//	TIM5->CNT=0;
//	while(TIM5->CNT<100);
	PCout(10) = 1;
	delay_us(12);
	GPIOC->CRH |= (0x02<<10); // �����������
	uart3_dma_send(buf,len);
}
void USART3_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		res =USART_ReceiveData(USART3); 	//��ȡ���յ�������

	} 
} 

