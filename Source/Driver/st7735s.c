//#include "sys.h"
//#include "st7735s.h"
//#include "spi.h"
//#include "delay.h"
#include "font.h"
#include "gbk_font.h"
//#include "malloc.h"
#include "math.h"
#include "includes.h"

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
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


extern u8 DMAING;
u16 *LCD_BUF;//零时数据存放，在DMA传输完成后释放

u16  POINT_COLOR = RED;//默认红色    
u16  BACK_COLOR = WHITE; //背景颜色.默认为白色

void LCD_WR_DATA8(char data) //发送数据-8位参数
{
	while(DMAING);
  LCD_AO = 1;
	CS = 0;
	SPI3_ReadWriteByte(data);
}  

void LCD_WR_DATA(int data)
{
	LCD_WR_DATA8(data>>8);
	LCD_WR_DATA8(data);
}
 
void LCD_WR_REG(char reg)	 
{
	while(DMAING);
	CS = 0;
  LCD_AO=0;
	SPI3_ReadWriteByte(reg);
}

void LCD_WR_REG_DATA(int reg,int da)
{
  LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
	x1++;
	y1+=2;
	x2++;
	y2+=2;
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);
}

void Lcd_Init(void)
{
	SPI3_init();
	LCD_BACK =0;
	LCD_RESET=0;
	delay_ms(20);
	LCD_RESET = 1;
	delay_ms(20);
	LCD_WR_REG(0x11); //Sleep out
	delay_ms(120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);
//	LCD_WR_REG(0x36); //MX, MY, RGB mode
//	LCD_WR_DATA8(0xA0);

	LCD_Set_Display_Mode(Sys.Config.lcd.rotate);

	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); //Display on

}


//void Lcd_Init(void)
//{
//	SPI3_init();
//	//LCD_BACK =0;
//	LCD_RESET=0;
//	delay_ms(20);
//	LCD_RESET = 1;
//	delay_ms(20);
//	LCD_WR_REG(0x11); //Sleep out
//	delay_ms(120); //Delay 120ms
//	CS =0;
//	//------------------------------------ST7735S Frame Rate-----------------------------------------//
//	LCD_WR_REG(0xB1);
//	LCD_WR_DATA8(0x01);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x05);

//	LCD_WR_REG(0xB2);
//	LCD_WR_DATA8(0x01);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x05);
//	
//	LCD_WR_REG(0xB3);
//	LCD_WR_DATA8(0x01);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x05);
//	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
//	LCD_WR_REG(0xB4); //Dot inversion
//	LCD_WR_DATA8(0x03);
//	
//	LCD_WR_REG(0xC0);
//	LCD_WR_DATA8(0x28);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x04);
//	
//	LCD_WR_REG(0xC1);
//	LCD_WR_DATA8(0XC0);
//	
//	LCD_WR_REG(0xC2);
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x00);
//	
//	LCD_WR_REG(0xC3);
//	LCD_WR_DATA8(0x8D);
//	LCD_WR_DATA8(0x2A);
//	
//	LCD_WR_REG(0xC4);
//	LCD_WR_DATA8(0x8D);
//	LCD_WR_DATA8(0xEE);
//	//---------------------------------End ST7735S Power Sequence-------------------------------------//
//	LCD_WR_REG(0xC5); //VCOM
//	LCD_WR_DATA8(0x1D);
////	LCD_WR_REG(0x36); //MX, MY, RGB mode
////	LCD_WR_DATA8(0xA0);
//		LCD_Set_Display_Mode(0);
//	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
//	LCD_WR_REG(0xE0);
//	LCD_WR_DATA8(0x07);   
//	LCD_WR_DATA8(0x17);   
//	LCD_WR_DATA8(0x0C);   
//	LCD_WR_DATA8(0x15);   
//	LCD_WR_DATA8(0x2E);   
//	LCD_WR_DATA8(0x2A);   
//	LCD_WR_DATA8(0x23);   
//	LCD_WR_DATA8(0x28);   
//	LCD_WR_DATA8(0x28);   
//	LCD_WR_DATA8(0x28);   
//	LCD_WR_DATA8(0x2E);   
//	LCD_WR_DATA8(0x39);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x03);   
//	LCD_WR_DATA8(0x02);   
//	LCD_WR_DATA8(0x10);   

