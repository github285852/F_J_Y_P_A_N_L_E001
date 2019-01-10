#include "includes.h"
#include "stdio.h"
#include "stdlib.h"

void point_out(void)
{
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,0,MENU_H-16,16,(char *)"press \"MENU\"");
	else
		Pictrue_printf(&MenuPic,0,MENU_H-16,16,(char *)"按确认键确认");
}


//界面执行函数
//////////////////////////////////////////
void HSIDisplay(void)
{
	int i,offset;
	float dim;
	char buf[50];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<DIS_MAX_LINE;i++)
		Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
		sprintf(buf,"Hue:%d",Sys.Config.hsi.h);
	else
		sprintf(buf,"色调:%d",Sys.Config.hsi.h);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
	i = Sys.Config.hsi.s*100;
	if(Sys.Config.english)
		sprintf(buf,"Sat:%d%%",i);
	else
		sprintf(buf,"饱和度:%d%%",i);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET-8,Rect[1].y0+offset,100,16,16,buf,0);
	dim = Sys.Config.hsi.i*100;
	if(Sys.Config.english)
		sprintf(buf,"Dim:%0.1f%%",dim);
	else
	{
		if(100.0>dim)
			sprintf(buf,"亮度:%0.1f%%",dim);
		else
			sprintf(buf,"亮度:100%%");
	}
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,0);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}
void HSIInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("HSI MODE");
	else
		HeadDisplay("HSI模式");
	while(DMAING);
	HSIDisplay();
	Sys.Config.lightmode = HSI_M;
	ColorLightHSIOut(Sys.Config.hsi,0);
}
u8 EC11_speed_cnt;
void HSITask(void)
{
//	if(pic.data == NULL)
//	{
//		return;
//	}
	if(ec11_pos[0]|ec11_pos[1]|ec11_pos[2])
	{		
		if(ec11_pos[0])
		{
			if(EC11_speed>45)
			{
				EC11_speed_cnt++;
				//if(EC11_speed_cnt>2)//消抖
				{
					Sys.Config.hsi.h -= ec11_pos[0]*0.05*EC11_speed;
					EC11_speed_cnt = 0;
				}
			}
			else
			{
				Sys.Config.hsi.h -= ec11_pos[0];
				EC11_speed_cnt = 0;
			}
			if(Sys.Config.hsi.h>359)
				Sys.Config.hsi.h = 359;
			if(Sys.Config.hsi.h<0)
				Sys.Config.hsi.h = 0;
			ec11_pos[0] = 0;
		}	
		if(ec11_pos[1])
		{
			if(EC11_speed>45)
			{
				EC11_speed_cnt++;
				//if(EC11_speed_cnt>2)
				{
					Sys.Config.hsi.s -= ec11_pos[1]*0.0005*EC11_speed;
					EC11_speed_cnt = 0;
				}
			}
			else
			{
				EC11_speed_cnt = 0;
				Sys.Config.hsi.s -= ec11_pos[1]*0.01;
			}
			if(Sys.Config.hsi.s>1)
				Sys.Config.hsi.s = 1;
			if(Sys.Config.hsi.s<0)
				Sys.Config.hsi.s = 0;
			ec11_pos[1] = 0;
		}		
		if(ec11_pos[2])
		{
			if(EC11_speed>25)
			{
				EC11_speed_cnt++;
			//	if(EC11_speed_cnt>2)
				{
					Sys.Config.hsi.i -= ec11_pos[2]*0.0005*EC11_speed;
					EC11_speed_cnt = 0;
				}
			}
			else
			{
				EC11_speed_cnt = 0;
				Sys.Config.hsi.i -= ec11_pos[2]*0.001;
			}
			if(Sys.Config.hsi.i>1)
				Sys.Config.hsi.i = 1;
			if(Sys.Config.hsi.i<0)
				Sys.Config.hsi.i = 0;
			ec11_pos[2] = 0;
		}
		HSIDisplay();//刷新界面
		ColorLightHSIOut(Sys.Config.hsi,0);
	}
	if(key_value==S10)
	{
		Menu_back();
	}
}

////////////////////////////////////////////////////////////////////CCT
void CCTDisplay(void)
{
	int i,offset;
	char buf[15];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<DIS_MAX_LINE;i++)
		Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Sys.Config.cct.kvn = cct_tab[Sys.Config.cct.pos].num;
	if(Sys.Config.english)
		sprintf(buf,"Kvn:%dK",Sys.Config.cct.kvn*100);
	else
		sprintf(buf,"色温:%dK",Sys.Config.cct.kvn*100);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"Grn:%d",Sys.Config.cct.grn);
	else
		sprintf(buf,"绿偏:%d",Sys.Config.cct.grn);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"Dim:%0.1f%%",Sys.Config.cct.dim*100);
	else
	{
		if(1.0>Sys.Config.cct.dim)
			sprintf(buf,"亮度:%0.1f%%",Sys.Config.cct.dim*100);
		else
			sprintf(buf,"亮度:100%%");
	}
	//	sprintf(buf,"亮度:%0.1f%%",Sys.Config.cct.dim*100);
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,0);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void CCTInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("CCT MODE");//CCT MODE
	else
		HeadDisplay("CCT模式");//CCT MODE
	LightCCTOut(Sys.Config.cct.pos,Sys.Config.cct.grn,Sys.Config.cct.dim,0);
	CCTDisplay();
	Sys.Config.lightmode = CCT_M;
}

