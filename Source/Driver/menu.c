
#include "menu.h"
#include "menu_handle.h"
#include "string.h"
#include "menutask.h"
#include "scene.h"

unsigned char MAX_MENUS;
struct MenuTyp *InitMenup;
//初始化好所有菜单
//
struct MenuTyp MenuTab[] = {
//从右至左     |        菜单名
//第一个不     |
//为零的数 
//所在坐标
//为菜单等级   | 
//   菜单等级                     英文名      中文名        指针函数
	{1,0,0,0,0,0,(unsigned char *)"CCT MODE",   "CCT模式",  				},
	{2,0,0,0,0,0,(unsigned char *)"HSI MODE",   "HSI模式",    0,0,0,},
	{3,0,0,0,0,0,(unsigned char *)"GEL MODE",   "GEL模式",					},
	{4,0,0,0,0,0,(unsigned char *)"RGB MODE",   "RGB模式",          },
	{5,0,0,0,0,0,(unsigned char *)"SCENE MODE", "场景模式",				  },
	{6,0,0,0,0,0,(unsigned char *)"SETTINGS",		"设置"			        },
//二级
	{1,1,0,0,0,0,(unsigned char *)"CCT DUTY",			0,       CCTInitTask,0,0,CCTTask},
	{2,1,0,0,0,0,(unsigned char *)"HSI DUTY",     0,       HSIInitTask,0,0,HSITask},
	{3,1,0,0,0,0,(unsigned char *)"GEL DUTY",			0,       GELInitTask,0,0,GELTask},
	{4,1,0,0,0,0,(unsigned char *)"RGB DUTY",			0,       RGBInitTask,0,0,RGBTask}, //必须以一条结束
	{5,1,0,0,0,0,(unsigned char *)"SCENE 1",		 "场景一"								       }, 
	{5,2,0,0,0,0,(unsigned char *)"SCENE 2",		 "场景二"								      }, 
	{5,3,0,0,0,0,(unsigned char *)"SCENE 3",		 "场景三"											}, 
	{5,4,0,0,0,0,(unsigned char *)"IMPORT",		  	"导入场景"								  }, 
	
	{6,1,0,0,0,0,(unsigned char *)"IN SELECT",	  "输入选择"                    },
	{6,2,0,0,0,0,(unsigned char *)"DMX", "DMX"                    },
	{6,3,0,0,0,0,(unsigned char *)"FAN", "风扇"   },
	{6,4,0,0,0,0,(unsigned char *)"DISPLAY",      "显示"    },
	{6,5,0,0,0,0,(unsigned char *)"USB",  "USB" },
	{6,6,0,0,0,0,(unsigned char *)"LANGUAGE",      "语言"},
	{6,7,0,0,0,0,(unsigned char *)"SYSTEM",        "系统"     },
//三级                             英文名              中文名           指针函数
	{5,1,1,0,0,0,(unsigned char *)"SCENE 1 DUTY",   		 0,	  			SceneFlashInit	,	SceneBack,SceneBack,SceneFlashTask}, 
	{5,2,1,0,0,0,(unsigned char *)"SCENE 2 DUTY",	   		 0,					Scene2TaskInit	,									}, 
	{5,3,1,0,0,0,(unsigned char *)"SCENE 3 DUTY",	   		 0,					Scene3TaskInit	,									}, 
	{5,4,1,0,0,0,(unsigned char *)"IMPORT SCENE DUTY",   0,			ImportInit,		0,0,ImportTask		}, 
	
	{6,1,1,0,0,0,(unsigned char *)"AUTO DETECT",   "自动检测"     },
	{6,1,2,0,0,0,(unsigned char *)"DMX IN",        "DMX输入"            },
	{6,1,3,0,0,0,(unsigned char *)"Art-Net IN",     "Art-Net输入"      },
	{6,2,1,0,0,0,(unsigned char *)"DMX MODE ",     "DMX模式"           },
	{6,2,2,0,0,0,(unsigned char *)"DMX ADDRES",      "DMX地址"             },
	{6,3,1,0,0,0,(unsigned char *)"FAN AUTO",        "自动"      },
	{6,3,2,0,0,0,(unsigned char *)"FAN OFF",         "关"   },
	{6,3,3,0,0,0,(unsigned char *)"DMX FAN",         "DMX控制"          },
	
