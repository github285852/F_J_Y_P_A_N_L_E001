#include "includes.h"



void timer4_init(u16 period,u16 Prescaler);

void GUI_DisplayInit(void)
{
	LCD_Draw_Circle(10,8,5,YELLOW);	
	switch(Sys.Config.lightmode)
	{
		case CCT_M:
		{
			CCTInitTask();
			break;
		}
		case HSI_M:
		{
			HSIInitTask();
			break;
		}
		case RGB_M:RGBInitTask();break;
		case SCENE_M:
		{
			switch(Sys.Config.scene)
			{
				case 1:Scene1TaskInit();break;
				case 2:Scene2TaskInit();break;
				case 3:Scene3TaskInit();break;
				case 0xff:ImportSceneTaskInit();break;
				default:break;
				
			}
		}
		default :break;
	}
}

void GUI_DisplayTask(void)
{
	if(current_menu == NULL)
	{
		switch(Sys.Config.lightmode)
		{
			case CCT_M:
			{
				CCTTask();
				break;
			}
			case HSI_M:
			{
				HSITask();
				break;
			}
			case RGB_M:RGBTask();break;
			case SCENE_M:
			{
				switch(Sys.Config.scene)
				{
					case 1:Scene1Task();break;
					//case 2:Scene2Task();break;
					//case 3:Scene3Task();break;
					//case 0xff:ImportSceneTask();break;
					default:break;
				}
			}
			break;
			case DMX_M:
			{
				DMX_ModeTask();	
			}
			break;
			default :break;
		}
	}
}

void InputTask(void)
{
	static CONFIG last_config;
	if(ec11_check || key_status || Sys.dmx_insert)//�ⲿ���봦��
	{
		if(Sys.dmx_connect)
			goto do_only_dmx;
		if((SelectMenu == menu_state)||(S9==key_value)) // ����ӳ��Ϊ�˵�����
		{
			if(ec11_pos[2]>0)
			{
				menu_key = Down_Key;
			}
			else if(ec11_pos[2]<0)
			{
				menu_key = Up_Key;
			}
			switch(key_value)
			{
				default :break;
			}
			if(current_menu != NULL)
				ec11_pos[2] = 0;
		}
		
		switch(key_value)
		{
			case S5: Sys.Config.lightmode = CCT_M; Sys.Config.cct.pos = 2; current_menu = NULL; GUI_DisplayInit(); break;// CCT MODE 2700K
			case S4: Sys.Config.lightmode = CCT_M; Sys.Config.cct.pos = 7; current_menu = NULL; GUI_DisplayInit(); break;// CCT MODE 3200K
			case S3: Sys.Config.lightmode = CCT_M; Sys.Config.cct.pos = 20; current_menu = NULL; GUI_DisplayInit(); break;// 4500K
			case S2: Sys.Config.lightmode = CCT_M; Sys.Config.cct.pos = 55-25; current_menu = NULL; GUI_DisplayInit(); break; //5500K	
		  case S1: Sys.Config.lightmode = CCT_M; Sys.Config.cct.pos = 60-25; current_menu = NULL; GUI_DisplayInit(); break; //6000K
			case S7: Sys.Config.lightmode++; if(Sys.Config.lightmode >1 ) Sys.Config.lightmode = 0; current_menu = NULL; GUI_DisplayInit();break;  //CCT_M - HSI_M - CCT_M
			case S9: menu_key = Back_Key;break;
			case S10: menu_key = Enter_Key;break;

		  default :break;
		}
		do_only_dmx:
		Sys.lcd_back_on = 1;
		Sys.save_cnt = 20*Sys.Config.lcd.tim;//10s

		MenuKeyTask();//�˵�ִ��,ѭ������	
		GUI_DisplayTask();
		
		key_status = 0;
		key_value = 0;
		ec11_check = 0;
		Sys.dmx_insert = 0;
	}
	else
	{
		MenuKeyTask();//�˵�ִ��,ѭ������	
		GUI_DisplayTask();
	}
	//��������
	if(Sys.save_cnt>0)
	{
		if(Sys.save_cnt == 1)
		{
			Sys.lcd_back_on = 0;
			if( memcmp(&last_config,&Sys.Config,sizeof(CONFIG)) )
			{
				SaveConfig();
				last_config = Sys.Config;
				current_menu = NULL;
				menu_state = HandleGui;
				GUI_DisplayInit();
			}	
		}
		Sys.save_cnt--;
	}
}


#define LCD_BACK_T	100
void LcdBackTask(void)
{
	static int tim;
	int light_dim = Sys.Config.lcd.dim*LCD_BACK_T/100;
	if(Sys.lcd_back_on)
	{
//		tim++;
//		if(tim>LCD_BACK_T)
//			tim = 0;
//		if(tim<light_dim)
			LCD_BACK_ON;
//		else
//			LCD_BACK_OFF;
	}
	else
		LCD_BACK_OFF;
}