void CCTTask(void)
{
//	if(pic.data == NULL)
//	{
//		return;
//	}
	if(ec11_pos[0]|ec11_pos[1]|ec11_pos[2])
	{
		if(ec11_pos[0])
		{
			Sys.Config.cct.pos -= (ec11_pos[0]*1);
			if(Sys.Config.cct.pos>Sys.Config.cct.max_pos)
				Sys.Config.cct.pos = Sys.Config.cct.max_pos;
			if(Sys.Config.cct.pos<0)
				Sys.Config.cct.pos = 0;
			ec11_pos[0] = 0;
		}	
		if(ec11_pos[1])
		{
			if(EC11_speed>30)
			{
				EC11_speed_cnt++;
			//	if(EC11_speed_cnt>2)
				{
					Sys.Config.cct.grn  -= ec11_pos[1]*0.05*EC11_speed;
					EC11_speed_cnt = 0;
				}
			}
			else
			{
				Sys.Config.cct.grn  -= ec11_pos[1];
				EC11_speed_cnt = 0;
			}
			if(Sys.Config.cct.grn>100)
			{
				Sys.Config.cct.grn = 100;
			}else if(Sys.Config.cct.grn<-100)
			{
				Sys.Config.cct.grn = -100;
			}
			ec11_pos[1] = 0;
		}	
		if(ec11_pos[2])
		{
			if(EC11_speed>25)
			{
				EC11_speed_cnt++;
			//	if(EC11_speed_cnt>2)
				{
					EC11_speed_cnt = 0;
					Sys.Config.cct.dim -= ec11_pos[2]*0.0005*EC11_speed;
				}
			}
			else
			{
				EC11_speed_cnt = 0;
				Sys.Config.cct.dim -= ec11_pos[2]*0.001;
			}
			if(Sys.Config.cct.dim>1)
			{
				Sys.Config.cct.dim = 1;
			}
			else if(Sys.Config.cct.dim<0)
			{
				Sys.Config.cct.dim = 0;
			}
			ec11_pos[2] = 0;
		}
		LightCCTOut(Sys.Config.cct.pos,Sys.Config.cct.grn,Sys.Config.cct.dim,0);
		CCTDisplay();
	}
	if(key_value==S10)
	{
		Menu_back();
	}
}

//////////////////////////////////////////////////////GEL

void GELDisplay(void)
{
	int i;
	char buf[50];
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	RECT rect[3]={
	{0,0,MENU_W,20},
	{0,24,MENU_W,76},
	{0,80,MENU_W,100},
	};
	
	for(i=0;i<3;i++)
		Picture_FillRect(MenuPic,rect[i],OPTION_COLOR);
	if(Sys.Config.english)
	{
		if(Sys.Config.gel.source)
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],0,16,"SOURCE:DAY LIGHT");
		}
		else
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],0,16,"SOURCE:TUNGSTEN");
		}
		sprintf(buf,"GEL:%s",GEL_TAB[Sys.Config.gel.number].NAME[0]);
		Picture_ShowString(MenuPic,rect[1].x0+16,rect[1].y0+1,MENU_W,16,16,buf,0);
		Pictrue_printf(&MenuPic,rect[1].x0+1,rect[1].y0+17,16,GEL_TAB[Sys.Config.gel.number].NAME[1]);
		sprintf(buf,"DIM:%0.1f%%",Sys.Config.gel.dim*100);
		Picture_ShowStringInRectCenter(MenuPic,rect[2],0,16,buf);
	}
	else
	{
		if(Sys.Config.gel.source)
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],0,16,"光源:日光灯");
		}
		else
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],0,16,"光源:钨丝灯");
		}
		sprintf(buf,"GEL:%s",GEL_TAB[Sys.Config.gel.number].NAME[0]);
		Picture_ShowString(MenuPic,rect[1].x0+16,rect[1].y0+1,MENU_W,16,16,buf,0);
		Pictrue_printf(&MenuPic,rect[1].x0+1,rect[1].y0+17,16,GEL_TAB[Sys.Config.gel.number].NAME[1]);
		sprintf(buf,"亮度:%0.1f%%",Sys.Config.gel.dim*100);
		Picture_ShowStringInRectCenter(MenuPic,rect[2],0,16,buf);
	}
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void GELInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("GEL MODE");
	else
		HeadDisplay("GEL模式");
	LightGELOut(&Sys.Config.gel,0);
	GELDisplay();
	Sys.Config.lightmode = GEL_M;
}

void GELTask(void)
{
	int temp;
	if(ec11_pos[0]|ec11_pos[1]|ec11_pos[2])
	{
		if(ec11_pos[2])//dim
		{
			if(EC11_speed>25)
			{
				EC11_speed_cnt++;
			//	if(EC11_speed_cnt>2)
				{
					EC11_speed_cnt = 0;
					Sys.Config.gel.dim -= ec11_pos[2]*0.0005*EC11_speed;
				}
			}
			else
			{
				EC11_speed_cnt = 0;
				Sys.Config.gel.dim -= ec11_pos[2]*0.001;
			}
			if(Sys.Config.gel.dim>1)
			{
				Sys.Config.gel.dim = 1;
			}
			else if(Sys.Config.gel.dim<0)
			{
				Sys.Config.gel.dim = 0;
			}
			ec11_pos[2] = 0;	
		}			
		if(ec11_pos[0])
		{
			if(Sys.Config.gel.source)
			{
				Sys.Config.gel.source = 0;
			}
			else
			{
				Sys.Config.gel.source = 1;
			}
			ec11_pos[0] = 0;
		}	
		if(ec11_pos[1])
		{			
			temp = Sys.Config.gel.number+0;
			temp -= (int)ec11_pos[1]*1;
			if(temp > (Sys.max_gel_number-1))
			{
				temp = Sys.max_gel_number-1;
			}
			else if(temp<0)
			{
				temp = 0;
			}
			Sys.Config.gel.number  = (unsigned char)temp;
			ec11_pos[1] = 0;
		}	
		LightGELOut(&Sys.Config.gel,0);
		GELDisplay();
	}
	if(key_value==S10)
	{
		Menu_back();
	}
}