	{6,4,1,0,0,0,(unsigned char *)"DIM",	    "背光亮度"},
	{6,4,2,0,0,0,(unsigned char *)"TIME",	  "背光时间"},
	{6,4,3,0,0,0,(unsigned char *)"ROTATE",	  "屏幕翻转"},

	{6,5,1,0,0,0,(unsigned char *)"UPDATE",	  "固件更新"},
	{6,5,2,0,0,0,(unsigned char *)"IMPORT",	            "导入文件" },
	
	{6,6,1,0,0,0,(unsigned char *)"ENGLISH",	          "英文"     },
	{6,6,2,0,0,0,(unsigned char *)"CHINESE",	          "中文"     },
	
	{6,7,1,0,0,0,(unsigned char *)"INFO",	              "关于设备"  },
	{6,7,2,0,0,0,(unsigned char *)"DEAULTS",	          "恢复出厂设置"  },
	{6,7,3,0,0,0,(unsigned char *)"MONITOR",	          "监控"     },
	
//四级  //最后一级目录 上下目录指针都为空
	{6,1,1,1,0,0,(unsigned char *)"AUTO DETECT DUTY", 0,          AutoDetectINInit, 0,Save_Interface     },
	{6,1,2,1,0,0,(unsigned char *)"DMX IN DUTY",      0,               DMXINTaskInit,0,Save_Interface    },
	 
	{6,1,3,1,0,0,(unsigned char *)"STATIC IP",        "静态IP",                          },
	{6,1,3,2,0,0,(unsigned char *)"DYNAMIC IP",       "自动获取"                          },

	{6,2,1,1,0,0,(unsigned char *)"DMX Mode DUTY",       0,					DMX_ModeTaskInit,  0,0,DMX_ModeTask, },
	{6,2,2,1,0,0,(unsigned char *)"DMX ADDRES DUTY",     0,         DMX_AdressTaskInit,0,0,DMX_AdressTask,    },
	{6,3,1,1,0,0,(unsigned char *)"FAN AUTO DUTY",       0,         FAN_AUTO_TaskInit, 0,SaveFanConfig  },
	{6,3,2,1,0,0,(unsigned char *)"FAN OFF DUTY",        0,         FAN_OFF_TaskInit,  0,SaveFanConfig},
	{6,3,3,1,0,0,(unsigned char *)"DMX FAN DUTY",        0,         FAN_DMX_TaskInit,  0,SaveFanConfig},
	
	{6,4,1,1,0,0,(unsigned char *)"BACKLITE DIM DUTY",	    0,         LCD_DIM_TaskInit,0,0, LCD_DIM_Task,  },
	{6,4,2,1,0,0,(unsigned char *)"BACKLITE TIME DUTY",	    0,         LCD_TIM_TaskInit,0,0,LCD_TIM_Task,  },
	{6,4,3,1,0,0,(unsigned char *)"ROTATE SCREEN DUTY",     0,         LCD_RotateTaskInit,0,0,LCD_RotateTask  },

	{6,5,1,1,0,0,(unsigned char *)"FIRMWARE UPDATE DUTY",	     0,      firmware_update_init,0,firmware_update_Enter, firmware_update   },
	{6,5,2,1,0,0,(unsigned char *)"IMPORT DUTY",	      0,         },
	
	{6,6,1,1,0,0,(unsigned char *)"ENGLISH DUTY",	      0,               EnglishTaskInit,0,0,ChineseTask      },
	{6,6,2,1,0,0,(unsigned char *)"CHINESE DUTY",	      0,               ChineseTaskInit,0,0,ChineseTask},
	
