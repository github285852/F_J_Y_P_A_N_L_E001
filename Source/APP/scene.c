#include "includes.h"


typedef struct
{
	char name[EXFile_MAX_NUM+1][EXFile_MAX_LEN];
	int option;//�ڼ�ѡ��
	int line;//ѡ���ڵڼ�����ʾ
	int max_nums;//��ʾ��������
	int max_line;//��ʾ�������
	RECT *rect;//��ʾ�ľ�����ָ��
}SELECTBAR;

void SceneBegain(SceneData *pscene);

SceneData scendatabuf;
SELECTBAR import_scene_bar;

SceneData *scene;

int CharToInt(char hex) 
{
    if (hex>='0' && hex <='9')
        return hex - '0';
    if (hex>='A' && hex <= 'F')
        return hex-'A'+10;
    if(hex>='a' && hex <= 'f')
        return hex-'a'+10;
    return -1;
}


/*
FOUNCTION:��16���Ƶ��ַ���ת������ֵ
INPUT:hex,Ҫת����ֵ��16�����ַ���
OUTPUT:value, point of value
RETURN:0,OK,-1,�зǷ��ַ�
*/

int StringHEXToUshort(char *hex,unsigned int *value)
{
	unsigned short temp=0;
	int num;
	while(*hex!=0)
	{
		temp *= 16;
		num = CharToInt(*hex);
		if(num==-1)
			return -1;
		temp += num;
		hex++;
	}
	*value = temp;
	return 0;
}


/*
FOUNCTION:�������йؼ��ֵ��ļ��������浽ָ����ά����
INPUT:path,point of file path;ex_name,browser key,
OUTPUT:name,�����ļ���
*/
uint8_t EXFile_Browser(char* path,char *ex_name,char *name)
{
  FRESULT res;
  uint8_t ret = 0;
  FILINFO fno;
  DIR dir;
  char *fn;
	int i=0;
	int len;
	int offset;
	#if _USE_LFN 
	char fname_buf[_MAX_LFN];
	fno.lfname = fname_buf;
	fno.lfsize = _MAX_LFN;
	#endif
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    
    for (;;) {
      res = f_readdir(&dir, &fno);
		#if _USE_LFN 
			fn = fno.lfname;
			if (fno.fname[0] == '.') continue;
		  if (res != FR_OK || fno.lfname[0] == 0) 
				if(fno.fname[0] == 0)
					break;
				else
					fn = fno.fname;
		#else
			if (res != FR_OK || fno.fname[0] == 0)
						break;
      if (fno.fname[0] == '.') continue;
			fn = fno.fname;
		#endif
								Debug_printf(fn);
					Debug_printf("\r\n");
      if (fno.fattrib & AM_DIR) //Ŀ¼
      {
        continue;
      } 
      else 
      {
        if((strstr(fn,ex_name)!=NULL)||strstr(fn,".CSV")!=NULL)
        {
					memset((char *)(name+i*EXFile_MAX_LEN),0,EXFile_MAX_LEN);
					len = strlen(fn);
				//	len -= 4;//.csv
					if(len>EXFile_MAX_LEN)
					{
						offset = len - EXFile_MAX_LEN;
					}
					else
					{
						offset = 0;
					}
					memcpy((char *)(name+i*EXFile_MAX_LEN),fn+offset,len-offset);
					
					i++;
					if(i>=EXFile_MAX_NUM)
						break;			
        }
      }
    }
		if(i==0)
			ret = FR_NO_FILE;
  }
  return ret;
}

#define BROWSER_FILE		0x01
#define SELECT_FILE			0x02
#define BROWSER_FAIL		0x03
#define SAVE_SCENE			0x04
#define DISPLAY_INFO			0x05
int SceneFileBrowser(void)
{

}



/*
FOUNCTION:��ȡ�ļ���һ��,�س����лᶪ��
INPUT:file,point of file,max_len,��ȡ����󳤶�
OUTPUT:buf,buffer of read data
return 0,OK,1,�ļ�������;2,�����˶�����󳤶�;3,������
*/
int f_readline(FIL *file,char *buf,int max_len)
{
	char byte,state=0,temp;
	int num;
	
  while(max_len--)
	{
		if(f_read(file,&temp,1,&num))
			return 3;
		if(num==0)
			return 1;
		
		if(temp==0x0D)
			state = 1;
		else if(state&&(temp==0x0A))
			return 0;
		else
			*buf = temp;
		buf++;
	}
	return 2;
}

/*
FOUNCTION:���������ļ�
INPUT:fname, piont of file name
OUTPUT:ImportSceneData, �������ݵ�ָ��
RETURN:1,�ļ���ʽ����

*/
#define FILE_FEATRUE	"FJYSC"