////////////////////////////////////////////////////////RGB
void RGBDisplay(void)
{
	int i,offset;
	char buf[15];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<DIS_MAX_LINE;i++)
		Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
		sprintf(buf,"R:%d",Sys.Config.rgb.r);	
	else
		sprintf(buf,"红:%d",Sys.Config.rgb.r);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"G:%d",Sys.Config.rgb.g);
	else
		sprintf(buf,"绿:%d",Sys.Config.rgb.g);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"B:%d",Sys.Config.rgb.b);
	else
		sprintf(buf,"蓝:%d",Sys.Config.rgb.b);
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,0);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void RGBInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("RGB MODE");
	else
		HeadDisplay("RGB模式");
	while(DMAING);
	LightRGBOut(Sys.Config.rgb.r,Sys.Config.rgb.g,Sys.Config.rgb.b,0);
	RGBDisplay();
	Sys.Config.lightmode = RGB_M;
}

void RGBTask(void)
{
//	if(pic.data == NULL)
//	{
//		return;
//	}
	if(ec11_pos[0]|ec11_pos[1]|ec11_pos[2])
	{
		if(ec11_pos[2])
		{
			if(EC11_speed>30)
			{
				Sys.Config.rgb.r  -= ec11_pos[2]*0.5*EC11_speed;
			}
			else
			{
				Sys.Config.rgb.r -= ec11_pos[2];
			}
			if(Sys.Config.rgb.r>10000)
			{
				Sys.Config.rgb.r = 10000;
			}else if(Sys.Config.rgb.r<0)
			{
				Sys.Config.rgb.r = 0;
			}
			ec11_pos[2] = 0;
		}		
		if(ec11_pos[1])
		{
			if(EC11_speed>30)
			{
				Sys.Config.rgb.g  -= ec11_pos[1]*0.5*EC11_speed;
			}
			else
			{
				Sys.Config.rgb.g  -= ec11_pos[1];
			}
			if(Sys.Config.rgb.g>10000)
			{
				Sys.Config.rgb.g = 10000;
			}else if(Sys.Config.rgb.g<0)
			{
				Sys.Config.rgb.g = 0;
			}
			ec11_pos[1] = 0;
		}	
		if(ec11_pos[0])
		{
			if(EC11_speed>30)
			{
				Sys.Config.rgb.b  -= ec11_pos[0]*0.5*EC11_speed;
			}
			else
			{
				Sys.Config.rgb.b  -= ec11_pos[0];
			}
			if(Sys.Config.rgb.b>10000)
			{
				Sys.Config.rgb.b = 10000;
			}else if(Sys.Config.rgb.b<0)
			{
				Sys.Config.rgb.b = 0;
			}
			ec11_pos[0] = 0;
		}	
		LightRGBOut(Sys.Config.rgb.r,Sys.Config.rgb.g,Sys.Config.rgb.b,0xff);
		RGBDisplay();
	}
	if(key_value==S10)
	{
		Menu_back();
	}
}

void ClearMenu(u16 color)
{
	Picture_Fill(MenuPic,color);
}
void FreshMenu(void)
{
	
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void SceneBegain(SceneData *pscene);
SceneData *pscene;
SceneData scendatabuf;
extern RGB rgbk;
//闪烁场景
float FlashDim = 0.5;
u16 FlashTimOn = 100;
u16 FlashTimOff = 100;
u8 flash_flg = 0;
void SceneFlashInit(void)
{
	u16 current[LED_NUMS];
	int i,j;
//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	pscene = &scendatabuf;
	if(pscene != NULL)
	{
		pscene->max_frames = 1;
		pscene->mode = 0;//无限循环
		LedkToCurrent(&rgbk,FlashDim,2);
		for(i=0;i<2;i++)
		{
			for(j=0;j<LED_NUMS;j++)
				pscene->frame[0].ch[i][j] = current[j];
			
			for(j=0;j<LED_NUMS;j++)
				pscene->frame[1].ch[i][j] = 0;
			
			pscene->frame[0].tim = FlashTimOn;
			pscene->frame[1].tim = FlashTimOff;
		}
	}
	else
	{
		Debug_printf(">>SceneFlashInit:mymalloc()fail\r\n");
	}
	SceneBegain(pscene);
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 1;
	Sys.menu_mask = 1;
	flash_flg = 0;
}

void SceneFlashTask(void)//50 ms
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	static int tim=0,tim_cnt=0;
	if(ec11_pos[2])
	{
		if(EC11_speed>25)
		{
			FlashDim -= ec11_pos[2]*0.0005*EC11_speed;
		}
		else
		{
			FlashDim -= ec11_pos[2]*0.001;
		}
		if(FlashDim>1)
		{
			FlashDim = 1;
		}
		else if(FlashDim<0)
		{
			FlashDim = 0;
		}
		ec11_pos[2] = 0;
		SceneFlashInit();
	}
	if(ec11_pos[1])
	{
		if(EC11_speed>25)
		{
			FlashTimOff -= ec11_pos[1]*0.5*EC11_speed;
		}
		else
		{

			FlashTimOff -= ec11_pos[1];
		}
		if(FlashTimOff<FRAME_MIN_T)
		{
			FlashTimOff = FRAME_MIN_T;
		}
		ec11_pos[1] = 0;
		SceneFlashInit();
	}
	if(ec11_pos[0])
	{
		if(EC11_speed>25)
		{
			FlashTimOn -= ec11_pos[0]*0.5*EC11_speed;
		}
		else
		{

			FlashTimOn -= ec11_pos[0];
		}
		if(FlashTimOn<FRAME_MIN_T)
		{
			FlashTimOn = FRAME_MIN_T;
		}
		ec11_pos[0] = 0;
		SceneFlashInit();
	}
	Pictrue_printf(&MenuPic,0,0,16,	"  Flash");
	Pictrue_printf(&MenuPic,0,16,16,"  ON:%dms",FlashTimOn);
	Pictrue_printf(&MenuPic,0,32,16,"  OFF:%dms",FlashTimOff);
	Pictrue_printf(&MenuPic,0,48,16,"  Dim:%0.1f%%",FlashDim*100);
	FreshMenu();
	if(key_value == S10)
	{
		if(flash_flg==0)
		{
			flash_flg = 1;
			SceneTimDisable();
		}
		else
		{
			flash_flg = 0;
			SceneTimEnable();
		}
	}
	if(key_value == S9)
	{
		flash_flg = 1;
		SceneTimDisable();
	}
}


