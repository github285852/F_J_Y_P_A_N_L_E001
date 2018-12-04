#ifndef __DMX512_H
#define __DMX512_H


#define EN_USART2_RX	1
//typedef struct
//{
//	unsigned char kvn;//É«ÎÂ
//	unsigned char grn;//Æ«ÒÆ
//	unsigned char dim;//ÁÁ¶È
//}DMX_CCT_8;

//typedef struct
//{
//	unsigned int kvn;//É«ÎÂ
//	unsigned int grn;//Æ«ÒÆ
//	unsigned int dim;//ÁÁ¶È
//}DMX_CCT_16;

//typedef struct
//{
//	unsigned char kvn;//É«ÎÂ
//	unsigned char grn;//Æ«ÒÆ
//	unsigned char dim;//ÁÁ¶È
//	unsigned char fan;
//}DMX_CCT_FAN_8;

//typedef struct
//{
//	unsigned int kvn;//É«ÎÂ
//	unsigned int grn;//Æ«ÒÆ
//	unsigned int dim;//ÁÁ¶È
//	unsigned int fan;
//}DMX_CCT_FAN_16;

//typedef struct
//{
//	unsigned char hub;
//	unsigned char sat;
//	unsigned char dim;
//}DMX_HSI_8;

//typedef struct
//{
//	unsigned int hub;//É«ÎÂ
//	unsigned int sat;//Æ«ÒÆ
//	unsigned int dim;//ÁÁ¶È
//}DMX_HSI_16;

//typedef struct
//{
//	unsigned char hub;
//	unsigned char sat;
//	unsigned char dim;
//	unsigned char fan;
//}DMX_HSI_FAN_8;

//typedef struct
//{
//	unsigned int hub;
//	unsigned int sat;
//	unsigned int dim;
//	unsigned int fan;
//}DMX_HSI_FAN_16;

//typedef struct
//{
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//}DMX_RGB_8;

//typedef struct
//{
//	unsigned int r;
//	unsigned int g;
//	unsigned int b;
//}DMX_RGB_16;

//typedef struct
//{
//	unsigned char r;
//	unsigned char g;
//	unsigned char b;
//	unsigned char fan;
//}DMX_RGB_FAN_8;

//typedef struct
//{
//	unsigned int r;
//	unsigned int g;
//	unsigned int b;
//	unsigned int fan;
//}DMX_RGB_FAN_16;

//typedef struct
//{
//	unsigned char num;
//}DMX_GEL;

//typedef struct
//{
//	unsigned char num;
//	unsigned char fan;
//}DMX_GEL_FAN;

//typedef struct
//{
//	unsigned char num;
//}DMX_SCENE;

//typedef struct
//{
//	unsigned char num;
//	unsigned char fan;
//}DMX_SCENE_FAN;

//typedef union 
//{
//	DMX_CCT_8 cct_8;
//	DMX_CCT_16 cct_16;
//	DMX_CCT_FAN_8 cct_fan_8;	
//	DMX_CCT_FAN_16 cct_fan_16;	
//	
//	DMX_HSI_8 hsi_8;
//	DMX_HSI_16 hsi_16;
//	DMX_HSI_FAN_8 hsi_fan_8;	
//	DMX_HSI_FAN_16 hsi_fan_16;
//	
//	DMX_RGB_8 rgb_8;
//	DMX_RGB_16 rgb_16;
//	DMX_RGB_FAN_8 rgb_fan_8;	
//	DMX_RGB_FAN_16 rgb_fan_16;
//	
//	DMX_GEL gel;
//	DMX_GEL_FAN gel_fan;
//	
//	DMX_SCENE scene;
//	DMX_SCENE_FAN scene_fan;
//	
//}DMXData;

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