extern FIL file;
int PraseSceneFile(char *fname,SceneData *scene_data)
{
	FIL *pfile;
	int res=0;
	char buf[100];
	char *p,*last_p;
	char temp;
	unsigned char version;
	unsigned char pixels;
	unsigned char chanles;
	unsigned tim;
	int i,j,k;
	char hex_buf[5];//һ��Ϊ�ַ���������
	unsigned int value;
	//pfile = (FIL*)malloc(sizeof(FIL));
	pfile = &file;
	if(pfile == NULL)
	{
		return ERRO_MALLOC;
	}
	res = f_open(pfile,fname, FA_OPEN_EXISTING | FA_READ);
	//�����ļ�ͷ
	memset(buf,0,100);
	res = f_readline(pfile,buf,100);
	p = strstr(buf,"FJYSC,");
	if(p==NULL)
	{
		//free(pfile);
		return 1;
	}
	temp = strlen(FILE_FEATRUE);
	version = atoi((const char*)(p+temp+1));
	if((version <10)||(version>99))
	{
		
	}
	p = strstr((const char*)(p+temp+1),(const char*)",");
	if(p==NULL)
	{
	//	free(pfile);
		return 1;
	}
	p++;
	pixels = atoi(p);
	if((pixels <1)||(pixels>8))
	{
		
	}
	p = strstr((const char*)p,(const char*)",");
	if(p==NULL)
	{
	///	free(pfile);
		return 1;
	}
	p++;
	chanles = atoi(p);
	if((chanles <1)||(chanles>8))
	{
		
	}
	p = strstr((const char*)p,(const char*)",");
	if(p==NULL)
	{
	//	free(pfile);
		return 1;
	}
	p++;
	tim = atoi(p);
	
	p = strstr((const char*)p,(const char*)",");
	if(p==NULL)
	{
	//	free(pfile);
		return 1;
	}
	p++;
	scene_data->mode = atoi(p);
	//����ÿ֡����
	scene_data->name = fname;
	while(1)
	{
		memset(buf,0,100);
		res = f_readline(pfile,buf,100);
		last_p = p = buf;
		switch(res)
		{
			case 0: 
			{
				for(j=0;j<pixels;j++)
				{
					for(k=0;k<chanles;k++)
					{
						p = strstr((const char *)p,(const char *)",");
						if(p==NULL)
								goto NO_DATA;	
						if((p-last_p)>4)
							goto NO_DATA;	
						memset(hex_buf,0,5);
						memcpy(hex_buf,(const char *)last_p,p-last_p);
						if(StringHEXToUshort(hex_buf,&value)!=0)
							goto NO_DATA;	
						scene_data->frame[i].ch[j][k] = value;
						p++;
						last_p = p;
					}
				}
				if(tim==0)
				{
						p = strstr((const char *)p,(const char *)",");
						if(p==NULL)
								goto NO_DATA;	
						if((p-last_p)>4)
							goto NO_DATA;	
						memset(hex_buf,0,5);
						memcpy(hex_buf,(const char *)last_p,p-last_p);
						if(StringHEXToUshort(hex_buf,&value)!=0)
							goto NO_DATA;	
						scene_data->frame[i].tim = value;
				}
			}
				break;
			case 1: scene_data->max_frames = i;return 0;
			case 2:
			case 3:  return 1;
			default : return 1;
		}
		i++;
		NO_DATA:continue;
	}
//	free(pfile);
	return 1;
}
void SelectBar(Picture *pic,SELECTBAR *bar);



void brower_scene_file(void)
{
	int i;


			memset(import_scene_bar.name,0,sizeof(import_scene_bar.name));
			if(EXFile_Browser("0:/",".csv",(char *)import_scene_bar.name)==0)
			{
				import_scene_bar.max_line = DIS_MAX_LINE; //���ú�selcet bar
				for(i=0;i<EXFile_MAX_NUM;i++)
				{
					if(import_scene_bar.name[i][0]==0)
						break;
				}
				import_scene_bar.max_nums = i;
				import_scene_bar.rect = Rect;
				Sys.scene.state = SELECT_FILE;
			}
			else
			{
				Sys.scene.state = BROWSER_FAIL;
			}
			
}


void ImportInit(void)
{
	if(Sys.Config.english)
		HeadDisplay("IMPORT SCENE"); 
	else
		HeadDisplay("���볡��"); 
	Sys.scene.state = BROWSER_FILE;
	Sys.usb.USBH_USR_ApplicationState = USH_USR_FS_IMPORT_SCENE;
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 4;
}