//SOS 三短三长三短
void SceneSOSInit(void)
{
	u16 current[LED_NUMS];
	int i,j,k;
//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	pscene = &scendatabuf;
	FlashTimOn = 1000; //代表长
	FlashTimOff = 500;
	if(pscene != NULL)
	{
		pscene->max_frames = 18;
		pscene->mode = 0;//无限循环
		LedkToCurrent(&rgbk,FlashDim,2);
		for(i=0;i<2;i++)
		{
			for(k=0;k<9;k++)
			{
				for(j=0;j<LED_NUMS;j++)
					pscene->frame[k*2].ch[i][j] = current[j];
				
				for(j=0;j<LED_NUMS;j++)
					pscene->frame[k*2+1].ch[i][j] = 0;
			}
			for(j=0;j<6;j++)
				pscene->frame[j].tim = FlashTimOff;//短
			for(j=6;j<12;j++)
				pscene->frame[j].tim = FlashTimOn;//长
			for(j=12;j<18;j++)
				pscene->frame[j].tim = FlashTimOff;
			
			for(j=0;j<LED_NUMS;j++)
				pscene->frame[18].ch[i][j] = 0;
			pscene->frame[18].tim = 2000;
		}
	}
	else
	{
		Debug_printf(">>SceneFlashInit:mymalloc()fail\r\n");
	}
	SceneBegain(pscene);
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 2;
	Sys.menu_mask = 1;
	flash_flg = 0;
}

void SceneSOSTask(void)//50 ms
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	static int tim=0,tim_cnt=0;
	
	if(ec11_pos[2])
	{
		if(EC11_speed>25)
		{
			FlashDim -= ec11_pos[2]*0.0005*EC11_speed;
		}
		else
		{
			FlashDim -= ec11_pos[2]*0.001;
		}
		if(FlashDim>1)
		{
			FlashDim = 1;
		}
		else if(FlashDim<0)
		{
			FlashDim = 0;
		}
		ec11_pos[2] = 0;
		SceneSOSInit();
	}
	if(ec11_pos[1])
	{
		if(EC11_speed>25)
		{
			FlashTimOff -= ec11_pos[1]*0.5*EC11_speed;
		}
		else
		{

			FlashTimOff -= ec11_pos[1];
		}
		if(FlashTimOff<FRAME_MIN_T)
		{
			FlashTimOff = FRAME_MIN_T;
		}
		ec11_pos[1] = 0;
		SceneSOSInit();
	}
	if(ec11_pos[0])
	{
		if(EC11_speed>25)
		{
			FlashTimOn -= ec11_pos[0]*0.5*EC11_speed;
		}
		else
		{

			FlashTimOn -= ec11_pos[0];
		}
		if(FlashTimOn<FRAME_MIN_T)
		{
			FlashTimOn = FRAME_MIN_T;
		}
		ec11_pos[0] = 0;
		SceneSOSInit();
	}
	Pictrue_printf(&MenuPic,0,0,16,	"  SOS");
	Pictrue_printf(&MenuPic,0,16,16,"  Long:%dms",FlashTimOn);
	Pictrue_printf(&MenuPic,0,32,16,"  Short:%dms",FlashTimOff);
	Pictrue_printf(&MenuPic,0,48,16,"  Dim:%0.1f%%",FlashDim*100);
	FreshMenu();
	
	if(key_value == S10)
	{
		if(flash_flg==0)
		{
			flash_flg = 1;
			SceneTimDisable();
		}
		else
		{
			flash_flg = 0;
			SceneSOSInit();
		}
	}
	if(key_value == S9)
	{
		flash_flg = 1;
		SceneTimDisable();
	}
}

const u16 stom_table[8][11] = 
{
{0,0xFFFF,0,0,0,0,0xFFFF,0,0,0,30},
{0,0,0,0,0,0,0,0,0,0,80},
{0,0x7FFF,0,0,0,0,0x7FFF,0,0,0,20},
{0,0,0,0,0,0,0,0,0,0,80},
{0,0x3FFF,0,0,0,0,0x3FFF,0,0,0,20},
{0,0,0,0,0,0,0,0,0,0,80},
{0,0,0,0,0,0,0,0,0,0,80},
{0,0x1FFF,0,0,0,0,0x1FFF,0,0,0,20}
};

void SceneStormInit(void)
{
	int i,j,k;
//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	pscene = &scendatabuf;
	pscene->max_frames = 8;
	pscene->mode = 255;//播放完熄灭
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<5;j++)
		{
			pscene->frame[i].ch[0][j] = stom_table[i][j];
			pscene->frame[i].ch[1][j] = stom_table[i][5+j];
			pscene->frame[i].tim = stom_table[i][10];
		}
	}
	SceneBegain(pscene);
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 3;
	Sys.menu_mask = 1;
	flash_flg = 0;
}

void SceneStormTask(void)//50 ms
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Pictrue_printf(&MenuPic,0,0,16,	"  STORM");
	FreshMenu();
	if(key_value == S10)
	{
		SceneStormInit();
	}
	if(key_value == S9)
	{
		flash_flg = 1;
		SceneTimDisable();
	}
}



