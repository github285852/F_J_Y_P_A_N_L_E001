
#include "menu.h"
#include "menu_handle.h"
#include "string.h"
#include "menutask.h"
#include "scene.h"

unsigned char MAX_MENUS;
struct MenuTyp *InitMenup;
//��ʼ�������в˵�
//
//struct MenuTyp MenuTab[] = {
////��������     |        �˵���
////��һ����     |
////Ϊ����� 
////��������
////Ϊ�˵��ȼ�   | 
////   �˵��ȼ�                     Ӣ����      ������        ָ�뺯��
//	{1,0,0,0,0,0,(unsigned char *)"CCT MODE",   "CCTģʽ",  				},
//	{2,0,0,0,0,0,(unsigned char *)"HSI MODE",   "HSIģʽ",    0,0,0,},
//	{3,0,0,0,0,0,(unsigned char *)"GEL MODE",   "GELģʽ",					},
//	{4,0,0,0,0,0,(unsigned char *)"RGB MODE",   "RGBģʽ",          },
//	{5,0,0,0,0,0,(unsigned char *)"SCENE MODE", "����ģʽ",				  },
//	{6,0,0,0,0,0,(unsigned char *)"SETTINGS",		"����"			        },
////����
//	{1,1,0,0,0,0,(unsigned char *)"CCT DUTY",			0,       CCTInitTask,0,0,CCTTask},
//	{2,1,0,0,0,0,(unsigned char *)"HSI DUTY",     0,       HSIInitTask,0,0,HSITask},
//	{3,1,0,0,0,0,(unsigned char *)"GEL DUTY",			0,       GELInitTask,0,0,GELTask},
//	{4,1,0,0,0,0,(unsigned char *)"RGB DUTY",			0,       RGBInitTask,0,0,RGBTask}, //������һ������
//	{5,1,0,0,0,0,(unsigned char *)"SCENE 1",		 "����һ"								       }, 
//	{5,2,0,0,0,0,(unsigned char *)"SCENE 2",		 "������"								      }, 
//	{5,3,0,0,0,0,(unsigned char *)"SCENE 3",		 "������"											}, 
//	{5,4,0,0,0,0,(unsigned char *)"IMPORT",		  	"���볡��"								  }, 
//	
//	{6,1,0,0,0,0,(unsigned char *)"IN SELECT",	  "����ѡ��"                    },
//	{6,2,0,0,0,0,(unsigned char *)"DMX", "DMX"                    },
//	{6,3,0,0,0,0,(unsigned char *)"FAN", "����"   },
//	{6,4,0,0,0,0,(unsigned char *)"DISPLAY",      "��ʾ"    },
//	{6,5,0,0,0,0,(unsigned char *)"USB",  "USB" },
//	{6,6,0,0,0,0,(unsigned char *)"LANGUAGE",      "����"},
//	{6,7,0,0,0,0,(unsigned char *)"SYSTEM",        "ϵͳ"     },
////����                             Ӣ����              ������           ָ�뺯��
//	{5,1,1,0,0,0,(unsigned char *)"SCENE 1 DUTY",   		 0,	  			SceneFlashInit	,	SceneBack,SceneBack,SceneFlashTask}, 
//	{5,2,1,0,0,0,(unsigned char *)"SCENE 2 DUTY",	   		 0,					Scene2TaskInit	,									}, 
//	{5,3,1,0,0,0,(unsigned char *)"SCENE 3 DUTY",	   		 0,					Scene3TaskInit	,									}, 
//	{5,4,1,0,0,0,(unsigned char *)"IMPORT SCENE DUTY",   0,			ImportInit,		0,0,ImportTask		}, 
//	
//	{6,1,1,0,0,0,(unsigned char *)"AUTO DETECT",   "�Զ����"     },
//	{6,1,2,0,0,0,(unsigned char *)"DMX IN",        "DMX����"            },
//	{6,1,3,0,0,0,(unsigned char *)"Art-Net IN",     "Art-Net����"      },
//	{6,2,1,0,0,0,(unsigned char *)"DMX MODE ",     "DMXģʽ"           },
//	{6,2,2,0,0,0,(unsigned char *)"DMX ADDRES",      "DMX��ַ"             },
//	{6,3,1,0,0,0,(unsigned char *)"FAN AUTO",        "�Զ�"      },
//	{6,3,2,0,0,0,(unsigned char *)"FAN OFF",         "��"   },
//	{6,3,3,0,0,0,(unsigned char *)"DMX FAN",         "DMX����"          },
//	
//	{6,4,1,0,0,0,(unsigned char *)"DIM",	    "��������"},
//	{6,4,2,0,0,0,(unsigned char *)"TIME",	  "����ʱ��"},
//	{6,4,3,0,0,0,(unsigned char *)"ROTATE",	  "��Ļ��ת"},

