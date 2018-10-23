#ifndef __MENU_H
#define __MENU_H

//����Ŀ¼��ʹ��
#define MAX_LEVES	6

//�˵���ʾ����
#define  DIS_MAX_LINE			3  //�����ʾ����
#define  MENU_W				140   //�˵����
#define  MENU_H       100		//�˵��߶�

#define RECT_H					20  //ÿ�еĸ߶�
#define RECT_W					100  //ÿ�еĿ��
#define RECT_INTERVAL   5 //ÿ��֮��ļ��

#define FIS_RECT_POS_H	12  //��һ���ڲ˵����������
#define FIS_RECT_POS_W	((MENU_W-RECT_W)/2)

#define MENU_POS_X	((LCD_W-MENU_W)/2)
#define MENU_POS_Y  20

#define NUM_OFFSET_X   0//ѡ��ŵ�ƫ��
#define NUM_OFFSET_Y   4

#define STRING_OFFSET 		16

#define MENU_BACK_COLOR				BLACK
#define SELECT_COLOR					DGRAY
#define OPTION_COLOR					LIGHTGRAY      

struct MenuTyp
{
	unsigned char leves[MAX_LEVES];//��1������
	unsigned char *EnName;//Ӣ����
	unsigned char *ChName;//������
	void (*pMenuTaskInit)(void);//ָ��˵������ʼ������ָ��,��һ�ν���
	void (*pMenuTaskEnd)(void);//ָ��˵������������ָ��
	void (*pTaskKeyEnter)(void);//ָ��˵����񰴼�������ָ�룬ȷ��
	void (*pMneuTask)(void); //�˵�ѭ��
	struct MenuTyp *pParent;//ָ���ϲ�˵�ָ��
	struct MenuTyp *pChild;//ָ���Ӳ˵���ָ��
	struct MenuTyp *pRight;//ָ���Ҳ˵���ָ��
	struct MenuTyp *pLeft;//ָ����˵���ָ��
	int pos;
};

void CreateMenu(void);
extern struct MenuTyp *InitMenup;
unsigned char Menu_GetoptionNum( struct MenuTyp *menu_option);

extern int menu_pos;
#endif



