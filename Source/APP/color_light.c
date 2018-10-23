#include "color_light.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "includes.h"
/*
unsigned char gama_tab[256]=
{
	0,1,4,9,16,25,36,49,65,82,101,122,145,170,198,227,258,291,327,364,403,444,488,533,581,630,681,735,790,848,907,969,1032,1098,1165,1235,1306,1380,1455,1533,1613,1694,1778,1864,1951,2041,2133,2226,2322,2420,2520,2621,2725,2831,2939,3049,3161,3274,3390,3508,3628,3750,3874,4000,4128,4258,4390,4524,4660,4798,4938,5081,5225,5371,5519,5669,5821,5976,6132,6290,6450,6612,6777,6943,7111,7282,7454,7628,7805,7983,8164,8346,8530,8717,8905,9096,9288,9483,9679,9878,10078,10281,10486,10692,10901,11111,11324,11539,11755,11974,12195,12418,12642,12869,13098,13329,13562,13796,14033,14272,14513,14756,15001,15248,15497,15748,16001,16256,16513,16772,17033,17296,17561,17828,18097,18368,18641,18916,19193,19473,19754,20037,20322,20609,20899,21190,21483,21778,22076,22375,22676,22980,23285,23593,23902,24213,24527,24842,25160,25479,25801,26124,26450,26777,27107,27439,27772,28108,28445,28785,29127,29470,29816,30164,30513,30865,31219,31575,31933,32292,32654,33018,33384,33752,34122,34493,34867,35243,35621,36001,36383,36767,37153,37541,37931,38323,38717,39113,39511,39912,40314,40718,41124,41532,41942,42355,42769,43185,43603,44024,44446,44870,45297,45725,46155,46588,47022,47458,47897,48337,48780,49224,49671,50119,50570,51022,51477,51933,52392,52852,53315,53780,54246,54715,55185,55658,56133,56610,57088,57569,58052,58537,59023,59512,60003,60496,60991,61488,61986,62487,62990,63495,64002,64511,65022,65535
};
*/
                     //  R   G   B   Y  W
unsigned int RGBPwm[6]={100,200,300,400,0,0};

float LEDWWX = LEDWWX_DEFAULT,
LEDWWY = LEDWWY_DEFAULT,
LEDCWX = LEDCWX_DEFAULT,
LEDCWY = LEDCWY_DEFAULT,
LEDWX = LEDWX_DEFAULT,
LEDWY = LEDWY_DEFAULT,
LEDRX = LEDRX_DEFAULT,
LEDRY = LEDRY_DEFAULT,
LEDGX = LEDGX_DEFAULT,
LEDGY = LEDGY_DEFAULT,
LEDBX = LEDBX_DEFAULT,
LEDBY = LEDBY_DEFAULT,
LEDYX = LEDYX_DEFAULT,
LEDYY = LEDYY_DEFAULT;

//COORD LED[LED_NUMS]=
//{
//	{LEDRX_DEFAULT,LEDRY_DEFAULT},//R
//	{LEDGX_DEFAULT,LEDGY_DEFAULT},//G
//	{LEDBX_DEFAULT,LEDBY_DEFAULT},//B
//	{LEDWWX_DEFAULT,LEDWWY_DEFAULT},//WW
//	{LEDCWX_DEFAULT,LEDCWY_DEFAULT},//CW
//};
	
void HSI_to_RGB(HSI HSI,RGB *rgbk)
{
	float e,f,g,h;
	double temp;
	float cose;
//	int mode = HSI.h*1000;
//	mode %= 120;
//	temp = mode/1000.0;
//	e = temp*PI/180.0;	
	e = (HSI.h%120)*PI/180.0;
	f = HSI.i*(1.0-HSI.s);
	
	cose = cos(e);
	temp = 60*PI/180.0 - e;
	temp = cos(temp);
	
	g = HSI.i*(1+ HSI.s*cose/temp );
	h = 3*HSI.i - (f+g);
	if(HSI.h>=120)
	{
		if(HSI.h<240)
		{
			rgbk->r = f;
			rgbk->g = g;
			rgbk->b = h;
		}
		else
		{
			rgbk->r = h;
			rgbk->g = f;
			rgbk->b = g;
		}
	}
	else
	{
			rgbk->r = g;
			rgbk->g = h;
			rgbk->b = f;
	}
}

