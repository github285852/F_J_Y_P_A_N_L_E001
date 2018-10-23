#ifndef __GRAPHICAL_H
#define __GRAPHICAL_H

#include "sys.h"

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 	0XC618 //浅灰色(PANNEL),窗体背景色
#define DGRAY     		0x7BEF    //      深灰色  127, 127, 127 
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


typedef struct { u16 x0,y0,x1,y1; } RECT;
typedef struct
{
	u16 *data; //数据指针
	u16 w;		//  宽度
	u16 h;     //高度
}Picture;

typedef struct
{
	u8 *data; //数据指针
	u16 w;		//  宽度
	u16 h;     //高度
}BMP;

void LCD_Fill_Picture(u16 x,u16 y,Picture pic);
void Picture_Fill(Picture pic,u16 color);
void Picture_ShowString(Picture pic,u16 x,u16 y,u16 width,u16 height,u8 size,char *p,u8 mode);
void Picture_ShowStringInRectCenter(Picture pic,RECT rect,u8 mode,u8 size,char *p);
void Picture_FillRect(Picture pic,RECT rect,u16 color);
void Picture_ShowNum(Picture pic,u16 x,u16 y,long num,u8 len,u8 size);
void Picture_FillCircle(Picture pic,u8 x, u8 y,u8 r,u16 color);
void Picture_FillTrigon(Picture pic,u16 x,u16 y,u8 size,u16 color);
void Picture_DrawTrigon(Picture pic,u16 x,u16 y,u8 h,u8 d,u16 color);
void Picture_DrawLine(Picture pic,u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void Picture_DrawInvertedTrigon(Picture pic,u16 x,u16 y,u8 h,u8 d,u16 color);
void Picture_DrawBMP(Picture pic,u16 x,u16 y,BMP *bmp,u16 point_color,u16 back_color);
void Picture_ShowChar(Picture pic,u16 x,u16 y,u8 num,u8 size,u8 mode);
void Pictrue_printf(Picture *pic,u16 x,u16 y,u8 size,char* fmt,...);
void Picture_TranDispalyOnline(Picture *pic,RECT *rect,u8 *pos,u8 size,u8 mode,char *p);

#endif



