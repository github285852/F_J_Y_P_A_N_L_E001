#include "stm32f10x.h"


void timer4_init(u16 period,u16 Prescaler)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
		//��ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Period = period; //	�Զ���ת��
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(TIM7, ENABLE);  //??TIMx	
	
}


void TIM7_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		LcdBackTask();
	}
}