#include "includes.h"
#include "stdio.h"
#include "stdlib.h"

u8 Set_display_option = 0;
u8 param_select = 0; //����Ҫ�޸ĵڼ�������
void point_out(void)
{
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,0,MENU_H-16,16,(char *)"press \"MENU\"");
	else
		Pictrue_printf(&MenuPic,0,MENU_H-16,16,(char *)"��ȷ�ϼ�ȷ��");
}


//����ִ�к���
//////////////////////////////////////////
void HSIDisplay(u8 pos)
{
	int i,offset;
	float dim;
	char buf[50];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<DIS_MAX_LINE;i++)
	{
		if(pos == i )
			Picture_FillRect(MenuPic,Rect[i],SELECT_COLOR);
		else
			Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	}
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
		sprintf(buf,"Hue:%d",Sys.Config.hsi.h);
	else
		sprintf(buf,"ɫ��:%d",Sys.Config.hsi.h);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,1);
	i = Sys.Config.hsi.s*100;
	if(Sys.Config.english)
		sprintf(buf,"Sat:%d%%",i);
	else
		sprintf(buf,"���Ͷ�:%d%%",i);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,1);
	dim = Sys.Config.hsi.i*100;
	if(Sys.Config.english)
		sprintf(buf,"Dim:%0.1f%%",dim);
	else
	{
		if(100.0>dim)
			sprintf(buf,"����:%0.1f%%",dim);
		else
			sprintf(buf,"����:100%%");
	}
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,1);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}
void HSIInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("HSI MODE");
	else
		HeadDisplay("HSIģʽ");
	while(DMAING);
	if(Set_display_option)
	{
		param_select = Set_display_option;
		Set_display_option = 0;
	}
	else
	{
		param_select = 0;
	}
	HSIDisplay(param_select);
	Sys.Config.lightmode = HSI_M;
	ColorLightHSIOut(Sys.Config.hsi,0);

}

u8 EC11_speed_cnt;
void HSITask(void)
{
	if(ec11_pos[2])
	{		
		switch(param_select)
		{
			case 0:
			{
				if(EC11_speed>45)
				{
					EC11_speed_cnt++;
					//if(EC11_speed_cnt>2)//����
					{
						Sys.Config.hsi.h -= ec11_pos[2]*0.05*EC11_speed;
						EC11_speed_cnt = 0;
					}
				}
				else
				{
					Sys.Config.hsi.h -= ec11_pos[2];
					EC11_speed_cnt = 0;
				}
				if(Sys.Config.hsi.h>359)
					Sys.Config.hsi.h = 0;
				if(Sys.Config.hsi.h<0)
					Sys.Config.hsi.h = 359;
				break;
			}	
			case 1:
			{
				if(EC11_speed>45)
				{
					EC11_speed_cnt++;
					//if(EC11_speed_cnt>2)
					{
						Sys.Config.hsi.s -= ec11_pos[2]*0.0005*EC11_speed;
						EC11_speed_cnt = 0;
					}
				}
				else
				{
					EC11_speed_cnt = 0;
					Sys.Config.hsi.s -= ec11_pos[2]*0.01;
				}
				if(Sys.Config.hsi.s>1)
					Sys.Config.hsi.s = 1;
				if(Sys.Config.hsi.s<0)
					Sys.Config.hsi.s = 0;
				break;
			}
			case 2:
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
				break;
			}
			default:break;
		}
		ec11_pos[2] = 0;
		HSIDisplay(param_select);//ˢ�½���
		ColorLightHSIOut(Sys.Config.hsi,0);
	}
	if(key_value==PSELECT_KEY)
	{
		param_select++;
		if(param_select>=3)
			Menu_back();
		HSIDisplay(param_select);//ˢ�½���
	}
}