void SceneBack(void)
{
	SceneTimDisable();
//	myfree(SRAMIN,pscene);
	pscene = 0;
}

void InfoTaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("INFO");
	else
		HeadDisplay("设备信息");
}	


void InfoTask(void)
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"Version:%s",VERSION);
		Pictrue_printf(&MenuPic,0,16,16,(char *)"DMX protocol:%s",DXM_PRO);
		Pictrue_printf(&MenuPic,0,32,16,(char *)"Primary colours:%s",PrimaryColours);
	}
	else
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"版本:%s",VERSION);
		Pictrue_printf(&MenuPic,0,16,16,(char *)"DMX协议版本:%s",DXM_PRO);
		Pictrue_printf(&MenuPic,0,32,16,(char *)"基色:%s",PrimaryColours);
	}
	FreshMenu();
}	

void MonitorInit(void)
{
	u32 h = Sys.Config.work_time/3600; // 时
	u8 m;  //分
	u8 s;  //
	u16 temp = Sys.Config.work_time%3600;
	m = temp/60;
	s = temp%60;
	if(Sys.Config.english)
		HeadDisplay("MONITOR");
	else
		HeadDisplay("监控");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"PANEL T:%0.1f\nWORK TIME %d:%d:%d",Sys.panle_t,h,m,s);
	}
	else
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"灯板温度:%0.1f\n工作时间 %d:%d:%d",Sys.panle_t,h,m,s);
	}
	FreshMenu();
}	



void MonitorTask(void)
{
	u32 h = Sys.Config.work_time/3600; // 时
	u8 m;  //分
	u8 s;  //
	u16 temp = Sys.Config.work_time%3600;
	m = temp/60;
	s = temp%60;
	static int i;
	i++;
	if(i<20)
	{
		return;
	}
	i = 0;
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"PANEL T:%0.1f\nWORK TIME %d:%d:%d",Sys.panle_t,h,m,s);
	}
	else
	{
		Pictrue_printf(&MenuPic,0,0,16,(char *)"灯板温度:%0.1f\n工作时间 %d:%d:%d",Sys.panle_t,h,m,s);
	}
	FreshMenu();
}		

void DefaultInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("DEFAULT");
	else
		HeadDisplay("出厂设置");
	ClearMenu(OPTION_COLOR);
	FreshMenu();
	Sys.menu_mask = 1;
}	

void DefaultTask(void)
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
	{
		Pictrue_printf(&MenuPic,16,0,16,(char *)"Are you sure to restore default parameters?");
	}
	else
	{
		Pictrue_printf(&MenuPic,16,0,16,(char *)"确定要恢复出厂设置吗?");
	}
	if(key_value == S10)
	{
		default_data.work_time = Sys.Config.work_time;
		Sys.Config = default_data;
		menu_key = Back_Key;
		Sys.menu_mask = 0;
	}
	point_out();
	FreshMenu();
}

u8 Art_net_en=0;
void DMXINTaskInit(void)
{
	u8 offset = (MENU_W%16)/2; // 水平中心对齐
	Art_net_en = 0;
	if(Sys.Config.english)
	{
		HeadDisplay("DMX IN");
	}
	else
	{
		
		HeadDisplay("DMX输入");
	}
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,offset,0,16,"  Select DMX IN.");
	else
		Pictrue_printf(&MenuPic,offset,0,16,"  选择DMX输入。");
	point_out();
	FreshMenu();
}

void AutoDetectINInit(void)
{
	u8 offset = (MENU_W%16)/2; // 水平中心对齐
	if(Sys.Config.english)
		HeadDisplay("AUTO DETECT");
	else
		HeadDisplay("自动检测");
	ClearMenu(OPTION_COLOR);
	/*
	执行网络检测
	*/
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(0)
	{
		Art_net_en = 1;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,offset,offset,16,"  Detect the Ethernet,Interface is Art-Net.");
		else
			Pictrue_printf(&MenuPic,offset,offset,16,"  检测到以太网连接,使用Art-Net接口。");

	}
	else
	{
		Art_net_en = 0;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,offset,0,16,"  Interface is DMX.");
		else
			Pictrue_printf(&MenuPic,offset,offset,16,"  使用DMX接口。");
	}
	point_out();
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void Art_NetINTask(void)
{
	u8 offset = (MENU_W%16)/2; // 水平中心对齐
	static unsigned char t,i;
	t++;
	if(t>20)//50ms*20 = 1S
	{
		t = 0;
	}
	if(t != 1)
		return ;
	i++;
	ClearMenu(OPTION_COLOR);
	/*
	执行网络检测
	*/
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(0)
	{
		Art_net_en = 1;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,16,0,16,"",i);
		else
			
		FreshMenu();
	}
	else
	{
		/*
		
		*/
		Art_net_en = 0;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,offset,0,16,"  Don't detect \"RJ45\"!Default is DMX.");
		else
			Pictrue_printf(&MenuPic,offset,0,16,"  没有检测到RJ45连接，默认使用DMX。");
	}
	FreshMenu();
  if(key_value==S10)
	{
		Menu_back();
	}
}

void Save_Interface(void)
{
	Sys.Config.Art_net_en =  Art_net_en;
	SaveConfig();
}

u8 temp_dmx_mode;
void DMX_ModeTaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("DMX MODE");
	else
		HeadDisplay("DMX模式");
	temp_dmx_mode = Sys.Config.dmx.mode;
	DMX_ModeTask();
}

void DMX_ModeDisplay(char *str1,char *str2,u8 ch)
{
	
	int i,offset;
	char buf[50];
	static TRANDISPLAY tran;
	offset = (RECT_H-16)/2;;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<3;i++)
		Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	BACK_COLOR = LGRAY;
	POINT_COLOR = BLACK;
	sprintf(buf,"%s",str1);