void RGB_to_coordinate(RGB rgb,COORD *coord)
{
	float RBx,RBy,temp;
	if(rgb.r != 0)
	{
		temp = (1+rgb.b/rgb.r);
		RBx = (RX+rgb.b*BX/rgb.r)/temp;
		RBy = (RY+rgb.b*BY/rgb.r)/temp;
		temp = 1+rgb.g/(rgb.r+rgb.b);
		coord->x = (RBx+rgb.g*GX/(rgb.b+rgb.r))/temp;
		coord->y = (RBy+rgb.g*GY/(rgb.b+rgb.r))/temp;
	}
	else //��GB����
	{
		coord->x = (GX*rgb.g+BX*rgb.b)/(rgb.g+rgb.b);
		coord->y = (GY*rgb.g+BY*rgb.b)/(rgb.g+rgb.b);
//		rs485_send_str("rgb.r=0\r\n");
//		sprintf(buf,"HSI(%d,%.2f,%.2f)\r\n",Hsi.h,Hsi.s,Hsi.i);
//		rs485_send_str(buf);
//		rs485_send_str("\r\n");
	}
}
void RGBWWCW_to_coordinate(RGB rgb,COORD *coord)
{
	COORD w_coord;
	float RBx,RBy,temp;
	coord->x = coord->y = 0;
	if((rgb.r+rgb.g+rgb.b)>0.0)
	{
		if(rgb.r != 0)
		{
			temp = (1+rgb.b/rgb.r);
			RBx = (LEDRX+rgb.b*LEDBX/rgb.r)/temp;
			RBy = (LEDRY+rgb.b*LEDBY/rgb.r)/temp;
			temp = 1+rgb.g/(rgb.r+rgb.b);
			coord->x = (RBx+rgb.g*LEDGX/(rgb.b+rgb.r))/temp;
			coord->y = (RBy+rgb.g*LEDGY/(rgb.b+rgb.r))/temp;
		}
		else //��GB����
		{
			coord->x = (LEDGX*rgb.g+LEDBX*rgb.b)/(rgb.g+rgb.b);
			coord->y = (LEDGY*rgb.g+LEDBY*rgb.b)/(rgb.g+rgb.b);
		}
	}

	if(rgb.ww>0)
	{
		if(rgb.cw>0)
		{
			w_coord.x = (LEDWWX*rgb.ww+LEDCWX*rgb.cw)/(rgb.ww+rgb.cw);
			w_coord.y = (LEDWWY*rgb.ww+LEDCWY*rgb.cw)/(rgb.ww+rgb.cw);
		}
		else
		{
			w_coord.x = LEDWWX;
			w_coord.y = LEDWWY;
		}
	}
	else
	{
		if(rgb.cw>0)
		{
			w_coord.x = LEDCWX;
			w_coord.y = LEDCWY;
		}
		else
		{
			return ;
		}
	}
	if((rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw)>0)
	{
		coord->x = (w_coord.x*(rgb.ww+rgb.cw) + coord->x*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw);
		coord->y = (w_coord.y*(rgb.ww+rgb.cw) + coord->y*(rgb.r+rgb.g+rgb.b))/(rgb.r+rgb.g+rgb.b+rgb.ww+rgb.cw);
	}
}