////////////////////////////////////////////////////////////////////CCT
void CCTDisplay(u8 pos)
{
	int i,offset;
	char buf[15];
	offset = (RECT_H-16)/2;
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	for(i=0;i<DIS_MAX_LINE;i++)
	{
		if(pos == i )
			Picture_FillRect(MenuPic,Rect[i],SELECT_COLOR);
		else
			Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
	}
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Sys.Config.cct.kvn = cct_tab[Sys.Config.cct.pos].num;
	if(Sys.Config.english)
		sprintf(buf,"Kvn:%dK",Sys.Config.cct.kvn*100);
	else
		sprintf(buf,"ɫ��:%dK",Sys.Config.cct.kvn*100);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,1);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"Grn:%d",Sys.Config.cct.grn);
	else
		sprintf(buf,"��ƫ:%d",Sys.Config.cct.grn);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,1);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"Dim:%0.1f%%",Sys.Config.cct.dim*100);
	else
	{
		if(1.0>Sys.Config.cct.dim)
			sprintf(buf,"����:%0.1f%%",Sys.Config.cct.dim*100);
		else
			sprintf(buf,"����:100%%");
	}
	//	sprintf(buf,"����:%0.1f%%",Sys.Config.cct.dim*100);
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,1);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void CCTInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("CCT MODE");//CCT MODE
	else
		HeadDisplay("CCTģʽ");//CCT MODE
	LightCCTOut(Sys.Config.cct.pos,Sys.Config.cct.grn,Sys.Config.cct.dim,0);
	if(Set_display_option)
	{
		param_select = Set_display_option;
		Set_display_option = 0;
	}
	else
	{
		param_select = 0;
	}
	CCTDisplay(param_select);
	Sys.Config.lightmode = CCT_M;
}

void CCTTask(void)
{
//	if(pic.data == NULL)
//	{
//		return;
//	}
	if(ec11_pos[2])
	{
		switch(param_select)
		{
			case 0:
			{
				Sys.Config.cct.pos -= (ec11_pos[2]*1);
				if(Sys.Config.cct.pos>Sys.Config.cct.max_pos)
					Sys.Config.cct.pos = Sys.Config.cct.max_pos;
				if(Sys.Config.cct.pos<0)
					Sys.Config.cct.pos = 0;
				break;
			}
			case 1:
			{
				if(EC11_speed>30)
				{
					EC11_speed_cnt++;
				//	if(EC11_speed_cnt>2)
					{
						Sys.Config.cct.grn  -= ec11_pos[2]*0.05*EC11_speed;
						EC11_speed_cnt = 0;
					}
				}
				else
				{
					Sys.Config.cct.grn  -= ec11_pos[2];
					EC11_speed_cnt = 0;
				}
				if(Sys.Config.cct.grn>100)
				{
					Sys.Config.cct.grn = 100;
				}else if(Sys.Config.cct.grn<-100)
				{
					Sys.Config.cct.grn = -100;
				}

				break;
			}	
			case 2:
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
				break;
			}	
			default :break;
		}
		LightCCTOut(Sys.Config.cct.pos,Sys.Config.cct.grn,Sys.Config.cct.dim,0);
		CCTDisplay(param_select);
		ec11_pos[2] = 0;	
	}
	if(key_value==PSELECT_KEY)
	{
		param_select++;
		if(param_select>=3)
			Menu_back();
		CCTDisplay(param_select);
	}
}

//////////////////////////////////////////////////////GEL