//	Picture_ShowStringInRectCenter(MenuPic,Rect[0],1,16,buf);
	tran.move_t = 30;
	Picture_TranDispalyOnline(&MenuPic,&Rect[0],&tran,16,1,buf);
	//Picture_ShowString(MenuPic,Rect[0].x0,Rect[0].y0+offset,100,16,16,buf,0);
	sprintf(buf,"%s",str2);
	Picture_ShowStringInRectCenter(MenuPic,Rect[1],1,16,buf);
	//Picture_ShowString(MenuPic,Rect[1].x0,Rect[1].y0+offset,100,16,16,buf,0);
	sprintf(buf,"CH:%d-%d",Sys.Config.dmx.addr,Sys.Config.dmx.addr+ch-1);
	//Picture_ShowString(MenuPic,Rect[2].x0,Rect[2].y0+offset,100,16,16,buf,0);
		Picture_ShowStringInRectCenter(MenuPic,Rect[2],1,16,buf);
	FreshMenu();
}


void DMX_ModeTask(void)
{
	if(ec11_pos[2])
	{
		if(ec11_pos[2]>0)
			temp_dmx_mode--;
		else
			temp_dmx_mode++;
		if(temp_dmx_mode>15)
			temp_dmx_mode = 15;
		if(temp_dmx_mode<1)
		{
			temp_dmx_mode = 1;
		}
		ec11_pos[2] = 0;
	}

	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	if(Sys.Config.english)
		switch(temp_dmx_mode)
		{
			case DMX_M1:DMX_ModeDisplay("M1:CCT&RALGB","8BITS",sizeof(CCT_LEDK_8BIT));break;
			case DMX_M2:DMX_ModeDisplay("M2:CCT","8BITS",sizeof(CCT_8BIT));break;		
			case DMX_M3:DMX_ModeDisplay("M3:CCT&HSI","8BITS",sizeof(CCT_HSI_8BIT));break;
			case DMX_M4:DMX_ModeDisplay("M4:RALGB","8BITS",sizeof(LEDK_8BIT));break;
			case DMX_M5:DMX_ModeDisplay("M5:HSI","8BITS",sizeof(HSI_8BIT));break;
			case DMX_M6:DMX_ModeDisplay("M6:CCT&RALGB","16BITS",sizeof(CCT_LEDK_16BIT));break;
			case DMX_M7:DMX_ModeDisplay("M7:CCT","16BITS",sizeof(CCT_16BIT));break;
			case DMX_M8:DMX_ModeDisplay("M8:CCT&HSI","16BITS",sizeof(CCT_HSI_16BIT));break;
			case DMX_M9:DMX_ModeDisplay("M9:RALGB","16BITS",sizeof(LEDK_16BIT));break;
			case DMX_M10:DMX_ModeDisplay("M10:HSI","16BITS",sizeof(HSI_16BIT));break;		
			case DMX_M11:DMX_ModeDisplay("M11:CCT&RALGB","Coare/Fine",sizeof(CCT_LEDK_CF));break;
			case DMX_M12:DMX_ModeDisplay("M12:CCT","Coare/Fine",sizeof(CCT_CF));break;
			case DMX_M13:DMX_ModeDisplay("M13:CCT&HSI","Coare/Fine",sizeof(CCT_HSI_CF));break;
			case DMX_M14:DMX_ModeDisplay("M14:RALGB","Coare/Fine",sizeof(LEDK_CF));break;
			case DMX_M15:DMX_ModeDisplay("M15:HSI","Coare/Fine",sizeof(HSI_CF));break;
			default:break;
		}
	else
		switch(temp_dmx_mode)
		{
			case DMX_M1:DMX_ModeDisplay("M1:CCT&RALGB","8位",sizeof(CCT_LEDK_8BIT));break;
			case DMX_M2:DMX_ModeDisplay("M2:CCT","8位",sizeof(CCT_8BIT));break;		
			case DMX_M3:DMX_ModeDisplay("M3:CCT&HSI","8位",sizeof(CCT_HSI_8BIT));break;
			case DMX_M4:DMX_ModeDisplay("M4:RALGB","8位",sizeof(LEDK_8BIT));break;
			case DMX_M5:DMX_ModeDisplay("M5:HSI","8位",sizeof(HSI_8BIT));break;
			case DMX_M6:DMX_ModeDisplay("M6:CCT&RALGB","16位",sizeof(CCT_LEDK_16BIT));break;
			case DMX_M7:DMX_ModeDisplay("M7:CCT","16位",sizeof(CCT_16BIT));break;
			case DMX_M8:DMX_ModeDisplay("M8:CCT&HSI","16位",sizeof(CCT_HSI_16BIT));break;
			case DMX_M9:DMX_ModeDisplay("M9:RALGB","16B位",sizeof(LEDK_16BIT));break;
			case DMX_M10:DMX_ModeDisplay("M10:HSI","16位",sizeof(HSI_16BIT));break;		
			case DMX_M11:DMX_ModeDisplay("M11:CCT&RALGB","粗调/细调",sizeof(CCT_LEDK_CF));break;
			case DMX_M12:DMX_ModeDisplay("M12:CCT","粗调/细调",sizeof(CCT_CF));break;
			case DMX_M13:DMX_ModeDisplay("M13:CCT&HSI","粗调/细调",sizeof(CCT_HSI_CF));break;
			case DMX_M14:DMX_ModeDisplay("M14:RALGB","粗调/细调",sizeof(LEDK_CF));break;
			case DMX_M15:DMX_ModeDisplay("M15:HSI","粗调/细调",sizeof(HSI_CF));break;
			default:break;
		}
	if(key_value==S10)
	{
		Sys.Config.dmx.mode = temp_dmx_mode;
	//	SaveConfig();
		Menu_back();
	}	
}

