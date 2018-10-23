#ifndef __ST7735S_H
#define __ST7735S_H

#include "sys.h"
#include "graphical.h"

#define LCD_RESET			PDout(0)
#define LCD_AO				PDout(1)
#define LCD_BACK			PDout(2)
#define LCD_BACK_ON		PDout(2) = 1
#define LCD_BACK_OFF	PDout(2) = 0

#define LCD_W			160 
#define LCD_H			128

#define SCK					PBout(3)
#define SDA					PBout(5)
#define CS					PAout(15)


void Lcd_Init(void);
void LCD_Clear(u16 Color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
u32 LCD_Pow(u8 m,u8 n);
void LCD_DrawBMP(u16 x,u16 y,BMP *bmp,u16 point_color,u16 back_color);

void LCD_Clear_Test(u16 Color);
void LCD_Set_Display_Mode(u8 mode);

extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];
extern const unsigned int GBK_TAB[];
extern const unsigned char Chinese16X16_Tab[][32];

#endif