void GELDisplay(u8 pos)
{
	int i;
	char buf[50];
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	POINT_COLOR = BLACK;
	RECT rect[3]={
	{0,0,MENU_W,20},
	{0,24,MENU_W,76},
	{0,80,MENU_W,100},
	};
	for(i=0;i<DIS_MAX_LINE;i++)
	{
		if(pos == i )
			Picture_FillRect(MenuPic,rect[i],SELECT_COLOR);
		else
			Picture_FillRect(MenuPic,rect[i],OPTION_COLOR);
	}
	if(Sys.Config.english)
	{
		if(Sys.Config.gel.source)
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],1,16,"SOURCE:DAY LIGHT");
		}
		else
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],1,16,"SOURCE:TUNGSTEN");
		}
		sprintf(buf,"GEL:%s",GEL_TAB[Sys.Config.gel.number].NAME[0]);
		Picture_ShowString(MenuPic,rect[1].x0+16,rect[1].y0+1,MENU_W,16,16,buf,1);
		if(pos == 1)
			BACK_COLOR = SELECT_COLOR;
		else
			BACK_COLOR = OPTION_COLOR;
		Pictrue_printf(&MenuPic,rect[1].x0+1,rect[1].y0+17,16,(char *)GEL_TAB[Sys.Config.gel.number].NAME[1]);
		sprintf(buf,"DIM:%0.1f%%",Sys.Config.gel.dim*100);
		Picture_ShowStringInRectCenter(MenuPic,rect[2],1,16,buf);
	}
	else
	{
		if(Sys.Config.gel.source)
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],1,16,"��Դ:�չ��");
		}
		else
		{
			Picture_ShowStringInRectCenter(MenuPic,rect[0],1,16,"��Դ:��˿��");
		}
		sprintf(buf,"GEL:%s",GEL_TAB[Sys.Config.gel.number].NAME[0]);
		Picture_ShowString(MenuPic,rect[1].x0+16,rect[1].y0+1,MENU_W,16,16,buf,1);
		if(pos == 1)
			BACK_COLOR = SELECT_COLOR;
		else
			BACK_COLOR = OPTION_COLOR;
		Pictrue_printf(&MenuPic,rect[1].x0+1,rect[1].y0+17,16,(char *)GEL_TAB[Sys.Config.gel.number].NAME[1]);
		sprintf(buf,"����:%0.1f%%",Sys.Config.gel.dim*100);
		Picture_ShowStringInRectCenter(MenuPic,rect[2],1,16,buf);
	}
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void GELInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("GEL MODE");
	else
		HeadDisplay("GELģʽ");
	LightGELOut(&Sys.Config.gel,0);
	if(Set_display_option)
	{
		param_select = Set_display_option;
		Set_display_option = 0;
	}
	else
	{
		param_select = 0;
	}	
	GELDisplay(param_select);
	Sys.Config.lightmode = GEL_M;
}

void GELTask(void)
{
	int temp;
	if(ec11_pos[2])
	{
		switch(param_select)
		{
			case 0:			
			{
				if(Sys.Config.gel.source)
				{
					Sys.Config.gel.source = 0;
				}
				else
				{
					Sys.Config.gel.source = 1;
				}
				break;
			}	
			case 2:
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
				break;
			}
			case 1:
			{
				temp = Sys.Config.gel.number+0;
				temp -= (int)ec11_pos[2]*1;
				if(temp > (Sys.max_gel_number-1))
				{
					temp = Sys.max_gel_number-1;
				}
				else if(temp<0)
				{
					temp = 0;
				}
				Sys.Config.gel.number  = (unsigned char)temp;
				break;				
			}
		}
		LightGELOut(&Sys.Config.gel,0);
		GELDisplay(param_select);
		ec11_pos[2] = 0;
	}
	if(key_value==PSELECT_KEY)
	{
		param_select++;
		if(param_select>=3)
			Menu_back();
		GELDisplay(param_select);
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
		sprintf(buf,"��:%d",Sys.Config.rgb.r);
	Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"G:%d",Sys.Config.rgb.g);
	else
		sprintf(buf,"��:%d",Sys.Config.rgb.g);
	Picture_ShowString(MenuPic,Rect[1].x0+STRING_OFFSET,Rect[1].y0+offset,100,16,16,buf,0);
	memset(buf,0,15);
	if(Sys.Config.english)
		sprintf(buf,"B:%d",Sys.Config.rgb.b);
	else
		sprintf(buf,"��:%d",Sys.Config.rgb.b);
	Picture_ShowString(MenuPic,Rect[2].x0+STRING_OFFSET,Rect[2].y0+offset,100,16,16,buf,0);
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void RGBInitTask(void)
{
	if(Sys.Config.english)
		HeadDisplay("RGB MODE");
	else
		HeadDisplay("RGBģʽ");
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
			}else if(Sys.Config.rgb.g<0.0)
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
			}else if(Sys.Config.rgb.b<0.0)
			{
				Sys.Config.rgb.b = 0;
			}
			ec11_pos[0] = 0;
		}	
		LightRGBOut(Sys.Config.rgb.r,Sys.Config.rgb.g,Sys.Config.rgb.b,0xff);
		RGBDisplay();
	}
	if(key_value==PSELECT_KEY)
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
//��˸����
float FlashDim = 0.5;
u16 FlashTimOn = 500;
u16 FlashTimOff = 500;
u8 flash_flg = 0;

