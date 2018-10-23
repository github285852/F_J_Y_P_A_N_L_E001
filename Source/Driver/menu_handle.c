#include "includes.h"



int menu_pos = 0, pre_pos = 0;
extern  u8 text_pos;
struct MenuTyp *current_menu=NULL;
struct MenuTyp *pre_menu = NULL;
void (*MenuDuty)(void);
unsigned char menu_key;
MenuState menu_state;
Picture MenuPic;
RECT Rect[DIS_MAX_LINE];
u16 MENU_BUF[MENU_W*MENU_H];
//u16 *MENU_BUF = NULL;
void MenuBuf_malloc(void)
{
//		MENU_BUF = mymalloc(SRAMIN,MENU_W*MENU_H*2);
}
void MenuBuf_free(void)
{
//	if(MENU_BUF != NULL)
//	{
//		myfree(SRAMIN,MENU_BUF);
//		MENU_BUF = NULL;
//	}
}
void MenuKeyTask()
{
//�ж�Ϊ�˵�����
	switch(menu_key)
	{
		//ͬ���˵�֮����л�
		case Up_Key: //up
		{
			if(current_menu->pLeft != NULL)
			{
				current_menu = current_menu->pLeft;
				menu_pos--;
				if(menu_pos<0)
				{
					menu_pos = 0;
				}
			}
		}
		break;
		case Down_Key://down
		{
			if(current_menu->pRight != NULL)
			{
				current_menu = current_menu->pRight;
				menu_pos++;
				if(menu_pos>DIS_MAX_LINE-1)
				{
					menu_pos = DIS_MAX_LINE-1;
				}
			}
		}
		break;
		//��ͬ���˵�֮����л�
		case Back_Key://back
		{
			if(current_menu != NULL)
			{
				if(current_menu->pParent != NULL)
				{
					current_menu = current_menu->pParent;
					menu_pos = current_menu->pos;
				}
			}
		}
		break;
		case Enter_Key://enter
		{	
			if(current_menu != NULL)
			{
				if(current_menu->pChild != NULL)
				{
					current_menu->pos = menu_pos;
					current_menu = current_menu->pChild;
					if((current_menu->pMenuTaskInit != NULL)&&(current_menu->pChild == NULL))
					{
						current_menu->pMenuTaskInit();
					}
					//pre_pos = menu_pos;
					menu_pos = 0;
				}
				else
				{
					if(current_menu->pTaskKeyEnter != NULL)
					{
						current_menu->pTaskKeyEnter();
					}
					current_menu = current_menu->pParent;//��ʾȷ��
				}
			}
			else
			{
				current_menu = InitMenup; //ע���ǵ�һ���˵���һ��
			}
		}
		break;
		default:break;
	}
	menu_key = 0;
	if(current_menu != NULL)
	{
		
		if(current_menu->pChild == NULL)//Ϊ��ͼ�Ŀ¼
		{
			if(current_menu->pMneuTask != NULL)
			{
				current_menu->pMneuTask();//�ڽ��洦��״̬�£�ÿ�������룬�ͻᴦ��
			}
			menu_state = HandleGui;
		}
		else
		{
			MenuSelectDisplay();
			menu_state = SelectMenu;
		}
	}
	else
	{
		menu_state = HandleGui;
		menu_pos = 0;
	}
}

void Menu_init(void)
{
	int i;
	CreateMenu();
	MenuPic.h = MENU_H;
	MenuPic.w = MENU_W;
	
	//MenuPic.data = MENU_BUF;
	for(i=0;i<DIS_MAX_LINE;i++)
	{
		Rect[i].x0 = FIS_RECT_POS_W;
		Rect[i].x1 = Rect[i].x0 + RECT_W;
//		Rect[i].y0 = FIS_RECT_POS_H + i*RECT_H + i*4;
//		Rect[i].y1 = FIS_RECT_POS_H + (i+1)*RECT_H + i*4;
		Rect[i].y0 = FIS_RECT_POS_H + i*RECT_H + i*RECT_INTERVAL;
		Rect[i].y1 = RECT_H + Rect[i].y0;
	}
}