	{6,7,1,1,0,0,(unsigned char *)"INFO DUTY",	        0,         InfoTaskInit,0,0,InfoTask},
	{6,7,2,1,0,0,(unsigned char *)"DEFAULT DUTY",	      0,          },
	{6,7,3,1,0,0,(unsigned char *)"MONITOR DUTY",	      0,         },
// 五级
	{6,1,3,1,1,0,(unsigned char *)"GATEWAY",          "网关"         },
	{6,1,3,1,2,0,(unsigned char *)"IP ADDRES",         "IP地址"    },
	{6,1,3,1,3,0,(unsigned char *)"SUBNET MASK",      "子网掩码"  },
	{6,1,3,2,1,0,(unsigned char *)"DYNAMIC IP DUTY",    0,         },
//六级
	{6,1,3,1,1,1,(unsigned char *)"GATEWAY DUTY",       0,         },
	{6,1,3,1,2,1,(unsigned char *)"IP ADDRES DUTY",     0,         },
	{6,1,3,1,3,1,(unsigned char *)"SUBNET MASK DUTY",   0,         },
	
};

struct MenuTyp *FindParent(struct MenuTyp *menutyp)
{
	int i,j=0,k;
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//确定自己所在层
	if(i==0)//最顶层
	{
		return 0;
	}
	else
	{
		while(j<MAX_MENUS)//
		{                   
			for(k=0;k<i;k++)//从最顶层开始匹配寻找
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])//上一层
				{
					break;
				}
			}
			if(k==i) //和母层索引号都相同
			{
				return &MenuTab[j];
			}
			j++;
		}
	}
	return 0;
}

struct MenuTyp *FindChild(struct MenuTyp *menutyp)
{
	int i,j=0,k;
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//确定自己所在层
	if(i==MAX_LEVES-1)//最底层
	{
		return 0;
	}
	else
	{
		while(j<MAX_MENUS)//搜索所有
		{
			for(k=0;k<=i;k++)//从最顶层开始匹配寻找
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i+1]==1)&&(k==i+1))//下一层
			{
					return &MenuTab[j];
			}
			j++;
		}
	}
	return 0;
}

struct MenuTyp *FindLeft(struct MenuTyp *menutyp)
{
	int i,j=0,k;
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//确定自己所在层
	if(menutyp->leves[i]==1)//最右层
	{
		return 0;
	}
	i++;
	while(j<MAX_MENUS)
	{
		if(i==1)
		{
			if(MenuTab[j].leves[i-1]==menutyp->leves[i-1]-1)// 左
			{
					return &MenuTab[j];
			}
		}
		else
		{
			for(k=0;k<i;k++)//从最顶层开始匹配寻找
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i-1]==menutyp->leves[i-1]-1)&&(k==i-1))//左
			{
					return &MenuTab[j];
			}
		}
		j++;
	}
	return 0;
}

struct MenuTyp *FindRight(struct MenuTyp *menutyp)
{
	int i,j=0,k;
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//确定自己所在层
	i++;
	while(j<MAX_MENUS)
	{
		if(i==1)
		{
			if(MenuTab[j].leves[i-1]==menutyp->leves[i-1]+1)//右层
			{
					return &MenuTab[j];
			}
		}
		else
		{
			for(k=0;k<i;k++)//从最顶层开始匹配寻找
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i-1]==menutyp->leves[i-1]+1)&&(k==i-1))//右层
			{
					return &MenuTab[j];
			}
		}
		j++;
	}
	return 0;
}

void CreateMenu(void)
{
	int i;
	MAX_MENUS = sizeof(MenuTab)/sizeof(struct MenuTyp);
	InitMenup = &MenuTab[0];
	for(i=0;i<MAX_MENUS;i++)
	{
		MenuTab[i].pParent = FindParent(&MenuTab[i]);
		MenuTab[i].pChild = FindChild(&MenuTab[i]);
		MenuTab[i].pLeft = FindLeft(&MenuTab[i]);
		MenuTab[i].pRight = FindRight(&MenuTab[i]);
	}
}

//获取菜单在选项号
unsigned char Menu_GetoptionNum( struct MenuTyp *menu_option)
{
	int i=MAX_LEVES-1;
	while(menu_option->leves[i]==0)
	{
		i--;
	}
	return menu_option->leves[i];
}