void SceneBack(void)
{
	SceneTimDisable();
//	myfree(SRAMIN,pscene);
	pscene = 0;
}

void LoadFlashData(void)
{
	int i,j;
	u16 current[LED_CHS];

//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	pscene = &scendatabuf;
	if(pscene != NULL)
	{
		pscene->max_frames = 1;
		pscene->mode = 0;//����ѭ��
		MixLightKToCurrent((float *)&rgbk,FlashDim,current);
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
}

void SceneFlashInit(void)
{
	LoadFlashData();
	//SceneBegain(pscene);
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 1;
	Sys.menu_mask = 1;
	flash_flg = 0;
	param_select = 0;
}

void SceneFlashTask(void)//50 ms
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	
	if(ec11_pos[2])
	{
		switch(param_select)
		{
			case 0:
			{
				if(EC11_speed>25)
				{
					FlashTimOn -= ec11_pos[2]*0.5*EC11_speed;
				}
				else
				{

					FlashTimOn -= ec11_pos[2];
				}
				if(FlashTimOn<FRAME_MIN_T)
				{
					FlashTimOn = FRAME_MIN_T;
				}
				break;
			}
			case 1:
			{
				if(EC11_speed>25)
				{
					FlashTimOff -= ec11_pos[2]*0.5*EC11_speed;
				}
				else
				{

					FlashTimOff -= ec11_pos[2];
				}
				if(FlashTimOff<FRAME_MIN_T)
				{
					FlashTimOff = FRAME_MIN_T;
				}			
				break;
			}
			case 2:
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
				break;
			}
			default :break;	
		}
		LoadFlashData();
		ec11_pos[2] = 0;
	}
	Pictrue_printf(&MenuPic,0,0,16,	"  Flash");
	if(param_select == 0)
			BACK_COLOR = SELECT_COLOR;
	else
			BACK_COLOR = OPTION_COLOR;
	Pictrue_printf(&MenuPic,0,16,16,"  ON:%dms         ",FlashTimOn);
	if(param_select == 1)
			BACK_COLOR = SELECT_COLOR;
	else
			BACK_COLOR = OPTION_COLOR;
	Pictrue_printf(&MenuPic,0,32,16,"  OFF:%dms        ",FlashTimOff);
	if(param_select == 2)
			BACK_COLOR = SELECT_COLOR;
	else
			BACK_COLOR = OPTION_COLOR;
	Pictrue_printf(&MenuPic,0,48,16,"  Dim:%0.1f%%      ",FlashDim*100);
	FreshMenu();
	
  if(key_value==PSELECT_KEY)
	{
		if(param_select<3)
			param_select++;
		else
		{
			if(flash_flg==0)
			{
				flash_flg = 1;
				SceneBegain(pscene);
			}
			else
			{
				flash_flg = 0;
				SceneTimDisable();
			}
		}
	}
}

int SOS_speed = 5;
// ����˸�����100ms������1000ms������˸�Ƕ̵�����ʱ��
void LoadSOSData(void)
{
	int i,j,k;
//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	u16 current[LED_CHS];
	pscene = &scendatabuf;
	if(pscene != NULL)
	{
		pscene->max_frames = 18;
		pscene->mode = 0;//����ѭ��
		MixLightKToCurrent((float *)&rgbk,FlashDim,current);
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
				pscene->frame[j].tim = 100*SOS_speed;//��
			for(j=6;j<12;j++)
				pscene->frame[j].tim = 200*SOS_speed;//��
			for(j=12;j<18;j++)
				pscene->frame[j].tim = 100*SOS_speed;
			
			for(j=0;j<LED_NUMS;j++)
				pscene->frame[18].ch[i][j] = 0;
			pscene->frame[18].tim = 200*SOS_speed;
		}
	}
	else
	{
		Debug_printf(">>SceneFlashInit:mymalloc()fail\r\n");
	}
}