u8 dmx_addr; 
void DMX_AdressDisplay(u8 ch)
{
	int i,offset;
	char buf[50];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<2;i++)
		Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
	{
		sprintf(buf,"ADDR:%d",dmx_addr);
		Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
		sprintf(buf,"CH:%d-%d",dmx_addr,dmx_addr+ch-1);
		Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,0);
	}
	else
	{
		sprintf(buf,"地址:%d",dmx_addr);
		Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
		sprintf(buf,"通道:%d-%d",dmx_addr,dmx_addr+ch-1);
		Picture_ShowStringInRectCenter(MenuPic,Rect[1],1,16,(char *)buf);
		//Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,0);
	}
	FreshMenu();
}

void DMX_AdressTaskInit(void)
{
	unsigned char ch;
		switch(Sys.Config.dmx.mode)
		{
			case DMX_M1:ch = sizeof(CCT_LEDK_8BIT);break;
			case DMX_M2:ch = sizeof(CCT_8BIT);break;		
			case DMX_M3:ch = sizeof(CCT_HSI_8BIT);break;
			case DMX_M4:ch = sizeof(LEDK_8BIT);break;
			case DMX_M5:ch = sizeof(HSI_8BIT);break;
			case DMX_M6:ch = sizeof(CCT_LEDK_16BIT);break;
			case DMX_M7:ch = sizeof(CCT_16BIT);break;
			case DMX_M8:ch = sizeof(CCT_HSI_16BIT);break;
			case DMX_M9:ch = sizeof(LEDK_16BIT);break;
			case DMX_M10:ch = sizeof(HSI_16BIT);break;		
			case DMX_M11:ch = sizeof(CCT_LEDK_CF);break;
			case DMX_M12:ch = sizeof(CCT_CF);break;
			case DMX_M13:ch = sizeof(CCT_HSI_CF);break;
			case DMX_M14:ch = sizeof(LEDK_CF);break;
			case DMX_M15:ch = sizeof(HSI_CF);break;
			default:break;
		}
	if(Sys.Config.english)
		HeadDisplay("SET DMX ADDR");
	else
		HeadDisplay("设置DMX地址");
	while(DMAING);
	dmx_addr = Sys.Config.dmx.addr;
	DMX_AdressDisplay(ch);
	
}

void DMX_AdressTask(void)
{
	unsigned char ch;
		switch(Sys.Config.dmx.mode)
		{
			case DMX_M1:ch = sizeof(CCT_LEDK_8BIT);break;
			case DMX_M2:ch = sizeof(CCT_8BIT);break;		
			case DMX_M3:ch = sizeof(CCT_HSI_8BIT);break;
			case DMX_M4:ch = sizeof(LEDK_8BIT);break;
			case DMX_M5:ch = sizeof(HSI_8BIT);break;
			case DMX_M6:ch = sizeof(CCT_LEDK_16BIT);break;
			case DMX_M7:ch = sizeof(CCT_16BIT);break;
			case DMX_M8:ch = sizeof(CCT_HSI_16BIT);break;
			case DMX_M9:ch = sizeof(LEDK_16BIT);break;
			case DMX_M10:ch = sizeof(HSI_16BIT);break;		
			case DMX_M11:ch = sizeof(CCT_LEDK_CF);break;
			case DMX_M12:ch = sizeof(CCT_CF);break;
			case DMX_M13:ch = sizeof(CCT_HSI_CF);break;
			case DMX_M14:ch = sizeof(LEDK_CF);break;
			case DMX_M15:ch = sizeof(HSI_CF);break;
			default:break;
		}
	if(ec11_pos[2])
	{
		if(EC11_speed>40)
		{
			dmx_addr -= (ec11_pos[2]*0.05*EC11_speed);
		}
		else
		{
			dmx_addr -= (ec11_pos[2]*1);
		}

		if(dmx_addr == 0xFF)
			dmx_addr = 256-ch;
		else if(dmx_addr>256-ch)
			dmx_addr = 0;
		DMX_AdressDisplay(ch);	
		ec11_pos[2] = 0;
	}	
	if(key_value==S10)
	{
		Sys.Config.dmx.addr = dmx_addr;
		//SaveConfig();
		Menu_back();
	}	
}

FAN fan;
void FAN_AUTO_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("FAN AUTO");
	else
		HeadDisplay("风扇自动控制");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_AUTO;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"Set FAN AUTO");
	else
		Pictrue_printf(&MenuPic,16,0,16,"风扇自动调节");
	//point_out();
	FreshMenu();
}

void FAN_OFF_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("FAN OFF");
	else
		HeadDisplay("风扇关闭");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_OFF;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"FAN OFF");
	else
		Pictrue_printf(&MenuPic,16,0,16,"关闭风扇");
	//point_out();
	FreshMenu();
}

void FAN_DMX_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("FAN DMX");
	else
		HeadDisplay("DMX 控制");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_DMX;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"FAN is control by DMX.");
	else
		Pictrue_printf(&MenuPic,16,0,16,"风扇通过DMX控制。");
//	point_out();
	FreshMenu();
	
	if(key_value==S10)
	{
		Menu_back();
	}	
}

void SaveFanConfig(void)
{
	Sys.Config.fan = fan;
	SaveConfig();
}

void LCD_DIM_display(void)
{
	int offset;
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	Picture_FillRect(MenuPic,Rect[0],OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Pictrue_printf(&MenuPic,Rect[0].x0+5,Rect[0].y0+offset,16,"%d%%",Sys.Config.lcd.dim);
	Pictrue_printf(&MenuPic,Rect[0].x0+5+32,Rect[0].y0+offset+4,12,"(10%%-100%%)",Sys.Config.lcd.dim);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}
void LCD_DIM_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("BACKLITE DIM");
	else
		HeadDisplay("亮度");
	LCD_DIM_display();
}