unsigned char coordinate_to_RGBYW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,RYx,RYy,GBx,GBy,GYx,GYy,BYx,BYy;
	float RGk,RBk,RYk,GBk,GYk,BYk,Vrage_x=0,Vrage_y,NWk;
	float temp1,temp2,temp3;
	unsigned char flag =0,valid_count=0;

	temp1 = (LEDWY-coord.y)/(LEDWX-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDRY)/(LEDYX-LEDRX);
	RYx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RYy = (RYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDGY)/(LEDYX-LEDGX);
	GYx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GYy = (GYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDBY)/(LEDYX-LEDBX);
	BYx = (coord.x*temp1-LEDBX*temp2+LEDBY-coord.y)/(temp1-temp2);
	BYy = (BYx-coord.x)*temp1 +coord.y;
	
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	RYk = (LEDRX-RYx)/(RYx-LEDYX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	GYk = (LEDGX-GYx)/(GYx-LEDYX);
	BYk = (LEDBX-BYx)/(BYx-LEDYX);
	
	if(RGk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RGx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x01;
			valid_count++;
			Vrage_x += RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x02;
			valid_count++;
			Vrage_x += RBx;
		}
	}
	if(RYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-RYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x04;
			valid_count++;
			Vrage_x += RYx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-GBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x08;
			valid_count++;
			Vrage_x += GBx;
		}
	}
	if(GYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-GYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x10;
			valid_count++;
			Vrage_x += GYx;
		}
	}
	if(BYk>0)
	{
		temp2 = (coord.x - LEDWX)/(coord.x-BYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x20;
			valid_count++;
			Vrage_x += BYx;
		}
	}
	if(valid_count<=0)
	{
		Debug_printf("Coor_To_RGBYW No Color\r\n");
		return 1;
	}
	Vrage_x = (float)(Vrage_x/valid_count);
	Vrage_y = (Vrage_x-coord.x)*temp1+coord.y;//����ƽ��ֵy
	NWk = (Vrage_x-coord.x)/(coord.x-LEDWX);//����Ͱ׹���߶α�
	memset(rgb,0,sizeof(RGB)); 
	rgb->w = 1;
	temp2 = rgb->w/NWk;//����׹�����Ϊ1,�������������
	if(flag&0x01)
	{
		temp3 = temp2/valid_count/(1+RGk);
		rgb->r += temp3;
		rgb->g += temp3*RGk;
	}
	if(flag&0x02)
	{
		temp3 = temp2/valid_count/(1+RBk);
		rgb->r += temp3;
		rgb->b += temp3*RBk;
	}
	if(flag&0x04)
	{
		temp3 = temp2/valid_count/(1+RYk);
		rgb->r += temp3;
		rgb->y += temp3*RYk;
	}
	if(flag&0x08)
	{
		temp3 = temp2/valid_count/(1+GBk);
		rgb->g += temp3;
		rgb->b += temp3*GBk;
	}
	if(flag&0x10)
	{
		temp3 = temp2/valid_count/(1+GYk);
		rgb->g += temp3;
		rgb->y += temp3*GYk;
	}
	if(flag&0x20)
	{
		temp3 = temp2/valid_count/(1+BYk);
		rgb->b += temp3;
		rgb->y += temp3*BYk;
	}
	return 0;
}

unsigned char coordinate_to_RGBYWWCW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,RYx,RYy,GBx,GBy,GYx,GYy,BYx,BYy;
	float RGk,RBk,RYk,GBk,GYk,BYk,Vrage_x=0,Vrage_y,NWk;
	float temp1,temp2,temp3;
	unsigned char flag =0,valid_count=0;

	float ledw_x,ledw_y ,point_x,point_y,ww_cw_k;//Ŀ�������׺�ů��������������
//��׺�ů�׺ϳɺϳɵİ�
	temp1 = (LEDCWY-LEDWWY)/(LEDCWX-LEDWWX);
	temp2 = (LEDCWX - LEDWWX)/(LEDCWY-LEDWWY);
	point_x = (LEDWWX*temp1+coord.x*temp2+coord.y-LEDWWY)/(temp1+temp2);
	point_y = (point_x-LEDWWX)*temp1+LEDWWY;
	if(point_x>=LEDWWX)
	{
		ledw_x = LEDWWX;
	}
	else
	{
		if(point_x<=LEDCWX)
		{
			ledw_x = LEDCWX;
		}
		else
		{
			ledw_x = point_x;
		}
	}
	ledw_y = (ledw_x - LEDCWX)*temp1+LEDCWY;
	
	temp1 = (ledw_y-coord.y)/(ledw_x-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDRY)/(LEDYX-LEDRX);
	RYx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RYy = (RYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDGY)/(LEDYX-LEDGX);
	GYx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GYy = (GYx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDYY-LEDBY)/(LEDYX-LEDBX);
	BYx = (coord.x*temp1-LEDBX*temp2+LEDBY-coord.y)/(temp1-temp2);
	BYy = (BYx-coord.x)*temp1 +coord.y;
	
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	RYk = (LEDRX-RYx)/(RYx-LEDYX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	GYk = (LEDGX-GYx)/(GYx-LEDYX);
	BYk = (LEDBX-BYx)/(BYx-LEDYX);
	
	if(RGk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RGx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x01;
			valid_count++;
			Vrage_x += RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x02;
			valid_count++;
			Vrage_x += RBx;
		}
	}
	if(RYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x04;
			valid_count++;
			Vrage_x += RYx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x08;
			valid_count++;
			Vrage_x += GBx;
		}
	}
	if(GYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x10;
			valid_count++;
			Vrage_x += GYx;
		}
	}
	if(BYk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-BYx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x20;
			valid_count++;
			Vrage_x += BYx;
		}
	}
	if(valid_count==0)
	{
		Debug_printf("NO COLOR\r\n");
		return 1;
	}
	Vrage_x = (float)(Vrage_x/valid_count);
	Vrage_y = (Vrage_x-coord.x)*temp1+coord.y;//����ƽ��ֵy
	NWk = (Vrage_x-coord.x)/(coord.x-ledw_x);//����Ͱ׹���߶α�
	memset(rgb,0,sizeof(RGB)); 
	temp2 = 1/NWk;//����׹�����Ϊ1,�������������
	ww_cw_k = (LEDCWX - ledw_x)/(LEDCWX-LEDWWX);
	rgb->ww = ww_cw_k;
	rgb->cw = (1-ww_cw_k);
	if(flag&0x01)
	{
		temp3 = temp2/valid_count/(1+RGk);
		rgb->r += temp3;
		rgb->g += temp3*RGk;
	}
	if(flag&0x02)
	{
		temp3 = temp2/valid_count/(1+RBk);
		rgb->r += temp3;
		rgb->b += temp3*RBk;
	}
	if(flag&0x04)
	{
		temp3 = temp2/valid_count/(1+RYk);
		rgb->r += temp3;
		rgb->y += temp3*RYk;
	}
	if(flag&0x08)
	{
		temp3 = temp2/valid_count/(1+GBk);
		rgb->g += temp3;
		rgb->b += temp3*GBk;
	}
	if(flag&0x10)
	{
		temp3 = temp2/valid_count/(1+GYk);
		rgb->g += temp3;
		rgb->y += temp3*GYk;
	}
	if(flag&0x20)
	{
		temp3 = temp2/valid_count/(1+BYk);
		rgb->b += temp3;
		rgb->y += temp3*BYk;
	}
	return 0;
}