void ImportTask(void)
{
	u8 offset = (MENU_W%16)/2; // ˮƽ���Ķ���
	int i=0;
	static int option_pos=0,line_pos=0;
	switch(Sys.scene.state)
	{
		case BROWSER_FILE:
			ClearMenu(OPTION_COLOR);
			BACK_COLOR = OPTION_COLOR;
			POINT_COLOR = BLACK;
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				if(Sys.Config.english)
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  Insert USB please.");
				}
				else
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  �����USB��");
				}
				goto end;
			}
			else
			{
				if(Sys.Config.english)
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  Seaching...");
				}
				else
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  ����������");
				}
			}
			break;
		case BROWSER_FAIL:
			ClearMenu(OPTION_COLOR);
			BACK_COLOR = OPTION_COLOR;
			POINT_COLOR = BLACK;
			if(Sys.Config.english)
			{
				Pictrue_printf(&MenuPic,offset,0,16,"  Not detect scene file.Extend name of file must be .csv");
			}
			else
			{
				Pictrue_printf(&MenuPic,offset,0,16,"  û����������Ч���ļ����ļ���չ��������.csv��");
			}
			break;
		case SELECT_FILE:
		{
			Sys.menu_mask = 1;//���� EnterKey
			if(ec11_pos[2])
			{
				import_scene_bar.option += ec11_pos[2];
				import_scene_bar.line += ec11_pos[2];
				if(import_scene_bar.option>=import_scene_bar.max_nums)
					import_scene_bar.option = import_scene_bar.max_nums-1;
				else if(import_scene_bar.option<0)
					import_scene_bar.option = 0;
				
				if(import_scene_bar.line>=DIS_MAX_LINE)
					import_scene_bar.line= DIS_MAX_LINE-1;
				else if(import_scene_bar.line<0)
					import_scene_bar.line = 0;		
				ec11_pos[2] = 0; //����
			}
			if(key_value == S10)//��������
			{
				Sys.scene.state = SAVE_SCENE;
				Sys.menu_mask = 0;		
			}
			SelectBar(&MenuPic,&import_scene_bar);
		}
		break;
		case SAVE_SCENE:
			Sys.menu_mask = 1;//���� EnterKey
			ClearMenu(OPTION_COLOR);
			BACK_COLOR = OPTION_COLOR;
			POINT_COLOR = BLACK;
			if(PraseSceneFile(import_scene_bar.name[import_scene_bar.option],&Sys.Config.ImportSceneData))
			{
				if(Sys.Config.english)
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  File erro.");
				}
				else
				{
					Pictrue_printf(&MenuPic,offset,0,16,"  �ļ���Ч��");
				}
				if(key_value == S10)//��������
				{
					Sys.scene.state = SELECT_FILE;;
					Sys.menu_mask = 0;		
				}
			}
			else
			{
				SceneBegain(&Sys.Config.ImportSceneData);
				Sys.scene.state = DISPLAY_INFO;
			}
		break;
		case DISPLAY_INFO:
				Pictrue_printf(&MenuPic,offset,0,16,"   %s",scene->name);
				Pictrue_printf(&MenuPic,offset,0+16,16,"mode:%d",scene->mode);
				Pictrue_printf(&MenuPic,offset,0+32,16,"max_frame:%d",scene->max_frames);
				Pictrue_printf(&MenuPic,offset,0+48,16,"frame:%d",scene->frame_pos);
			break;
		default: break;
	}
end:
	FreshMenu();
}


void SelectBar(Picture *pic,SELECTBAR *bar)
{
	BMP bmp;
	int i;
	int move_pos;
	static TRANDISPLAY tran;
	Picture_Fill(*pic,MENU_BACK_COLOR);
	//��ʾ
	POINT_COLOR = BLACK;
	Picture_FillRect(*pic,bar->rect[bar->line],SELECT_COLOR);//ѡ��ѡ���Ի�ɫ��ʾ
	//Picture_ShowStringInRectCenter(*pic,bar->rect[bar->line],1,16,bar->name[bar->option]);
	Picture_TranDispalyOnline(pic,&bar->rect[bar->line],&tran,16,1,bar->name[bar->option]);
	move_pos = bar->option;
	tran.move_t = 20;
	//��ʾǰ�����
	for(i=bar->line-1;i>=0;i--)
	{
		move_pos--;
		if(move_pos>=0)
		{
			POINT_COLOR = BLACK;
			Picture_FillRect(*pic,bar->rect[i],OPTION_COLOR);
		Picture_ShowStringInRectCenter(*pic,bar->rect[i],1,16,bar->name[move_pos]);
			//Picture_TranDispalyOnline(pic,&bar->rect[i],&tran,16,1,bar->name[move_pos]);
			
			
		}
		else
			break;	
	}
	if(move_pos>0)//��ʾ���滹��ѡ��
	{
		//Picture_DrawTrigon(MenuPic,MENU_W/2-10,0,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)trigon_data_16X8;
		Picture_DrawBMP(*pic,pic->w/2-10,0,&bmp,LGRAY,MENU_BACK_COLOR);
	}
	
	//��ʾ�����
	move_pos = bar->option;
	for(i=bar->line+1;i<bar->max_line;i++)
	{
		move_pos++;
		if(move_pos < bar->max_nums)
		{
			POINT_COLOR = BLACK;
			Picture_FillRect(*pic,bar->rect[i],OPTION_COLOR);
			Picture_ShowStringInRectCenter(*pic,bar->rect[i],1,16,bar->name[move_pos]);
			//Picture_TranDispalyOnline(pic,&bar->rect[i],&tran,16,1,bar->name[move_pos]);
		}
		else
		{
			break;
		}
	}
	if(move_pos < bar->max_nums-1)
	{
		//Picture_DrawTrigon(MenuPic,MENU_W/2-10,0,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)inverted_trigon_data_16X8;
		Picture_DrawBMP(MenuPic,pic->w/2-10,bar->rect[bar->max_line-1].y1+RECT_INTERVAL,&bmp,LGRAY,MENU_BACK_COLOR);
	}
}

