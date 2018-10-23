#ifndef __MENU_H
#define __MENU_H

//创建目录树使用
#define MAX_LEVES	6

//菜单显示参数
#define  DIS_MAX_LINE			3  //最大显示几行
#define  MENU_W				140   //菜单宽度
#define  MENU_H       100		//菜单高度

#define RECT_H					20  //每行的高度
#define RECT_W					100  //每行的宽度
#define RECT_INTERVAL   5 //每行之间的间隔

#define FIS_RECT_POS_H	12  //第一行在菜单界面的坐标
#define FIS_RECT_POS_W	((MENU_W-RECT_W)/2)

#define MENU_POS_X	((LCD_W-MENU_W)/2)
#define MENU_POS_Y  20

#define NUM_OFFSET_X   0//选项号的偏移
#define NUM_OFFSET_Y   4

#define STRING_OFFSET 		16

#define MENU_BACK_COLOR				BLACK
#define SELECT_COLOR					DGRAY
#define OPTION_COLOR					LIGHTGRAY      

struct MenuTyp
{
	unsigned char leves[MAX_LEVES];//第1层索引
	unsigned char *EnName;//英文名
	unsigned char *ChName;//中文名
	void (*pMenuTaskInit)(void);//指向菜单任务初始化函数指针,第一次进入
	void (*pMenuTaskEnd)(void);//指向菜单任务结束函数指针
	void (*pTaskKeyEnter)(void);//指向菜单任务按键处理函数指针，确认
	void (*pMneuTask)(void); //菜单循环
	struct MenuTyp *pParent;//指向上层菜单指针
	struct MenuTyp *pChild;//指向子菜单的指针
	struct MenuTyp *pRight;//指向右菜单的指针
	struct MenuTyp *pLeft;//指向左菜单的指针
	int pos;
};

void CreateMenu(void);
extern struct MenuTyp *InitMenup;
unsigned char Menu_GetoptionNum( struct MenuTyp *menu_option);

extern int menu_pos;
#endif



