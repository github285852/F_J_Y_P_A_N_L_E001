#ifndef __DEBUG_H
#define __DEBUG_H




#define DEBUG_USART											UART4
#define RCC_APB1Periph_DEBUG_USART			RCC_APB1Periph_UART4
#define DEBUG_USART_IRQn								UART4_IRQn

#define GPIO_REMAP											0
#define GPIO_Remap_DEBUG_USART					GPIO_FullRemap_UART4

#define DEBUG_RX_GPIO										GPIOC
#define DEBUG_TX_GPIO										GPIOC
#define DEBUG_RX_PIN										GPIO_Pin_11
#define DEBUG_TX_PIN										GPIO_Pin_10

#define DEBUG_USART_IRQHandler					UART4_IRQHandler

void Debug_init(void);
void Debug_printf(char* fmt,...);
void uart_duty(void);

#endif