//	
void SceneDuty(SceneData *scene)
{
	RGB ledk;
	float dim = 0;;
	u16 *temp;
	int i,j;
	if(scene == NULL)
		return;
	//װ��LED�������
	for(j=0;j<2;j++) 
	{
		temp = scene->frame[scene->frame_pos].ch[j];
		for(i=0;i<LED_NUMS;i++)
		{
			dim += temp[i];
		}
		dim = dim/65535.0;
		if(dim>1)
			dim = 1;
		ledk.ww = temp[0];
		ledk.cw = temp[1];
		ledk.r = temp[2];
		ledk.g = temp[3];
		ledk.b = temp[4];
		LedPowerOut(&ledk,dim,j);
	}
	//������һ֡��ʱ��
	SceneSetframeTim(scene->frame[scene->frame_pos].tim);
	scene->frame_pos++;
	switch(scene->mode)
	{
		case 0://����ѭ��
		{
			if(scene->frame_pos > scene->max_frames)//���
			{
				scene->frame_pos = 0;
			}
			break;
		}
		case 254://�������������״̬
		{
			if(scene->frame_pos > scene->max_frames)
			{
				scene = NULL;
				SceneTimDisable();
			}
			break;
		}	
		case 255://������Ϩ��
		{
			if(scene->frame_pos > scene->max_frames)
			{
				AllLedPowerOff();
				scene = NULL;
				SceneTimDisable();
			}
			break;
		}
		default :
		{
			if(scene->frame_pos > scene->max_frames)
			{
				if(scene->loop>0)
				{
					scene->frame_pos = 0;
					scene->loop = 0;
				}
				scene = NULL;
				SceneTimDisable();
			}
			break;
		}
	}
}

void SceneBegain(SceneData *pscene)
{
	if(pscene == NULL)
		return;
	scene = pscene;
	scene->frame_pos = 0;
	scene->loop = scene->mode;
	SceneSetframeTim(1);
	SceneTimEnable();
}

float tim4_t,ttt; 
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		SceneDuty(scene);
	}
}

extern RGB rgbk;
extern u16 current[LED_NUMS];
//��˸����
float FlashDim = 0.5;
u16 FlashTim = 100;
void SceneFlashInit(void)
{
	int i,j;
	SceneData *pScene=NULL;
	//pScene = mymalloc(SRAMIN,sizeof(SceneData));
	pScene = &scendatabuf;
	if(pScene != NULL)
	{
		pScene->max_frames = 1;
		pScene->mode = 0;//����ѭ��
		LedkToCurrent(&rgbk,FlashDim,2);
		for(i=0;i<2;i++)
		{
			for(j=0;j<LED_NUMS;j++)
				pScene->frame[0].ch[i][j] = current[j];
			
			for(j=0;j<LED_NUMS;j++)
				pScene->frame[1].ch[i][j] = 0;
			
			pScene->frame[0].tim = FlashTim;
			pScene->frame[1].tim = FlashTim;
		}
	}
	else
	{
		Debug_printf(">>SceneFlashInit:mymalloc()fail\r\n");
	}
	SceneBegain(pScene);
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 1;
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
			FlashTim -= ec11_pos[1]*0.5*EC11_speed;
		}
		else
		{

			FlashTim -= ec11_pos[1];
		}
		if(FlashTim<100)
		{
			FlashTim = 100;
		}
		ec11_pos[1] = 0;
		SceneFlashInit();
	}
	
	Pictrue_printf(&MenuPic,0,0,16,"   Flash");
	Pictrue_printf(&MenuPic,0,16,16,"  T:%dms",FlashTim);
	Pictrue_printf(&MenuPic,0,32,16,"  Dim:%0.1f%%",FlashDim*100);
	FreshMenu();
}

void SceneBack(void)
{
	SceneTimDisable();
}

