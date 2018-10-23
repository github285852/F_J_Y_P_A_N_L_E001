#include "graphical.h"
#include "st7735s.h"
#include "picture_data.h"
#include "spi.h"

#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "stdlib.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//通过将数据先放入缓存，然后再将缓存显示
void LCD_Fill_Picture(u16 x,u16 y,Picture pic)
{  
	u16 ex,ey;
	ex = x + pic.w - 1;
	if(ex>LCD_W-1)
	{
		ex = LCD_W-1;
	}
	ey = y + pic.h - 1;
	if(ey>LCD_H-1)
	{
		ey = LCD_H-1;
	}	
	Address_set(x,y,ex,ey);
	LCD_AO = 1;//数据传输
	SPI3_write_half_words((uint32_t *)pic.data,pic.w*pic.h);
}

void Picture_Fill(Picture pic,u16 color)
{  
	u16 i,temp;
	temp = pic.w*pic.h;
	for(i=0;i<temp;i++)
	{
		pic.data[i] = color;
	}
}

void Picture_DrawPoint(Picture pic,u16 x,u16 y,u16 color)
{	   
	u16 pos;
	if(x>pic.w-1)
		return;
	if(y>pic.h-1)
		return;
	pos = y*pic.w + x;
	pic.data[pos] = color;
}	 

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void Picture_ShowChar(Picture pic,u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	  //调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)Picture_DrawPoint(pic,x,y,POINT_COLOR);
			else if(mode==0)Picture_DrawPoint(pic,x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>pic.h)
				return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>pic.w)
					return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 


void Picture_ShowChinese(Picture pic,u16 x,u16 y,u8 *code,u8 size,u8 mode)
{
	int i,j;
	unsigned int y0 = y;
	unsigned char *pfont,temp;
	unsigned int gbk_code = code[1] | (code[0]<<8);
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	//根据编码获得字符数据
	for(i=0;;i++)
	{
		if(GBK_TAB[i] == gbk_code)
	   break;
		else if(GBK_TAB[i] == 0x0000)//找不到字库
			return;
	}
	switch(size)
	{
		case 12:break;
		case 16:pfont = (u8 *)Chinese16X16_Tab[i];break;
		case 24:break;
		default:break;
	}
	for(i=0;i<csize;i++)
	{
		temp = pfont[i];
		for(j=0;j<8;j++)
		{
			if(temp&0x80)Picture_DrawPoint(pic,x,y,POINT_COLOR);
			else if(mode==0)Picture_DrawPoint(pic,x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>pic.h)
				return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>pic.w)
					return;	//超区域了
				break;
			}
		} 
	}
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  

void Picture_ShowString(Picture pic,u16 x,u16 y,u16 width,u16 height,u8 size,char *p,u8 mode)
{         
	u8 x0=x;
	width+=x;
	height+=y;
	static char last_char=0;
  while(*p != 0)//判断数据结束
  {       
		if(*p=='\n')
		{
			if(x+size/2>=width){x=x0;y+=size;continue;}
			if(y>=height)break;//退出
			x=x0;y+=size;
			p++;
		}
		else if(*p<0x80)
		{
			if(x+size/2>=width){if((last_char<0x80)&&(last_char!=' '))Picture_ShowChar(pic,x,y,'-',size,mode); x=x0;y+=size;continue;}
			if(y>=height)break;//退出
			Picture_ShowChar(pic,x,y,*p,size,mode);
			x+=size/2;
			last_char = *p;
			p++;
		}
		else //中文
		{
			if(x+size>=width){x=x0;y+=size;continue;}
			if(y>=height)break;//退出
			Picture_ShowChinese(pic,x,y,(u8 *)p,size,mode);
			x += size;
			p += 2;
		}
	}  
}



//在一个矩形中流动显示，当显示的长度大于矩形，则用这种方法
// pic 菜单指针
// rect 显示的区域指针
// pos 轮询坐标指针
// speed 轮询速度
// size 字体大小
// p 字符串指针
void Picture_TranDispalyOnline(Picture *pic,RECT *rect,u8 *pos,u8 size,u8 mode,char *p)
{
	int xlen;
	u16 y = ((rect->y1 - rect->y0)-size)/2;
	char *buf=NULL;
	int  temp;
	xlen = rect->x1-rect->x0;
	xlen  = xlen/size*2;
	temp = strlen(p) - xlen;
	if(temp<=0)
	{
		Picture_ShowStringInRectCenter(*pic,*rect,mode,size,p);
		return;
	}
	buf = malloc(xlen+1);
	if(buf != NULL)
	{
		memset(buf,0,xlen+1);
		memcpy(buf,p + *pos,xlen);
		if(*pos>temp)
			*pos = 0;
		//
		Picture_ShowStringInRectCenter(*pic,*rect,mode,size,buf);
		//Picture_ShowString(*pic,rect->x0,rect->y0,rect->x1-rect->x0,rect->y1 -rect->y0,size,buf,0);
		free(buf);
	}
}

