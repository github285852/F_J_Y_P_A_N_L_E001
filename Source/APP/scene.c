#include "includes.h"


typedef struct
{
	char name[EXFile_MAX_NUM+1][EXFile_MAX_LEN];
	int option;//显示第几选项
	int line;//选项在第几行显示
	int max_nums;//显示的最大个数,整个选择条要显示的个数
	int max_line;//显示的最大行，一次能显示的个数
	RECT *rect;//显示的矩形行指针
}SELECTBAR;

void SceneBegain(SceneData *pscene);

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
FOUNCTION:将16进制的字符串转换成数值
INPUT:hex,要转成数值的16进制字符串
OUTPUT:value, point of value
RETURN:0,OK,-1,有非法字符
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
FOUNCTION:搜索含有关键字的文件，并保存到指定二维数组
INPUT:path,point of file path;ex_name,browser key,
OUTPUT:name,保存文件名
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
      if (fno.fattrib & AM_DIR) //目录
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
FOUNCTION:读取文件的一行,回车换行会丢掉
INPUT:file,point of file,max_len,读取的最大长度
OUTPUT:buf,buffer of read data
return 0,OK,1,文件读完了;2,超出了读的最大长度;3,读错误。
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
FOUNCTION:解析场景文件
INPUT:fname, piont of file name
OUTPUT:ImportSceneData, 保存数据的指针
RETURN:1,文件格式错误

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
	char hex_buf[5];//一个为字符串结束符
	unsigned int value;
	//pfile = (FIL*)malloc(sizeof(FIL));
	pfile = &file;
	if(pfile == NULL)
	{
		return ERRO_MALLOC;
	}
	res = f_open(pfile,fname, FA_OPEN_EXISTING | FA_READ);
	//解析文件头
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
	//解析每帧数据
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
void SelectBar(Picture *pic,SELECTBAR *bar,int changeline);



void brower_scene_file(void)
{
	int i;


			memset(import_scene_bar.name,0,sizeof(import_scene_bar.name));
			if(EXFile_Browser("0:/",".csv",(char *)import_scene_bar.name)==0)
			{
				import_scene_bar.max_line = DIS_MAX_LINE; //配置好selcet bar
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
		HeadDisplay("导入场景"); 
	Sys.scene.state = BROWSER_FILE;
	Sys.usb.USBH_USR_ApplicationState = USH_USR_FS_IMPORT_SCENE;
	Sys.Config.lightmode = SCENE_M;
	Sys.Config.scene.num = 4;
}

void ImportTask(void)
{
	u8 offset = (MENU_W%16)/2; // 水平中心对齐
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
					Pictrue_printf(&MenuPic,offset,0,16,"  请插入USB。");
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
					Pictrue_printf(&MenuPic,offset,0,16,"  正在搜索。");
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
				Pictrue_printf(&MenuPic,offset,0,16,"  没有搜索到有效的文件。文件扩展名必须是.csv。");
			}
			break;
		case SELECT_FILE:
		{
			Sys.menu_mask = 1;//屏蔽 EnterKey

			if(key_value == PSELECT_KEY)//不用清零
			{
				Sys.scene.state = SAVE_SCENE;
				Sys.menu_mask = 0;		
			}
			SelectBar(&MenuPic,&import_scene_bar,ec11_pos[2]);
			ec11_pos[2] = 0; //清零
		}
		break;
		case SAVE_SCENE:
			Sys.menu_mask = 1;//屏蔽 EnterKey
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
					Pictrue_printf(&MenuPic,offset,0,16,"  文件无效。");
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
				if(key_value == PSELECT_KEY)//不用清零
				{
					SceneBegain(&Sys.Config.ImportSceneData);
				}
			break;
		default: break;
	}
end:
	FreshMenu();
}

