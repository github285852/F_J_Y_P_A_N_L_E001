#include "stm32f10x.h"
#include "scene.h"

void timer4_init(u16 period,u16 Prescaler)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
		//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = period; //	自动重转载
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	TIM_Cmd(TIM7, ENABLE);  //??TIMx	
	
}


void TIM7_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx更新中断标志 
		LcdBackTask();
	}
}

void SceneTimerInit()
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
		//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = 100; //	自动重转载
	TIM_TimeBaseStructure.TIM_Prescaler = 36000; //   2分频，1K
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV2; //
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能时钟
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
//	  //??TIMx	
}


void SceneSetframeTim(u16 ms)
{
	if(ms<=FRAME_MIN_T)
		ms = FRAME_MIN_T;
	TIM4->ARR = ms;
}
void ClearCNT(void)
{
	TIM4->CNT = 0;
}
void SceneTimEnable(void)
{
	//TIM4->CNT = 0;
	TIM_Cmd(TIM4, ENABLE);
}

void SceneTimDisable(void)
{
	//TIM4->CNT = 0;
	TIM_Cmd(TIM4, DISABLE);
}