unsigned char coordinate_to_RGBWWCW(COORD coord,RGB *rgb)
{
	float RGx,RGy,RBx,RBy,GBx,GBy;
	float RGk,RBk,GBk,valid_x=0,valid_y,NWk;
	float temp1,temp2,temp3,max_k;
	unsigned char flag =0;

	float ledw_x,ledw_y ,point_x,point_y,ww_cw_k;//Ŀ�������׺�ů��������������
//��׺�ů�׺ϳɺϳɵİ�
	temp1 = (LEDCWY-LEDWWY)/(LEDCWX-LEDWWX);
	temp2 = (LEDCWX - LEDWWX)/(LEDCWY-LEDWWY);
	point_x = (LEDWWX*temp1+coord.x*temp2+coord.y-LEDWWY)/(temp1+temp2);
	point_y = (point_x-LEDWWX)*temp1+LEDWWY;
	if(point_x>=LEDWWX)
	{
		ledw_x = LEDWWX;
	}
	else
	{
		if(point_x<=LEDCWX)
		{
			ledw_x = LEDCWX;
		}
		else
		{
			ledw_x = point_x;
		}
	}
	ledw_y = (ledw_x - LEDCWX)*temp1+LEDCWY;
	//Ŀ������ů������̵���ֱ��L1
	//Ŀ������̵��б��
	temp1 = (ledw_y-coord.y)/(ledw_x-coord.x);
	temp2 = (LEDGY-LEDRY)/(LEDGX-LEDRX);
	RGx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RGy = (RGx-coord.x)*temp1 +coord.y;

	temp2 = (LEDBY-LEDRY)/(LEDBX-LEDRX);
	RBx = (coord.x*temp1-LEDRX*temp2+LEDRY-coord.y)/(temp1-temp2);
	RBy = (RBx-coord.x)*temp1 +coord.y;
	
	temp2 = (LEDBY-LEDGY)/(LEDBX-LEDGX);
	GBx = (coord.x*temp1-LEDGX*temp2+LEDGY-coord.y)/(temp1-temp2);
	GBy = (GBx-coord.x)*temp1 +coord.y;
	
	//�߶α���
	RGk = (LEDRX-RGx)/(RGx-LEDGX);
	RBk = (LEDRX-RBx)/(RBx-LEDBX);
	GBk = (LEDGX-GBx)/(GBx-LEDBX);
	
	if(RGk>0) //������RG�м�
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RGx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x01;
			valid_x = RGx;
		}
	}
	if(RBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-RBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x02;
			valid_x = RBx;
		}
	}
	if(GBk>0)
	{
		temp2 = (coord.x - ledw_x)/(coord.x-GBx);
		if(temp2<0)//Ŀ��ɫ���м�
		{
			flag |= 0x04;
			valid_x = GBx;
		}
	}
	if(valid_x==0)
	{
		Debug_printf("NO COLOR\r\n");
		return 1;
	}
	valid_y = (valid_x-coord.x)*temp1+coord.y;//����y
	NWk = (valid_x-coord.x)/(coord.x-ledw_x);//����Ͱ׹���߶α�
	memset(rgb,0,sizeof(RGB)); 
	temp2 = 1/NWk;//����׹�����Ϊ1,�������������
	ww_cw_k = (LEDCWX - ledw_x)/(LEDCWX-LEDWWX);//
	rgb->ww = ww_cw_k;
	rgb->cw = (1-ww_cw_k);
	
	switch(flag)
	{
		case 0x01://���r������
		{
			temp3 = temp2/(1+RGk);
			rgb->r += temp3;
			rgb->g += temp3*RGk; //���g������
			break;
		}
		case 0x02:
		{
			temp3 = temp2/(1+RBk);
			rgb->r += temp3;
			rgb->b += temp3*RBk;
			break;
		}
		case 0x04:
		{
			temp3 = temp2/(1+GBk);
			rgb->g += temp3;
			rgb->b += temp3*GBk;
			break;
		}
		default :break;
	}
	//��һ������
//	max_k = MAX(rgb->r,rgb->g);
//	max_k = MAX(max_k,rgb->b);
//	max_k = MAX(max_k,rgb->cw);
//	max_k = MAX(max_k,rgb->ww);

//	rgb->r = rgb->r/max_k;
//	rgb->g = rgb->g/max_k;
//	rgb->b = rgb->b/max_k;
//	rgb->ww = rgb->ww/max_k;
//	rgb->cw = rgb->cw/max_k; ��ledpower.c��ChanleDataChange()������һ������
	return 0;
}


COORD LED_CHECK[5][10] ;

	
//���е���У׼��ɫ����תLED����
int check_coordinate_to_RGBWWCW(COORD coord,char dim,RGB *rgb)
{
	float temp;
	unsigned int k;
	int i;
	LEDWWX = LEDWWX_DEFAULT,
	LEDWWY = LEDWWY_DEFAULT,
	LEDCWX = LEDCWX_DEFAULT,
	LEDCWY = LEDCWY_DEFAULT,
	LEDRX = LEDRX_DEFAULT,
	LEDRY = LEDRY_DEFAULT,
	LEDGX = LEDGX_DEFAULT,
	LEDGY = LEDGY_DEFAULT,
	LEDBX = LEDBX_DEFAULT,
	LEDBY = LEDBY_DEFAULT;
	coordinate_to_RGBWWCW(coord,rgb);
	temp = rgb->r + rgb->g + rgb->b + rgb->cw + rgb->ww;//������ϵ��λtemp
	temp = (float)(dim*10/temp);

	
	k = (int)(rgb->r*temp+0.5);//��������
	if(k>10)
	{
		k = 10;
	}
	LEDRX = LED_CHECK[0][k-1].x;
	LEDRY = LED_CHECK[0][k-1].x;
		
	k = (int)(rgb->g*temp+0.5);//��������
	if(k>10)
	{
		k = 10;
	}
	LEDGX = LED_CHECK[1][k-1].x;
	LEDGY = LED_CHECK[1][k-1].x;
	
	k = (int)(rgb->b*temp+0.5);//��������
	if(k>10)
	{
		k = 10;
	}
	LEDBX = LED_CHECK[2][k-1].x;
	LEDBY = LED_CHECK[2][k-1].x;
	
	k = (int)(rgb->ww*temp+0.5);//��������
	if(k>10)
	{
		k = 10;
	}
	LEDWWX = LED_CHECK[3][k-1].x;
	LEDWWY = LED_CHECK[3][k-1].x;
	
	k = (int)(rgb->cw*temp+0.5);//��������
	if(k>10)
	{
		k = 10;
	}
	LEDCWX = LED_CHECK[4][k-1].x;
	LEDCWY = LED_CHECK[4][k-1].x;
	return coordinate_to_RGBWWCW(coord,rgb);
}