//SOS ������������
void SceneSOSInit(void)
{
	LoadSOSData();
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 2;
	Sys.menu_mask = 1;
	flash_flg = 0;
	param_select = 0;
}


void SceneSOSTask(void)//50 ms
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(ec11_pos[2])
	{
		switch(param_select)
		{
			case 0:
			{
				SOS_speed -= ec11_pos[2];
				if(SOS_speed<1)
				{
					SOS_speed = 1;
				}
				if(SOS_speed>10)
					SOS_speed = 10;
				break;
			}
			case 1:
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
				break;
			}
			default :break;
		}
		ec11_pos[2] = 0;
		LoadSOSData();
	}
	Pictrue_printf(&MenuPic,0,0,16,	"  SOS");
	if(param_select == 0)
			BACK_COLOR = SELECT_COLOR;
	else
			BACK_COLOR = OPTION_COLOR;
	Pictrue_printf(&MenuPic,0,16,16,"  Speed:%d        ",SOS_speed);
	if(param_select == 1)
			BACK_COLOR = SELECT_COLOR;
	else
			BACK_COLOR = OPTION_COLOR;
	Pictrue_printf(&MenuPic,0,32,16,"  Dim:%0.1f%%     ",FlashDim*100);
	FreshMenu();
	
  if(key_value==PSELECT_KEY)
	{
		if(param_select<2)
			param_select++;
		else
		{
			if(flash_flg==0)
			{
				flash_flg = 1;
				SceneBegain(pscene);
			}
			else
			{
				flash_flg = 0;
				SceneTimDisable();
			}
		}
	}

}

const u8 stom_table[8][11] = 
{
{0,38,0,20,40 ,0,38,0,20,40, 30},
{0,0,0,0,0,0,0,0,0,0,80},
{0,38,0,20,40 ,0,38,0,20,40, 20},
{0,0,0,0,0,0,0,0,0,0,80},
{0,38,0,20,40 ,0,38,0,20,40, 20},
{0,0,0,0,0,0,0,0,0,0,80},
{0,0,0,0,0,0,0,0,0,0,80},
{0,38,0,20,40 ,0,38,0,20,40, 20},
};

void SceneStormInit(void)
{
	int i,j,k;
//	if(pscene == NULL)
//		pscene = mymalloc(SRAMIN,sizeof(SceneData)); //
	pscene = &scendatabuf;
	pscene->max_frames = 8;
	pscene->mode = 255;//������Ϩ��
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<5;j++)
		{
			pscene->frame[i].ch[0][j] = 65535/stom_table[i][j];
			pscene->frame[i].ch[1][j] = 65535/stom_table[i][5+j];
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
	if(key_value == PSELECT_KEY)
	{
		SceneBegain(pscene);
	}
}


void InfoTaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("INFO");
	else
		HeadDisplay("�豸��Ϣ");
}	


void InfoTask(void)
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Pictrue_printf(&MenuPic,0,0,16,(char *)"Version:%s",VERSION);
	Pictrue_printf(&MenuPic,0,16,16,(char *)"DMX protocol:%s",DXM_PRO);
	Pictrue_printf(&MenuPic,0,32,16,(char *)"Primary colours:%s",PrimaryColours);
	FreshMenu();
}	

void MonitorInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("MONITOR");
	else
		HeadDisplay("���");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Pictrue_printf(&MenuPic,16,0,16,(char *)"PANEL T:%0.1f",Sys.panle_t);
	FreshMenu();
}	