//在一个矩形内显示字符串
void Picture_ShowStringInRectCenter(Picture pic,RECT rect,u8 mode,u8 size,char *p)
{         
	int x0,str_w,rect_w,rect_h;
	int x,y;
	str_w = strlen(p)*size/2;
	rect_w = rect.x1-rect.x0;
	rect_h = rect.y1-rect.y0;
	x = (rect_w - str_w)/2;
	y = (rect_h - size)/2;
	if(x<0)
	{
		x = 0;
	}
	if(y<0)
	{
		y = 0;
	}
	x += rect.x0;
	x0 = x;
	y += rect.y0;
	
	Picture_ShowString(pic,x,y,rect.x1-x,rect.y1-y,size,p,mode);
//  while(*p != 0)//判断是不是非法字符!
//  {       
//		if(x>=rect.x1){x=x0;y+=size;}
//		if(y>=rect.y1)break;//退出
//		
//		if(*p=='\n')
//		{
//			x=x0;y+=size;
//			p++;
//		}
//		else if(*p<0x80)
//		{
//			Picture_ShowChar(pic,x,y,*p,size,mode);
//			x+=size/2;
//			p++;
//		}
//		else //中文
//		{
//			Picture_ShowChinese(pic,x,y,(u8 *)p,size,mode);
//			x += size;
//			p += 2;
//		}
//	} 

//  while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//  {       
//		if(x>=rect.x1){x=x0;y+=size;}
//		if(y>=rect.y1)break;//退出
//		Picture_ShowChar(pic,x,y,*p,size,mode);
//		x+=size/2;
//		p++;
//	}  
}

//u16 abs(u16 a,u16 b)
//{
//	if(a>b)
//	{
//		return a-b;
//	}
//	else
//	{
//		return b-a;
//	}
//}

u16 pow2(u16 a)
{
	return a*a;
}
//r必须小于等于y和x
void Picture_FillCircle(Picture pic,u8 x, u8 y,u8 r,u16 color)
{
	int x0=x-r,y0=y-r;
	u16 l,R=pow2(r);
	
	for(;y0<pic.h;y0++)
	{
		for(;x0<pic.w;x0++)
		{
			l =  pow2(abs(y0-y)) + pow2(abs(x0-x)) ;
			if(l<=R)
			{
				Picture_DrawPoint(pic,x0,y0,color);
			}
		}
		x0 = x-r;
	}
}

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void Picture_ShowNum(Picture pic,u16 x,u16 y,long num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;		
	if(num<0)
	{
		num *= -1;
		t = 1;
		Picture_ShowChar(pic,x,y,'-',size,0);
	}
	else
	{
		t = 0;
	}
	for(;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				Picture_ShowChar(pic,x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	Picture_ShowChar(pic,x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void Picture_DrawLine(Picture pic,u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		Picture_DrawPoint(pic,uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

void Picture_FillRect(Picture pic,RECT rect,u16 color)
{
	int i,j;
	for(i=rect.x0;i<rect.x1;i++)
		for(j=rect.y0;j<rect.y1;j++)
			if((i<pic.w)&&(j<pic.h))
				Picture_DrawPoint(pic,i,j,color);
}
unsigned int myabs(int num)
{
	if(num>0)
	{
		return num;
	}
	else
	{
		return -num;
	}
}
	
void Picture_DrawTrigon(Picture pic,u16 x,u16 y,u8 h,u8 d,u16 color)
{
	Picture_DrawLine(pic,x+d,y,x+2*d,y+h,color);
	Picture_DrawLine(pic,x,y+h,x+2*d,y+h,color);
	Picture_DrawLine(pic,x+d,y,x,y+h,color);
}
void Picture_DrawInvertedTrigon(Picture pic,u16 x,u16 y,u8 h,u8 d,u16 color)
{
	Picture_DrawLine(pic,x,y,x+2*d,y,color);
	Picture_DrawLine(pic,x,y,x+d,y+h,color);
	Picture_DrawLine(pic,x+2*d,y,x+d,y+h,color);
}
	
void Picture_FillTrigon(Picture pic,u16 x,u16 y,u8 size,u16 color)
{
	int i,j,k,n=size;
	for(i=1;i<=2*n+1;i++)
  {
    j=myabs(n+1-i);
    for(k=1;k<=j;k++)
			x++;
    for(k=1;k<=2*n+1-2*j;k++)
			Picture_DrawPoint(pic,x,y,color);
    y++;
  } 
}

//在图片中画bmp
void Picture_DrawBMP(Picture pic,u16 x,u16 y,BMP *bmp,u16 point_color,u16 back_color)
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
				Picture_DrawPoint(pic,x,y,point_color);
			}
			else
			{
				Picture_DrawPoint(pic,x,y,back_color);
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

void Pictrue_printf(Picture *pic,u16 x,u16 y,u8 size,char* fmt,...)  
{  
	u8 *pbuf;
//	u8 *p;
	u16 width=pic->w,height=pic->h;
	
	va_list ap;
	pbuf = malloc(100);
	if(!pbuf)									
	{
		return ;
	}
	va_start(ap,fmt);
	vsprintf((char*)pbuf,fmt,ap);
	va_end(ap);			
// p = pbuf;
	Picture_ShowString(*pic,x,y,pic->w-x,pic->h-y,size,pbuf,0);	
//  while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
//  {       
//		if(x>=width){x=0;y+=size;continue;}
//		if(y>=height)break;//退出
//		if(*p=='\n')
//		{
//			x=0;y+=size;
//		}
//		else
//		{
//			Picture_ShowChar(*pic,x,y,*p,size,0);
//			x+=size/2;
//		}
//		p++;
//	}  
	//
	free(pbuf);
}
