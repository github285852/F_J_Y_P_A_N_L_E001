#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "stm32f10x.h"

#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "stdlib.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

#include "sys.h"
#include "delay.h"
#include "scheduler.h"
#include "pwm16.h"
#include "dmx512.h"
#include "led.h"
#include "spi.h"
#include "adc.h"
#include "uart3.h"
#include "stmflash.h"

#include "malloc.h"
#include "ec11.h"
#include "tm1829.h"
#include "mcp4728.h"
#include "ucs8903.h"
#include "menu.h"
#include "key.h"
#include "color_light.h"
#include "ledpower.h"

#include "graphical.h"
#include "st7735s.h"
#include "menu.h"
#include "menutask.h"
#include "menu_handle.h"
#include "picture_data.h"
#include "gui.h"
#include "dmxprotocol.h"


#define DEBUG			0
#define LED_NUMS	5



/*
FLASH 

      |    bootloader |  config |   app
  0x08000000
 
*/
#define APP_ADDR_OFFSET				0xC800        //50k
#define CNFIG_DATA_SIZE				0x2800				//10k

#define STM32_FLASH_APP_BASE   (STM32_FLASH_BASE + APP_ADDR_OFFSET)             //偏移量 0x200的倍数
#define CONFIG_ADDRESS				 (STM32_FLASH_APP_BASE - CNFIG_DATA_SIZE)        // 

#define FIRMWARE_NAME								"filmgear001.bin"

typedef enum 
{
	CCT_M = 0,
	HSI_M,
	GEL_M,
	RGB_M,
	SCENE_M,
	DMX_M
}LightMode;

typedef enum 
{
	FAN_AUTO,
	FAN_OFF,
	FAN_DMX,
}FAN;

typedef struct 
{
	unsigned char tim;//背光时间
	unsigned char dim;//背光亮度
	unsigned char rotate;
}Lcd;//显示设置


typedef struct 
{
	u16 update;//放在第一个
	HSI hsi;
	COORD coord;
	struct
	{
		unsigned int r;
		unsigned int g;
		unsigned int b;
	}rgb;
	unsigned char gel;
	unsigned char scene;//场景
	LightMode lightmode;
	FAN fan;
  Lcd lcd;
	CCT cct;
	unsigned char english;
	struct
	{
		unsigned char dhcp; //自动获取IP
		unsigned char ip[4];
		unsigned char netmask[4];
		unsigned char gateway[4];
	}ArtNet;
	struct
	{
		unsigned char addr;
		unsigned char _16bits;
		unsigned char mode;
	}dmx;
	u8 Art_net_en;
	u32 check;
}CONFIG;

typedef struct{
	uint8_t USBH_USR_ApplicationState;
	uint8_t mass_connect;
	uint8_t update_state;
}USB_STATE;

typedef struct
{
	RGB rgbw;
	unsigned int fan_pwm;
	unsigned char dmx_handle;
	unsigned char dmx_connect;
	unsigned char dmx_insert;
	unsigned char last_light_mode;
	float panle_t;
	USB_STATE usb;
	unsigned char lcd_back_on;
	int save_cnt; //自动灭屏和保存数据计数
	CONFIG Config;
}SYS;


extern SYS Sys;
extern __ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
extern __ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;
extern CONFIG default_data;
extern Picture pic;
extern unsigned char gImage_filmgear_data[40960];
void FAN_OUT(unsigned int PWM);
void Debug_printf(char* fmt,...);

#endif