RGB rgb={0,0,0,0};
RGB rgbk={0,0,0,0};
COORD coord={0.3,0.5};

/*


*/
int HSIToCoordinate(HSI *Hsi,COORD *coord)
{
	HSI hsi;
	float temp;
	int i;
	hsi = *Hsi;
	hsi.s = 1; //��Щ���Ͷ�̫�ߵ���ɫ�ϳɲ���
	if(hsi.i<=0.0009)
	{
		AllLedPowerOff();
		return 1;
	}
	do
	{
		HSI_to_RGB(hsi,&rgb);
		RGB_to_coordinate(rgb,coord);
		hsi.s -= 0.01;//�ҵ����Ͷ������Ǹ���
	}
	while(coordinate_to_RGBWWCW(*coord,&rgbk)&&(hsi.s>0));
	Debug_printf("�ܴﵽ����󱥺Ͷ�=%d\r\n",(u8)(hsi.s*100));
	hsi.s = hsi.s*Hsi->s; // ���Ͷȱ��ͶȻ�
	
	HSI_to_RGB(hsi,&rgb);
	RGB_to_coordinate(rgb,coord);
	return 0;
	
}
/*
INPUT HSI:H=0-259;S=0.0-1.0;
pixel:0,ALL,
*/
void ColorLightHSIOut(HSI Hsi,unsigned char pixel)
{
	if(HSIToCoordinate(&Hsi,&coord))
	{
		Debug_printf("���Ͷ�ת������Ȼû��\r\n");
		return;
	}
	if(CoordinateOut(&coord,Hsi.i,pixel))
	{
		return ;
	}
	Debug_printf("\r\n");
	Debug_printf("HSI(%d,%.2f,%.2f)\r\n",Hsi.h,Hsi.s,Hsi.i);
	Debug_printf("ɫ����(%.4f,%.4f)\r\n",coord.x,coord.y);
	Debug_printf("RGB(%.4f,%.4f,%.4f)\r\n",rgb.r,rgb.g,rgb.b);
}

