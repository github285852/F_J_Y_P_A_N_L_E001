#ifndef __DMX512_H
#define __DMX512_H


#define EN_USART2_RX	1


#define DMX_USART											USART3
#define RCC_APB1Periph_DMX_USART			RCC_APB1Periph_USART3
#define RCC_APB1Periph_DMX_TIM				RCC_APB1Periph_TIM6
#define DMX_USART_IRQn								USART3_IRQn

#define DMX_USART_GPIO_REMAP										1
#define GPIO_Remap_DMX_USART					GPIO_FullRemap_USART3

#define DMX_RXEN_GPIO									GPIOD
#define DMX_RX_GPIO										GPIOD
#define DMX_TX_GPIO										GPIOD
#define DMX_RXEN_PIN									GPIO_Pin_12
#define DMX_RX_PIN										GPIO_Pin_9
#define DMX_TX_PIN										GPIO_Pin_8

#define DMX_GPIO_PortSource						GPIO_PortSourceGPIOD
#define DMX_EXTI_PinSource						GPIO_PinSource9
#define DMX_EXTI_Line									EXTI_Line9
#define DMX_EXTI_IRQn									EXTI9_5_IRQn

#define DMX_TIM												TIM6
#define DMX_TIM_IRQn									TIM6_IRQn

#define DMX_USART_IRQHandler					USART3_IRQHandler
#define DMX_EXTI_IRQHandler						EXTI9_5_IRQHandler
#define DMX_TIM_IRQHandler						TIM6_IRQHandler


#define DMX_RXEN				PDout(12) = 0
#define DMX_TXEN				PDout(12) = 1


void dmx512_init(void);

extern unsigned char DMX512_RX_BUF[513];


#endif