void LCD_DIM_Task(void)
{
	if(ec11_pos[2])
	{
		if(EC11_speed>30)
		{
				Sys.Config.lcd.dim -= ec11_pos[2]*0.05*EC11_speed;
		}
		else
		{
				Sys.Config.lcd.dim -= (ec11_pos[2]*1);
		}
		if(Sys.Config.lcd.dim <10)
		{
			Sys.Config.lcd.dim = 10;
		}
		else if(Sys.Config.lcd.dim>100)
		{
			Sys.Config.lcd.dim = 100;
		}
		LCD_DIM_display();
		ec11_pos[2] = 0;
	}	
	if(key_value==S10)
	{
		Menu_back();
	}	
}

void LCD_TIM_display(void)
{
	int offset;
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	POINT_COLOR = BLACK;
	if(Sys.Config.lcd.tim >30)
	{
		Picture_FillRect(MenuPic,Rect[0],OPTION_COLOR);
		Picture_FillRect(MenuPic,Rect[1],SELECT_COLOR);
		BACK_COLOR = OPTION_COLOR;
		Pictrue_printf(&MenuPic,Rect[0].x0+10,Rect[0].y0+offset,16,"30S");
		Pictrue_printf(&MenuPic,Rect[0].x0+10+24,Rect[0].y0+offset+4,12,"(10S-30S)",Sys.Config.lcd.tim);
		BACK_COLOR = SELECT_COLOR;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"ALL THE TIME",Sys.Config.lcd.tim);
		else
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"常亮",Sys.Config.lcd.tim);
	}
	else
	{
		Picture_FillRect(MenuPic,Rect[0],SELECT_COLOR);
		Picture_FillRect(MenuPic,Rect[1],OPTION_COLOR);
		BACK_COLOR = SELECT_COLOR;
		Pictrue_printf(&MenuPic,Rect[0].x0+10,Rect[0].y0+offset,16,"%dS",Sys.Config.lcd.tim);
		Pictrue_printf(&MenuPic,Rect[0].x0+10+24,Rect[0].y0+offset+4,12,"(10S-30S)",Sys.Config.lcd.tim);
		BACK_COLOR = OPTION_COLOR;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"ALL THE TIME",Sys.Config.lcd.tim);
		else
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"常亮",Sys.Config.lcd.tim);
	}		
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void LCD_TIM_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("BACKLITE TIM");
	else
		HeadDisplay("背光时间");
	LCD_TIM_display();
}

void LCD_TIM_Task(void)
{
	if(ec11_pos[2])
	{
		Sys.Config.lcd.tim -= (ec11_pos[2]*1);
		if(Sys.Config.lcd.tim>31)
		{
			Sys.Config.lcd.tim = 31;
		}
		else if(Sys.Config.lcd.tim<10)
		{
			Sys.Config.lcd.tim = 10;
		}
		LCD_TIM_display();
		ec11_pos[2] = 0;
	}	
	if(key_value==S10)
	{
		Menu_back();
	}	
}

void LCD_RotateTaskInit(void)
{
	Picture screen;
	screen.data = (u16*)(&gImage_filmgear_data[0]);
	screen.h = 128;
	screen.w = 160;

	if(Sys.Config.lcd.rotate)
		Sys.Config.lcd.rotate = 0;
	else
		Sys.Config.lcd.rotate = 1;
	LCD_Draw_Circle(10,8,5,YELLOW);	
	LCD_Set_Display_Mode(Sys.Config.lcd.rotate);
	LCD_Fill_Picture(0,0,screen);
	FreshMenu();
}

void LCD_RotateTask(void)
{
	SaveConfig();
	menu_key = Back_Key;
}

void EnglishTaskInit(void)
{
	Sys.Config.english = 1;
	SaveConfig();

}

void ChineseTaskInit(void)
{
	Sys.Config.english = 0;
	SaveConfig();
}
void ChineseTask(void)
{
	menu_key = Back_Key;
}

void firmware_update(void)
{
	u8 offset = (MENU_W%16)/2; // 水平中心对齐
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	
	if((HCD_IsDeviceConnected(&USB_OTG_Core))&&(Sys.usb.update_state==UPDATE_DETECT_FILE))
	{
		if(Sys.Config.english)
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  1.Press enter key to update firmware.\n  2.It will restart after updated.");
		}
		else
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  1.按确认键之后更新。\n  2.更新完成后自动重启。");
		}
	}
	else if(!HCD_IsDeviceConnected(&USB_OTG_Core))
	{
		if(Sys.Config.english)
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  Insert USB please.");
		}
		else
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  请插入USB。");
		}
	}
	else if(Sys.usb.update_state==UPDATE_NO_DETECT_FILE)
	{
		if(Sys.Config.english)
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  Not detect firmware.\n  Name of file must be %s.",FIRMWARE_NAME);
		}
		else
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  没有搜索到有效的文件。\n  文件名必须是%s。",FIRMWARE_NAME);
		}
	}
	else //正在搜
	{
		if(Sys.Config.english)
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  Searching for %s...",FIRMWARE_NAME);
		}
		else
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  正在搜索%s...",FIRMWARE_NAME);
		}
	}
	Sys.usb.USBH_USR_ApplicationState = USH_USR_FS_UPDATE;
	FreshMenu();
	if(key_value==S10)
	{
		Menu_back();
	}	
}

void firmware_update_init(void)
{
	if(Sys.Config.english)
		HeadDisplay("UPDATE"); 
	else
		HeadDisplay("固件更新"); 
}

void firmware_update_Enter(void)
{
	if(Sys.usb.update_state==UPDATE_DETECT_FILE)
	{
		Sys.Config.update = 1;
		SaveConfig();
		Sys_Soft_Reset();
	}
}