const CCT_TAB cct_tab[] = {
{25,0.4769,0.4137,0.5200,0.4820},
{26,0.4681,0.4120,0.5100,0.4810},
{27,0.4597,0.4105,0.4990,0.4790}, 
{28,0.4518,0.4086,0.4890,0.4770}, 
{29,0.4441,0.4064,0.4780,0.4730}, 
{30,0.4369,0.4040,0.4690,0.4700},
{31,0.4299,0.4015,0.4600,0.4660}, 
{32,0.4233,0.3989,0.4510,0.4625}, 
{33,0.4170,0.3962,0.4430,0.4590}, 
{34,0.4109,0.3934,0.4350,0.4550}, 
{35,0.4052,0.3907,0.4280,0.4510},
{36,0.3998,0.3879,0.4210,0.4470}, 
{37,0.3948,0.3851,0.4145,0.4430}, 
{38,0.3895,0.3823,0.4080,0.4390},
{39,0.3849,0.3795,0.4025,0.4360},
{40,0.3804,0.3767,0.3970,0.4320},
{41,0.3761,0.3740,0.3910,0.4280},
{42,0.3720,0.3713,0.3850,0.4240}, 
{43,0.3681,0.3687,0.3800,0.4210}, 
{44,0.3643,0.3661,0.3750,0.4180}, 
{45,0.3607,0.3635,0.3705,0.4140}, 
{46,0.3573,0.3611,0.3660,0.4100}, 
{47,0.3541,0.3586,0.3620,0.4070}, 
{48,0.3510,0.3562,0.3580,0.4040}, 
{49,0.3480,0.3539,0.3540,0.4005}, 
{50,0.3450,0.3516,0.3500,0.3970}, 
{51,0.3423,0.3494,0.3460,0.3940}, 
{52,0.3397,0.3473,0.3425,0.3910}, 
{53,0.3371,0.3451,0.3390,0.3880}, 
{54,0.3348,0.3431,0.3360,0.3850}, 
{55,0.3324,0.3410,0.3330,0.3820}, 
{56,0.3302,0.3391,0.3300,0.3800}, 
{57,0.3281,0.3372,0.3270,0.3770}, 
{58,0.3260,0.3353,0.3245,0.3745}, 
{59,0.3240,0.3335,0.3220,0.3720}, 
{60,0.3222,0.3318,0.3195,0.3700}, 
{61,0.3203,0.3301,0.3170,0.3680}, 
{62,0.3185,0.3284,0.3150,0.3660}, 
{63,0.3168,0.3267,0.3130,0.3630}, 
{64,0.3152,0.3252,0.3110,0.3610}, 
{65,0.3136,0.3237,0.3090,0.3590}, 
{66,0.3121,0.3221,0.3070,0.3570}, 
{67,0.3105,0.3206,0.3050,0.3551}, 
{68,0.3092,0.3193,0.3030,0.3538}, 
{69,0.3078,0.3179,0.3015,0.3520}, 
{70,0.3065,0.3166,0.3000,0.3500}, 
{71,0.3051,0.3152,0.2980,0.3480}, 
{72,0.3038,0.3139,0.2962,0.3465}, 
{73,0.3027,0.3127,0.2945,0.3450}, 
{74,0.3016,0.3116,0.2930,0.3430}, 
{75,0.3004,0.3104,0.2915,0.3420}, 
{76,0.2993,0.3092,0.2905,0.3390}, 
{77,0.2981,0.3080,0.2890,0.3380}, 
{78,0.2972,0.3070,0.2878,0.3370}, 
{79,0.2962,0.3060,0.2865,0.3360}, 
{80,0.2953,0.3049,0.2852,0.3350}, 
{81,0.2943,0.3039,0.2840,0.3338}, 
{82,0.2934,0.3029,0.2830,0.3322}, 
{83,0.2924,0.3018,0.2820,0.3310},
{84,0.2916,0.3009,0.2810,0.3300},
{85,0.2908,0.3000,0.2800,0.3290}, 
{86,0.2901,0.2992,0.2790,0.3280}, 
{87,0.2893,0.2983,0.2781,0.3270}, 
{88,0.2885,0.2974,0.2772,0.3258}, 
{89,0.2878,0.2965,0.2763,0.3245}, 
{90,0.2870,0.2956,0.2754,0.3235}, 
{91,0.2862,0.2948,0.2746,0.3224},
{92,0.2856,0.2941,0.2738,0.3212}, 
{93,0.2850,0.2934,0.2730,0.3200}, 
{94,0.2844,0.2927,0.2722,0.3190}, 
{95,0.2837,0.2919,0.2715,0.3180},
{96,0.2831,0.2912,0.2708,0.3170}, 
{97,0.2825,0.2905,0.2700,0.3160},
{98,0.2819,0.2898,0.2692,0.3150},
{99,0.2812,0.2890,0.2685,0.3135},
{100,0.2806,0.2883,0.2680,0.3120}
};

void color_light_init(void)
{
	Sys.Config.cct.max_pos = sizeof(cct_tab)/sizeof(CCT_TAB);
}