//	{6,5,1,0,0,0,(unsigned char *)"UPDATE",	  "�̼�����"},
//	{6,5,2,0,0,0,(unsigned char *)"IMPORT",	            "�����ļ�" },
//	
//	{6,6,1,0,0,0,(unsigned char *)"ENGLISH",	          "Ӣ��"     },
//	{6,6,2,0,0,0,(unsigned char *)"CHINESE",	          "����"     },
//	
//	{6,7,1,0,0,0,(unsigned char *)"INFO",	              "�����豸"  },
//	{6,7,2,0,0,0,(unsigned char *)"DEAULTS",	          "�ָ���������"  },
//	{6,7,3,0,0,0,(unsigned char *)"MONITOR",	          "���"     },
//	
////�ļ�  //���һ��Ŀ¼ ����Ŀ¼ָ�붼Ϊ��
//	{6,1,1,1,0,0,(unsigned char *)"AUTO DETECT DUTY", 0,          AutoDetectINInit, 0,Save_Interface     },
//	{6,1,2,1,0,0,(unsigned char *)"DMX IN DUTY",      0,               DMXINTaskInit,0,Save_Interface    },
//	 
//	{6,1,3,1,0,0,(unsigned char *)"STATIC IP",        "��̬IP",                          },
//	{6,1,3,2,0,0,(unsigned char *)"DYNAMIC IP",       "�Զ���ȡ"                          },

//	{6,2,1,1,0,0,(unsigned char *)"DMX Mode DUTY",       0,					DMX_ModeTaskInit,  0,0,DMX_ModeTask, },
//	{6,2,2,1,0,0,(unsigned char *)"DMX ADDRES DUTY",     0,         DMX_AdressTaskInit,0,0,DMX_AdressTask,    },
//	{6,3,1,1,0,0,(unsigned char *)"FAN AUTO DUTY",       0,         FAN_AUTO_TaskInit, 0,SaveFanConfig  },
//	{6,3,2,1,0,0,(unsigned char *)"FAN OFF DUTY",        0,         FAN_OFF_TaskInit,  0,SaveFanConfig},
//	{6,3,3,1,0,0,(unsigned char *)"DMX FAN DUTY",        0,         FAN_DMX_TaskInit,  0,SaveFanConfig},
//	
//	{6,4,1,1,0,0,(unsigned char *)"BACKLITE DIM DUTY",	    0,         LCD_DIM_TaskInit,0,0, LCD_DIM_Task,  },
//	{6,4,2,1,0,0,(unsigned char *)"BACKLITE TIME DUTY",	    0,         LCD_TIM_TaskInit,0,0,LCD_TIM_Task,  },
//	{6,4,3,1,0,0,(unsigned char *)"ROTATE SCREEN DUTY",     0,         LCD_RotateTaskInit,0,0,LCD_RotateTask  },

//	{6,5,1,1,0,0,(unsigned char *)"FIRMWARE UPDATE DUTY",	     0,      firmware_update_init,0,firmware_update_Enter, firmware_update   },
//	{6,5,2,1,0,0,(unsigned char *)"IMPORT DUTY",	      0,         },
//	
//	{6,6,1,1,0,0,(unsigned char *)"ENGLISH DUTY",	      0,               EnglishTaskInit,0,0,ChineseTask      },
//	{6,6,2,1,0,0,(unsigned char *)"CHINESE DUTY",	      0,               ChineseTaskInit,0,0,ChineseTask},
//	
//	{6,7,1,1,0,0,(unsigned char *)"INFO DUTY",	        0,         InfoTaskInit,0,0,InfoTask},
//	{6,7,2,1,0,0,(unsigned char *)"DEFAULT DUTY",	      0,          },
//	{6,7,3,1,0,0,(unsigned char *)"MONITOR DUTY",	      0,         },
//// �弶
//	{6,1,3,1,1,0,(unsigned char *)"GATEWAY",          "����"         },
//	{6,1,3,1,2,0,(unsigned char *)"IP ADDRES",         "IP��ַ"    },
//	{6,1,3,1,3,0,(unsigned char *)"SUBNET MASK",      "��������"  },
//	{6,1,3,2,1,0,(unsigned char *)"DYNAMIC IP DUTY",    0,         },
////����
//	{6,1,3,1,1,1,(unsigned char *)"GATEWAY DUTY",       0,         },
//	{6,1,3,1,2,1,(unsigned char *)"IP ADDRES DUTY",     0,         },
//	{6,1,3,1,3,1,(unsigned char *)"SUBNET MASK DUTY",   0,         },
//	
//};

