#ifndef __GRAPHICAL_H
#define __GRAPHICAL_H

#include "sys.h"

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 	0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define DGRAY     		0x7BEF    //      ���ɫ  127, 127, 127 
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


typedef struct { u16 x0,y0,x1,y1; } RECT;
typedef struct
{
	u16 *data; //����ָ��
	u16 w;		//  ���
	u16 h;     //�߶�
}Picture;

typedef struct
{
	u8 *data; //����ָ��
	u16 w;		//  ���
	u16 h;     //�߶�
}BMP;

typedef struct
{
	u8 move_t;
	u8 cnt;
	u8 pos;
}TRANDISPLAY;

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
void Picture_TranDispalyOnline(Picture *pic,RECT *rect,TRANDISPLAY *tran,u8 size,u8 mode,char *p);
#endif