void MonitorTask(void)
{
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
	Pictrue_printf(&MenuPic,16,0,16,(char *)"PANEL T:%0.1f",Sys.panle_t);
	FreshMenu();
}		

void DefaultInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("DEFAULT");
	else
		HeadDisplay("��������");
	ClearMenu(OPTION_COLOR);
	FreshMenu();
}	

void DefaultTask(void)
{
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	Pictrue_printf(&MenuPic,16,0,16,(char *)"Are you sure to restore default parameters?");
	if(key_value == PSELECT_KEY)
	{
		Sys.Config = default_data;
	}
	point_out();
	FreshMenu();
}

u8 Art_net_en=0;
void DMXINTaskInit(void)
{
	u8 offset = (MENU_W%16)/2; // ˮƽ���Ķ���
	Art_net_en = 0;
	if(Sys.Config.english)
	{
		HeadDisplay("DMX IN");
	}
	else
	{
		
		HeadDisplay("DMX����");
	}
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,offset,0,16,"  Select DMX IN.");
	else
		Pictrue_printf(&MenuPic,offset,0,16,"  ѡ��DMX���롣");
	point_out();
	FreshMenu();
}

void AutoDetectINInit(void)
{
	u8 offset = (MENU_W%16)/2; // ˮƽ���Ķ���
	if(Sys.Config.english)
		HeadDisplay("AUTO DETECT");
	else
		HeadDisplay("�Զ����");
	ClearMenu(OPTION_COLOR);
	/*
	ִ��������
	*/
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	if(0)
	{
		Art_net_en = 1;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,offset,offset,16,"  Detect the Ethernet,Interface is Art-Net.");
		else
			Pictrue_printf(&MenuPic,offset,offset,16,"  ��⵽��̫������,ʹ��Art-Net�ӿڡ�");

	}
	else
	{
		Art_net_en = 0;
		if(Sys.Config.english)
			Pictrue_printf(&MenuPic,offset,0,16,"  Interface is DMX.");
		else
			Pictrue_printf(&MenuPic,offset,offset,16,"  ʹ��DMX�ӿڡ�");
	}
	point_out();
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void Art_NetINTask(void)
{
	u8 offset = (MENU_W%16)/2; // ˮƽ���Ķ���
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
	ִ��������
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
			Pictrue_printf(&MenuPic,offset,0,16,"  û�м�⵽RJ45���ӣ�Ĭ��ʹ��DMX��");
	}
	FreshMenu();
  if(key_value==PSELECT_KEY)
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
		HeadDisplay("DMXģʽ");
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
			case DMX_M1:DMX_ModeDisplay("M1:CCT&RALGB","8λ",sizeof(CCT_LEDK_8BIT));break;
			case DMX_M2:DMX_ModeDisplay("M2:CCT","8λ",sizeof(CCT_8BIT));break;		
			case DMX_M3:DMX_ModeDisplay("M3:CCT&HSI","8λ",sizeof(CCT_HSI_8BIT));break;
			case DMX_M4:DMX_ModeDisplay("M4:RALGB","8λ",sizeof(LEDK_8BIT));break;
			case DMX_M5:DMX_ModeDisplay("M5:HSI","8λ",sizeof(HSI_8BIT));break;
			case DMX_M6:DMX_ModeDisplay("M6:CCT&RALGB","16λ",sizeof(CCT_LEDK_16BIT));break;
			case DMX_M7:DMX_ModeDisplay("M7:CCT","16λ",sizeof(CCT_16BIT));break;
			case DMX_M8:DMX_ModeDisplay("M8:CCT&HSI","16λ",sizeof(CCT_HSI_16BIT));break;
			case DMX_M9:DMX_ModeDisplay("M9:RALGB","16Bλ",sizeof(LEDK_16BIT));break;
			case DMX_M10:DMX_ModeDisplay("M10:HSI","16λ",sizeof(HSI_16BIT));break;		
			case DMX_M11:DMX_ModeDisplay("M11:CCT&RALGB","�ֵ�/ϸ��",sizeof(CCT_LEDK_CF));break;
			case DMX_M12:DMX_ModeDisplay("M12:CCT","�ֵ�/ϸ��",sizeof(CCT_CF));break;
			case DMX_M13:DMX_ModeDisplay("M13:CCT&HSI","�ֵ�/ϸ��",sizeof(CCT_HSI_CF));break;
			case DMX_M14:DMX_ModeDisplay("M14:RALGB","�ֵ�/ϸ��",sizeof(LEDK_CF));break;
			case DMX_M15:DMX_ModeDisplay("M15:HSI","�ֵ�/ϸ��",sizeof(HSI_CF));break;
			default:break;
		}
	if(key_value==PSELECT_KEY)
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
		sprintf(buf,"��ַ:%d",dmx_addr);
		Picture_ShowString(MenuPic,Rect[0].x0+STRING_OFFSET,Rect[0].y0+offset,100,16,16,buf,0);
		sprintf(buf,"ͨ��:%d-%d",dmx_addr,dmx_addr+ch-1);
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
		HeadDisplay("����DMX��ַ");
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
	if(key_value==PSELECT_KEY)
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
		HeadDisplay("�����Զ�����");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_AUTO;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"Set FAN AUTO");
	else
		Pictrue_printf(&MenuPic,16,0,16,"�����Զ�����");
	//point_out();
	FreshMenu();
}