#if defined(ARRI_MENU)
struct MenuTyp MenuTab[] = {
//��������     |        �˵���
//��һ����     |
//Ϊ����� 
//��������
//Ϊ�˵��ȼ�   | 
//   �˵��ȼ�                     Ӣ����      ������        ָ�뺯��
	{1,0,0,0,0,0,(unsigned char *)"CCT MODE",   "CCTģʽ",  				},
	{2,0,0,0,0,0,(unsigned char *)"HSI MODE",   "HSIģʽ",    0,0,0,},
	{3,0,0,0,0,0,(unsigned char *)"GEL MODE",   "GELģʽ",					},
	{4,0,0,0,0,0,(unsigned char *)"RGB MODE",   "RGBģʽ",          },
	{5,0,0,0,0,0,(unsigned char *)"SCENE MODE", "����ģʽ",				  },
	{6,0,0,0,0,0,(unsigned char *)"SETTINGS",		"����"			        },
//����
	{1,1,0,0,0,0,(unsigned char *)"CCT DUTY",			0,       CCTInitTask,0,0,CCTTask},
	{2,1,0,0,0,0,(unsigned char *)"HSI DUTY",     0,       HSIInitTask,0,0,HSITask},
	{3,1,0,0,0,0,(unsigned char *)"GEL DUTY",			0,       GELInitTask,0,0,GELTask},
	{4,1,0,0,0,0,(unsigned char *)"RGB DUTY",			0,       RGBInitTask,0,0,RGBTask}, //������һ������
	{5,1,0,0,0,0,(unsigned char *)"SCENE 1",		 "����һ"								       }, 
	{5,2,0,0,0,0,(unsigned char *)"SCENE 2",		 "������"								      }, 
	{5,3,0,0,0,0,(unsigned char *)"SCENE 3",		 "������"											}, 
	{5,4,0,0,0,0,(unsigned char *)"IMPORT",		  	"���볡��"								  }, 
	
	{6,1,0,0,0,0,(unsigned char *)"IN SELECT",	  "����ѡ��"                    },
	{6,2,0,0,0,0,(unsigned char *)"DMX", "DMX"                    },
	{6,3,0,0,0,0,(unsigned char *)"FAN", "����"   },
	{6,4,0,0,0,0,(unsigned char *)"DISPLAY",      "��ʾ"    },
	{6,5,0,0,0,0,(unsigned char *)"USB",  "USB" },
	{6,6,0,0,0,0,(unsigned char *)"LANGUAGE",      "����"},
	{6,7,0,0,0,0,(unsigned char *)"SYSTEM",        "ϵͳ"     },
//����                             Ӣ����              ������           ָ�뺯��
	{5,1,1,0,0,0,(unsigned char *)"SCENE 1 DUTY",   		 0,	  			SceneFlashInit	,	SceneBack,SceneBack,SceneFlashTask}, 
	{5,2,1,0,0,0,(unsigned char *)"SCENE 2 DUTY",	   		 0,					SceneFlashInit	,	SceneBack,SceneBack,SceneFlashTask}, 
	{5,3,1,0,0,0,(unsigned char *)"SCENE 3 DUTY",	   		 0,					SceneFlashInit	,	SceneBack,SceneBack,SceneFlashTask}, 
	{5,4,1,0,0,0,(unsigned char *)"IMPORT SCENE DUTY",   0,			ImportInit,		0,0,ImportTask		}, 
	
	{6,1,1,0,0,0,(unsigned char *)"AUTO DETECT",   "�Զ����"     },
	{6,1,2,0,0,0,(unsigned char *)"DMX IN",        "DMX����"            },
	{6,1,3,0,0,0,(unsigned char *)"Art-Net IN",     "Art-Net����"      },
	{6,2,1,0,0,0,(unsigned char *)"DMX MODE ",     "DMXģʽ"           },
	{6,2,2,0,0,0,(unsigned char *)"DMX ADDRES",      "DMX��ַ"             },
	{6,3,1,0,0,0,(unsigned char *)"FAN AUTO",        "�Զ�"      },
	{6,3,2,0,0,0,(unsigned char *)"FAN OFF",         "��"   },
	{6,3,3,0,0,0,(unsigned char *)"DMX FAN",         "DMX����"          },
	