//void MenuSelectDisplay1(void)
//{
//	struct MenuTyp *pMenu = current_menu;
//	int i;
//	//pic.data =(u16*) malloc(MENU_W*MENU_H*2);
//	//�����˵�
//	if(MenuPic.data == NULL)
//	{
//		return;
//	}
//	Picture_Fill(MenuPic,YELLOW);

//	Picture_FillRect(MenuPic,Rect[menu_pos],BLUE);
//	
//	//�ڲ˵�ָ���������ʾ���
//	POINT_COLOR = BLACK;
//	BACK_COLOR = BLUE;
//	//Picture_ShowString(pic,0,5 + 16*menu_pos,MENU_W,20,16,pMenu->Name);
//	Picture_ShowStringInRectCenter(MenuPic,Rect[menu_pos],1,16,(char *)pMenu->Name);
//	BACK_COLOR = YELLOW;
//	
//	//��ʾ�������
//	for(i=menu_pos-1;i>=0;i--)
//	{
//		pMenu = pMenu->pLeft;
//		if(pMenu != NULL)
//		{
//		//void Picture_ShowString(Picture pic,u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
//		//	Picture_ShowString(pic,0,5 + 16*i,MENU_W,20,16,pMenu->Name);
//				Picture_ShowStringInRectCenter(MenuPic,Rect[i],1,16,(char *)pMenu->Name);
//		}
//	}
//	pMenu = current_menu;
//	for(i=menu_pos+1;i<5;i++)
//	{
//		pMenu = pMenu->pRight;
//		if(pMenu != NULL)
//		{
//			//Picture_ShowString(pic,0,5 + 16*i,MENU_W,20,16,pMenu->Name);
//			Picture_ShowStringInRectCenter(MenuPic,Rect[i],1,16,(char *)pMenu->Name);
//		}
//	}
//	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
//}


void HeadDisplay(char *p)
{
	Picture temp_pic;
	int x,y,i;
	u16 *pdata=(u16*)(&gImage_filmgear_data[0]);
	int pos;
	
	temp_pic.h = 16;
  temp_pic.w = 120;
	//�����ڴ�;
	temp_pic.data = MENU_BUF;
	//��һ����ͼƬ�������
	for(y=0;y<temp_pic.h;y++)
	{
		for(x=0;x<temp_pic.w;x++)
		{
			pos = LCD_W*y+x+20;//20����ʾ������
			temp_pic.data[i++] = pdata[pos]; 
		}
	}
	//Picture_Fill(temp_pic,MENU_BACK_COLOR);
	BACK_COLOR = BLACK;
	POINT_COLOR = YELLOW;
	
	Picture_ShowString(temp_pic,0,0,100,16,16,p,1);
	LCD_Fill_Picture(20,0,temp_pic);
	while(DMAING);
}
	