void FAN_OFF_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("FAN OFF");
	else
		HeadDisplay("���ȹر�");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_OFF;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"FAN OFF");
	else
		Pictrue_printf(&MenuPic,16,0,16,"�رշ���");
	//point_out();
	FreshMenu();
}

void FAN_DMX_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("FAN DMX");
	else
		HeadDisplay("DMX ����");
	ClearMenu(OPTION_COLOR);
	BACK_COLOR = OPTION_COLOR;
	POINT_COLOR = BLACK;
	fan = FAN_DMX;
	if(Sys.Config.english)
		Pictrue_printf(&MenuPic,16,0,16,"FAN is control by DMX.");
	else
		Pictrue_printf(&MenuPic,16,0,16,"����ͨ��DMX���ơ�");
//	point_out();
	FreshMenu();
	
	if(key_value==PSELECT_KEY)
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
	int i,offset;
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
		HeadDisplay("����");
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
	if(key_value==PSELECT_KEY)
	{
		Menu_back();
	}	
}

void LCD_TIM_display(void)
{
	int i,offset;
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
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"����",Sys.Config.lcd.tim);
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
			Pictrue_printf(&MenuPic,Rect[1].x0+3,Rect[1].y0+offset,16,"����",Sys.Config.lcd.tim);
	}		
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void LCD_TIM_TaskInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("BACKLITE TIM");
	else
		HeadDisplay("����ʱ��");
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
	if(key_value==PSELECT_KEY)
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
	u8 offset = (MENU_W%16)/2; // ˮƽ���Ķ���
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
			Pictrue_printf(&MenuPic,offset,0,16,"  1.��ȷ�ϼ�֮����¡�\n  2.������ɺ��Զ�������");
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
			Pictrue_printf(&MenuPic,offset,0,16,"  �����USB��");
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
			Pictrue_printf(&MenuPic,offset,0,16,"  û����������Ч���ļ���\n  �ļ���������%s��",FIRMWARE_NAME);
		}
	}
	else //������
	{
		if(Sys.Config.english)
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  Searching for %s...",FIRMWARE_NAME);
		}
		else
		{
			Pictrue_printf(&MenuPic,offset,0,16,"  ��������%s...",FIRMWARE_NAME);
		}
	}
	Sys.usb.USBH_USR_ApplicationState = USH_USR_FS_UPDATE;
	FreshMenu();
	if(key_value==PSELECT_KEY)
	{
		Menu_back();
	}	
}

void firmware_update_init(void)
{
	if(Sys.Config.english)
		HeadDisplay("UPDATE"); 
	else
		HeadDisplay("�̼�����"); 
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