//显示选择条
void SelectBar(Picture *pic,SELECTBAR *bar,int changeline)
{
	BMP bmp;
	int i;
	int dis_pos;
	static TRANDISPLAY tran;
	int line;
	bar->option += changeline;
	if(bar->option>=bar->max_nums)
		bar->option = bar->max_nums-1;
	else if(bar->option<0)
		bar->option = 0;
		
	line = bar->option/bar->max_line; //从第几行开始显示
	dis_pos = bar->option%bar->max_line;//显示区域行数坐标 0 - bar->max_line-1
	
	Picture_Fill(*pic,MENU_BACK_COLOR);
	//显示选项
	POINT_COLOR = BLACK;
	Picture_FillRect(*pic,bar->rect[dis_pos],SELECT_COLOR);//选中选项以灰色表示
	//Picture_ShowStringInRectCenter(*pic,bar->rect[bar->line],1,16,bar->name[bar->option]);
	Picture_TranDispalyOnline(pic,&bar->rect[dis_pos],&tran,16,1,bar->name[bar->option]);

	tran.move_t = 20;
	//显示前面的行
	for(i=dis_pos-1;i>=0;i--)
	{
		POINT_COLOR = BLACK;
		Picture_FillRect(*pic,bar->rect[i],OPTION_COLOR);
		Picture_ShowStringInRectCenter(*pic,bar->rect[i],1,16,bar->name[line*bar->max_line+i]);
		//Picture_TranDispalyOnline(pic,&bar->rect[i],&tran,16,1,bar->name[move_pos]);	
	}
	if(line>0)//提示上面还有选项
	{
		//Picture_DrawTrigon(MenuPic,MENU_W/2-10,0,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)trigon_data_16X8;
		Picture_DrawBMP(*pic,pic->w/2-10,0,&bmp,LGRAY,MENU_BACK_COLOR);
	}
	
	//显示后面的
//	move_pos = bar->option;
	for(i=dis_pos+1;i<bar->max_line;i++)
	{
		if(line*bar->max_line+i < bar->max_nums)
		{
			POINT_COLOR = BLACK;
			Picture_FillRect(*pic,bar->rect[i],OPTION_COLOR);
			Picture_ShowStringInRectCenter(*pic,bar->rect[i],1,16,bar->name[line*bar->max_line+i]);
			//Picture_TranDispalyOnline(pic,&bar->rect[i],&tran,16,1,bar->name[move_pos]);
		}
	}
	if((bar->max_nums - line*bar->max_line) > bar->max_line)//提示下面还有选项
	{
		//Picture_DrawTrigon(MenuPic,MENU_W/2-10,0,8,10,LGRAY);
		bmp.w = 16;
		bmp.h = 8;
		bmp.data = (unsigned char *)inverted_trigon_data_16X8;
		Picture_DrawBMP(MenuPic,pic->w/2-10,bar->rect[bar->max_line-1].y1+RECT_INTERVAL,&bmp,LGRAY,MENU_BACK_COLOR);
	}
}

//根据导入的 Scene 数据 执行场景任务
void SceneDuty(SceneData *scene)
{
	RGB ledk;
	float dim = 0;;
	int i,j;
	if(scene == NULL)
		return;
	//装载LED输出数据
	for(j=0;j<PIXELS;j++) 
	{
		DMXChanleDataRefresh(j,scene->frame[scene->frame_pos].ch[j]);
	}
	if(scene->frame[scene->frame_pos].tim>100)
		DMX1_TX_BUF[0] = 0;
	else
		DMX1_TX_BUF[0] = 1;
	DMX1_Send(DMX1_TX_BUF,DMX_LEN);
	//设置下一帧的时间
	SceneSetframeTim(scene->frame[scene->frame_pos].tim);
	scene->frame_pos++;
	switch(scene->mode)
	{
		case 0://无限循环
		{
			if(scene->frame_pos > scene->max_frames)//最后
			{
				scene->frame_pos = 0;
			}
			break;
		}
		case 254://播放完留在最后状态
		{
			if(scene->frame_pos > scene->max_frames)
			{
				scene = NULL;
				SceneTimDisable();
			}
			break;
		}	
		case 255://播放完熄灭
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
					scene->loop--;
				}
				else
				{
					scene = NULL;
					SceneTimDisable();
				}
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
	ClearCNT();
	SceneTimEnable();
}

float tim4_t,ttt; 
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
		SceneDuty(scene);
	}
}





