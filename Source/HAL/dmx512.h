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

void dmx512_init(void);
void uart_duty(void);
void rs485_send_str(unsigned char *str);

extern u8 DMX512_RX_BUF[513];


#endif