	{6,4,1,0,0,0,(unsigned char *)"DIM",	    "��������"},
	{6,4,2,0,0,0,(unsigned char *)"TIME",	  "����ʱ��"},
	{6,4,3,0,0,0,(unsigned char *)"ROTATE",	  "��Ļ��ת"},

	{6,5,1,0,0,0,(unsigned char *)"UPDATE",	  "�̼�����"},
	{6,5,2,0,0,0,(unsigned char *)"IMPORT",	            "�����ļ�" },
	
	{6,6,1,0,0,0,(unsigned char *)"ENGLISH",	          "Ӣ��"     },
	{6,6,2,0,0,0,(unsigned char *)"CHINESE",	          "����"     },
	
	{6,7,1,0,0,0,(unsigned char *)"INFO",	              "�����豸"  },
	{6,7,2,0,0,0,(unsigned char *)"DEAULTS",	          "�ָ���������"  },
	{6,7,3,0,0,0,(unsigned char *)"MONITOR",	          "���"     },
	
//�ļ�  //���һ��Ŀ¼ ����Ŀ¼ָ�붼Ϊ��
	{6,1,1,1,0,0,(unsigned char *)"AUTO DETECT DUTY", 0,          AutoDetectINInit, 0,Save_Interface     },
	{6,1,2,1,0,0,(unsigned char *)"DMX IN DUTY",      0,               DMXINTaskInit,0,Save_Interface    },
	 
	{6,1,3,1,0,0,(unsigned char *)"STATIC IP",        "��̬IP",                          },
	{6,1,3,2,0,0,(unsigned char *)"DYNAMIC IP",       "�Զ���ȡ"                          },

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
// �弶
	{6,1,3,1,1,0,(unsigned char *)"GATEWAY",          "����"         },
	{6,1,3,1,2,0,(unsigned char *)"IP ADDRES",         "IP��ַ"    },
	{6,1,3,1,3,0,(unsigned char *)"SUBNET MASK",      "��������"  },
	{6,1,3,2,1,0,(unsigned char *)"DYNAMIC IP DUTY",    0,         },
//����
	{6,1,3,1,1,1,(unsigned char *)"GATEWAY DUTY",       0,         },
	{6,1,3,1,2,1,(unsigned char *)"IP ADDRES DUTY",     0,         },
	{6,1,3,1,3,1,(unsigned char *)"SUBNET MASK DUTY",   0,         },
	
};
#else

struct MenuTyp MenuTab[] = {
//��������     |        �˵���
//��һ����     |
//Ϊ����� 
//��������
//Ϊ�˵��ȼ�   | 
//   �˵��ȼ�                     Ӣ����      ������        ָ�뺯��
	{1,0,0,0,0,0,(unsigned char *)"CCT MODE",   "CCTģʽ",		     CCTInitTask,0,0,CCTTask},
	{2,0,0,0,0,0,(unsigned char *)"HSI MODE",   "HSIģʽ",         HSIInitTask,0,0,HSITask},
	{3,0,0,0,0,0,(unsigned char *)"GEL MODE",   "GELģʽ",		     GELInitTask,0,0,GELTask},
	{4,0,0,0,0,0,(unsigned char *)"SCENE MODE", "����ģʽ",				  },
	{5,0,0,0,0,0,(unsigned char *)"SETTINGS",		"����"			        },
//����			
	{4,1,0,0,0,0,(unsigned char *)"FLASH",		 "��˸",	  						SceneFlashInit,	SceneBack,SceneBack,SceneFlashTask}, 
	{4,2,0,0,0,0,(unsigned char *)"SOS",		 		"SOS",	   						SceneSOSInit,	SceneBack,SceneBack,SceneSOSTask}, 							      
	{4,3,0,0,0,0,(unsigned char *)"STORM",		   "����",	   		 			SceneStormInit,	SceneBack,SceneBack,SceneStormTask},  
	{4,4,0,0,0,0,(unsigned char *)"IMPORT",		  	"���볡��",					ImportInit,		0,0,ImportTask}, 
	
	{5,1,0,0,0,0,(unsigned char *)"DMX", "DMX"                    },
	{5,2,0,0,0,0,(unsigned char *)"FAN", "����"   },
	{5,3,0,0,0,0,(unsigned char *)"DISPLAY",      "��ʾ"    },
	{5,4,0,0,0,0,(unsigned char *)"USB",  "USB" },
	{5,5,0,0,0,0,(unsigned char *)"LANGUAGE",      "����"},
	{5,6,0,0,0,0,(unsigned char *)"SYSTEM",        "ϵͳ"     },
//����                             Ӣ����              ������           ָ�뺯��

	{5,1,1,0,0,0,(unsigned char *)"DMX MODE ",     "DMXģʽ"  ,  DMX_ModeTaskInit,  0,0,DMX_ModeTask},
	{5,1,2,0,0,0,(unsigned char *)"DMX ADDRES",      "DMX��ַ" , DMX_AdressTaskInit,0,0,DMX_AdressTask}, 
	{5,2,1,0,0,0,(unsigned char *)"FAN AUTO",        "�Զ�" ,  FAN_AUTO_TaskInit, 0,SaveFanConfig},         
	{5,2,2,0,0,0,(unsigned char *)"FAN OFF",         "��"  ,   FAN_OFF_TaskInit,  0,SaveFanConfig},
	{5,2,3,0,0,0,(unsigned char *)"DMX FAN",         "DMX����" ,FAN_DMX_TaskInit,  0,SaveFanConfig},
	
	{5,3,1,0,0,0,(unsigned char *)"TIME",	  	"����ʱ��"},
	{5,3,2,0,0,0,(unsigned char *)"ROTATE",	  "��Ļ��ת"},

	{5,4,1,0,0,0,(unsigned char *)"UPDATE",	  "�̼�����",firmware_update_init,0,firmware_update_Enter, firmware_update},
	{5,4,2,0,0,0,(unsigned char *)"IMPORT",	            "�����ļ�" },
	
	{5,5,1,0,0,0,(unsigned char *)"Ӣ��" ,"ENGLISH",	          EnglishTaskInit,0,0,ChineseTask  },
	{5,5,2,0,0,0,(unsigned char *)"����" ,"CHINESE",	          ChineseTaskInit,0,0,ChineseTask},
	
	{5,6,1,0,0,0,(unsigned char *)"INFO",	              "�����豸" ,InfoTaskInit,0,0,InfoTask},
	{5,6,2,0,0,0,(unsigned char *)"DEAULTS",	          "�ָ���������" , DefaultInit,0,0,DefaultTask},
	{5,6,3,0,0,0,(unsigned char *)"MONITOR",	          "���"     ,  MonitorInit,0,0,MonitorTask},
	     
};

#endif

struct MenuTyp *FindParent(struct MenuTyp *menutyp)
{
	int i,j=0,k;
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//ȷ���Լ����ڲ�
	if(i==0)//���
	{
		return 0;
	}
	else
	{
		while(j<MAX_MENUS)//
		{                   
			for(k=0;k<i;k++)//����㿪ʼƥ��Ѱ��
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])//��һ��
				{
					break;
				}
			}
			if(k==i) //��ĸ�������Ŷ���ͬ
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
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//ȷ���Լ����ڲ�
	if(i==MAX_LEVES-1)//��ײ�
	{
		return 0;
	}
	else
	{
		while(j<MAX_MENUS)//��������
		{
			for(k=0;k<=i;k++)//����㿪ʼƥ��Ѱ��
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i+1]==1)&&(k==i+1))//��һ��
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
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//ȷ���Լ����ڲ�
	if(menutyp->leves[i]==1)//���Ҳ�
	{
		return 0;
	}
	i++;
	while(j<MAX_MENUS)
	{
		if(i==1)
		{
			if(MenuTab[j].leves[i-1]==menutyp->leves[i-1]-1)// ��
			{
					return &MenuTab[j];
			}
		}
		else
		{
			for(k=0;k<i;k++)//����㿪ʼƥ��Ѱ��
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i-1]==menutyp->leves[i-1]-1)&&(k==i-1))//��
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
	for(i=MAX_LEVES-1;menutyp->leves[i]==0;i--);//ȷ���Լ����ڲ�
	i++;
	while(j<MAX_MENUS)
	{
		if(i==1)
		{
			if(MenuTab[j].leves[i-1]==menutyp->leves[i-1]+1)//�Ҳ�
			{
					return &MenuTab[j];
			}
		}
		else
		{
			for(k=0;k<i;k++)//����㿪ʼƥ��Ѱ��
			{
				if(MenuTab[j].leves[k]!=menutyp->leves[k])
				{
					break;
				}
			}
			if((MenuTab[j].leves[i-1]==menutyp->leves[i-1]+1)&&(k==i-1))//�Ҳ�
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

//��ȡ�˵���ѡ���
unsigned char Menu_GetoptionNum( struct MenuTyp *menu_option)
{
	int i=MAX_LEVES-1;
	while(menu_option->leves[i]==0)
	{
		i--;
	}
	return menu_option->leves[i];
}