void MenuSelectDisplay(void)
{
	struct MenuTyp *pMenu = current_menu;
	int i;
	char num_tab[4]={0,0,0,0};
	char *pName;
	BMP bmp;
	//��ʾ�˵�
		//LCD_Draw_Circle(10,8,5,YELLOW);	
//	bmp.w = 16;
//	bmp.h = 16;
//	bmp.data = (unsigned char *)angle_data16X16;
//	LCD_DrawBMP(2,2,&bmp,YELLOW,MENU_BACK_COLOR);
	
	if(Sys.Config.english)
	{
		if(current_menu->pParent == NULL)
		{
			HeadDisplay("MENU");
		}
		else
		{
			HeadDisplay((char *)current_menu->pParent->EnName);//
		}
		pName  = (char *)pMenu->EnName;
	}
	else
	{
		if(current_menu->pParent == NULL)
		{
			HeadDisplay("�˵�");
		}
		else
		{
			HeadDisplay((char *)current_menu->pParent->ChName);//
		}
		pName  = (char *)pMenu->ChName;
	}
	while(DMAING);//�ȴ���һ֡�����꣬��Ϊ����һ��buf
	/////////////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	Picture_Fill(MenuPic,MENU_BACK_COLOR);
	
	POINT_COLOR = BLACK;
	Picture_FillRect(MenuPic,Rect[menu_pos],SELECT_COLOR);//ѡ��ѡ���Ի�ɫ��ʾ
	//Picture_ShowString(pic,0,5 + 16*menu_pos,MENU_W,20,16,pMenu->Name);
	Picture_ShowStringInRectCenter(MenuPic,Rect[menu_pos],1,16,pName);
	//��ʾѡ����
	sprintf(num_tab,"%d.",Menu_GetoptionNum(pMenu));
	BACK_COLOR = BLACK;
	POINT_COLOR = LGRAY;
	Picture_ShowString(MenuPic,NUM_OFFSET_X,Rect[menu_pos].y0+NUM_OFFSET_Y,40,16,16,num_tab,0);
	//��ʾ�������
	for(i=menu_pos-1;i>=0;i--)
	{
		pMenu = pMenu->pLeft;
		if(pMenu != NULL)
		{
		//void Picture_ShowString(Picture pic,u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
		//	Picture_ShowString(pic,0,5 + 16*i,MENU_W,20,16,pMenu->Name);
			POINT_COLOR = BLACK;
			Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
			if(Sys.Config.english)
			{
				pName  = (char *)pMenu->EnName;
			}
			else
			{
				pName  = (char *)pMenu->ChName;
			}
			Picture_ShowStringInRectCenter(MenuPic,Rect[i],1,16,pName);
			memset(num_tab,0,4);
			sprintf(num_tab,"%d.",Menu_GetoptionNum(pMenu));
			BACK_COLOR = BLACK;
			POINT_COLOR = LGRAY;
			Picture_ShowString(MenuPic,NUM_OFFSET_X,Rect[i].y0+NUM_OFFSET_Y,40,16,16,num_tab,0);
		}
	}
	if((pMenu != NULL)&&(pMenu->pLeft != NULL))//��ʾ���滹��ѡ��
	{
		//Picture_DrawTrigon(MenuPic,MENU_W/2-10,0,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)trigon_data_16X8;
		Picture_DrawBMP(MenuPic,MENU_W/2-10,0,&bmp,LGRAY,MENU_BACK_COLOR);
		
	}	
	pMenu = current_menu;
	for(i=menu_pos+1;i<DIS_MAX_LINE	;i++)
	{
		pMenu = pMenu->pRight;
		if(pMenu != NULL)
		{
			//Picture_ShowString(pic,0,5 + 16*i,MENU_W,20,16,pMenu->Name);
			POINT_COLOR = BLACK;
			Picture_FillRect(MenuPic,Rect[i],OPTION_COLOR);
//		bmp.w = 104;
//		bmp.h = 24;
//		bmp.data = (unsigned char *)angle_rect_data104X24;
//		Picture_DrawBMP(MenuPic,Rect[i].x0,Rect[i].y0,&bmp,LGRAY,MENU_BACK_COLOR);
			if(Sys.Config.english)
			{
				pName  = (char *)pMenu->EnName;
			}
			else
			{
				pName  = (char *)pMenu->ChName;
			}
			Picture_ShowStringInRectCenter(MenuPic,Rect[i],1,16,pName);
			//��ʾ���
			memset(num_tab,0,4);
			sprintf(num_tab,"%d.",Menu_GetoptionNum(pMenu));
			BACK_COLOR = BLACK;
			POINT_COLOR = LGRAY;
			Picture_ShowString(MenuPic,NUM_OFFSET_X,Rect[i].y0+NUM_OFFSET_Y,40,16,16,num_tab,0);
		}
	}
	if((pMenu != NULL)&&(pMenu->pRight != NULL))//��ʾxia�滹��ѡ��
	{
		//Picture_DrawInvertedTrigon(MenuPic,MENU_W/2-10,Rect[DIS_MAX_LINE-1].y1+RECT_INTERVAL,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)inverted_trigon_data_16X8;
		Picture_DrawBMP(MenuPic,MENU_W/2-10,Rect[DIS_MAX_LINE-1].y1+RECT_INTERVAL,&bmp,LGRAY,MENU_BACK_COLOR);
	}	
	
	LCD_Fill_Picture(MENU_POS_X,MENU_POS_Y,MenuPic);
}

void MenuTask(void)
{
	MenuKeyTask();//�˵�ִ��,ѭ������
}