//	LCD_WR_REG(0xE1);
//	LCD_WR_DATA8(0x06);   
//	LCD_WR_DATA8(0x21);   
//	LCD_WR_DATA8(0x0D);   
//	LCD_WR_DATA8(0x17);   
//	LCD_WR_DATA8(0x35);   
//	LCD_WR_DATA8(0x30);   
//	LCD_WR_DATA8(0x2A);   
//	LCD_WR_DATA8(0x2D);   
//	LCD_WR_DATA8(0x2C);   
//	LCD_WR_DATA8(0x29);   
//	LCD_WR_DATA8(0x31);   
//	LCD_WR_DATA8(0x3B);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x02);   
//	LCD_WR_DATA8(0x03);   
//	LCD_WR_DATA8(0x12); 
//	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
//	LCD_WR_REG(0x3A); //65k mode
//	LCD_WR_DATA8(0x05);
//	
//	LCD_WR_REG(0x2A);     
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x02);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x81); 

//	LCD_WR_REG(0x2b);     
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0x01);   
//	LCD_WR_DATA8(0x00);   
//	LCD_WR_DATA8(0xa0); 
//	
//	LCD_WR_REG(0x29); //Display on
//	delay_ms(120);
//	//LCD_WR_REG(0x28);  //Display off
//	delay_ms(120);
//}


//清屏函数
//Color:要清屏的填充色
u16 FILL_DATA;
void LCD_Clear(u16 Color)
{
	Address_set(0,0,LCD_W-1,LCD_H-1);
	FILL_DATA = Color;
	LCD_AO = 1;//数据传输
	SPI3_send_same_half_word((uint32_t *)(&FILL_DATA),LCD_W*LCD_H);
	delay_ms(20);
}


//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	Address_set(x,y,x,y);
	LCD_WR_DATA(color);
	/*
	LCD_AO = 1;//数据传输
	FILL_DATA = color;
	SPI3_write_half_words((uint32_t *)(&FILL_DATA),1);
	*/
}	 
void LCD_Set_Display_Mode(u8 mode)
{
	if(mode)
	{
		LCD_WR_REG(0x36); //MX, MY, RGB mode
		LCD_WR_DATA8(0xA0);
	}
	else
	{
		LCD_WR_REG(0x36); //MX, MY, RGB mode
		LCD_WR_DATA8(0x60);
	}
}

void LCD_Clear_Test(u16 Color)
{
	int i;
	Address_set(0,0,LCD_W-1,LCD_H-1);
	LCD_AO = 1;//数据传输
	for(i=0;i<LCD_W*LCD_H;i++)
	{
		LCD_WR_DATA(Color);
		delay_ms(20);
	}
}

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
//u16 LCD_ReadPoint(u16 x,u16 y)
//{

//}
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  
	Address_set(sx,sy,ex,ey);
	FILL_DATA = color;
	LCD_AO = 1;//数据传输
	SPI3_send_same_half_word((uint32_t *)(&FILL_DATA),(ex-sx+1)*(ey-sy+1));
} 

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_H)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_W)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 

//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
	{       
		if(x>=width){x=x0;y+=size;}
		if(y>=height)break;//退出
		LCD_ShowChar(x,y,*p,size,0);
		x+=size/2;
		p++;
	}  
	while((x<width)||(y<height))
	{
		if(x>=width){x=x0;y+=size;}
		LCD_ShowChar(x,y,' ',size,0);
		x+=size/2;
	}
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_Fast_DrawPoint(x0+a,y0-b,color);             //5
 		LCD_Fast_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_Fast_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_Fast_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_Fast_DrawPoint(x0-a,y0+b,color);             //1       
 		LCD_Fast_DrawPoint(x0-b,y0+a,color);             
		LCD_Fast_DrawPoint(x0-a,y0-b,color);             //2             
  	LCD_Fast_DrawPoint(x0-b,y0-a,color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	

void LCD_DrawBMP(u16 x,u16 y,BMP *bmp,u16 point_color,u16 back_color)
{
	unsigned int data_len,i,j;
	unsigned int x0=x;
	unsigned char bit;
	data_len = bmp->h*bmp->w/8;
	if(bmp->h*bmp->w%8)
	{
		data_len++;
	}
	if(bmp->data == 0)
		return;
	for(i=0;i<data_len;i++)
	{
		bit = 0x01;
		for(j=0;j<8;j++)
		{
			if(bmp->data[i]&bit)
			{
				LCD_Fast_DrawPoint(x,y,point_color);
			}
			else
			{
				LCD_Fast_DrawPoint(x,y,back_color);
			}
			bit <<= 1;
			x++;
			if(x-x0>=bmp->w)
			{
				x = x0;
				y++;
			}
		}
	}
}