/*
INPUT:pos:number;offset;
OUTPUT:target,point of coordinate
RETURN:0,OK;1,fail
*/
int CCTToCoordinate(unsigned char pos,int offset,COORD *target)
{
	COORD neg_offset;//��ƫ����
	COORD pos_offset,centre;
	if( pos >= sizeof(cct_tab)/sizeof(CCT_TAB) ||(offset>100)||(offset<-100))
	{
		return 1;
	}
	pos_offset = cct_tab[pos].offset;
	centre = cct_tab[pos].centre;
	neg_offset.x = 2*centre.x - pos_offset.x;
	neg_offset.y = 2*centre.y - pos_offset.y;
	if(offset>0)
	{
		target->x = (pos_offset.x-centre.x)*offset/100.0 + centre.x;
		target->y = (pos_offset.y-centre.y)*offset/100.0 + centre.y;
	}
	else
	{
		offset *= -1;
		target->x = (neg_offset.x-centre.x)*offset/115.0 + centre.x;
		target->y = (neg_offset.y-centre.y)*offset/115.0 + centre.y;
	}
	return 0;
}


/*
INPUT:pos:number;offset,grn;pixel,0,ALL;
OUTPUT:NO ONE
RETURN:NO ONE
*/
void LightCCTOut(unsigned char pos,int offset,float dim,unsigned char pixel)
{
	COORD target;//��ƫ����
	if(CCTToCoordinate(pos,offset,&target))
		return;
	CoordinateOut(&target,dim,pixel);
	Debug_printf("\r\n");
	Debug_printf("ɫ����(%.4f,%.4f)\r\n",target.x,target.y);
	Debug_printf("RGB(%.4f,%.4f,%.4f)\r\n",rgbk.r,rgbk.g,rgbk.b);
}

int CoordinateOut(COORD *coord,float dim,unsigned char pixel)
{
	if(coordinate_to_RGBWWCW(*coord,&rgbk))
	{
		return 1;
	}
	if(pixel==0)
		AllLedPowerOut(&rgbk,dim);
	else
   LedPowerOut(&rgbk,dim,pixel-1);
}

void LightRGBOut(unsigned int R,unsigned int G,unsigned int B,unsigned char pixel)
{
	float dim = (R+G+B)/30000.0;
	rgb.r = R;
	rgb.g = G;
	rgb.b = B;
	RGB_to_coordinate(rgb,&coord);
	if(coordinate_to_RGBWWCW(coord,&rgbk))
	{

		Debug_printf("RGB �ϳɲ���\r\n");
		return ;
	}
	if(pixel>3)
		AllLedPowerOut(&rgbk,dim);
	else
		LedPowerOut(&rgbk,dim,pixel);
}
//Test Code
////////////////////////////////////////////////////////

void ColorTest(void)
{
	HSI hsi;
	char buf[100];
	hsi.i = 1;
	for(hsi.h=0;hsi.h<360;hsi.h++)
	{
		hsi.s = 1; //��Щ���Ͷ�̫�ߵ���ɫ�ϳɲ���
		do
		{
			HSI_to_RGB(hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			hsi.s -= 0.01;//�ҵ����Ͷ������Ǹ���
		}
		while(coordinate_to_RGBYW(coord,&rgbk)&&(hsi.s>0));
		hsi.s += 0.01;

		while(hsi.s>0)
		{
			HSI_to_RGB(hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			if(coordinate_to_RGBYW(coord,&rgbk))
			{
				Debug_printf("���Ͷ�ת����:No Color\r\n");
				Debug_printf("\r\n");
				Debug_printf("HSI(%d,%.2f,%.2f)\r\n",hsi.h,hsi.s,hsi.i);
				Debug_printf("ɫ����(%.4f,%.4f)\r\n",coord.x,coord.y);					
				Debug_printf("\r\n");
			}
			hsi.s -= 0.01;
		}
	}
}

void HSI_Test(void)
{
	Sys.Config.hsi.i = 1;
	for(Sys.Config.hsi.h=0;Sys.Config.hsi.h<360;Sys.Config.hsi.h++)
	{
		Sys.Config.hsi.s = 1; //��Щ���Ͷ�̫�ߵ���ɫ�ϳɲ���
		while(Sys.Config.hsi.s>0)
		{
			HSI_to_RGB(Sys.Config.hsi,&rgb);
			RGB_to_coordinate(rgb,&coord);
			Sys.Config.hsi.s -= 0.01;
		}
	}
}






