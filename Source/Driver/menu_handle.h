#ifndef __MENU_HANDLE_H
#define __MENU_HANDLE_H

#include "graphical.h"


#define Enter_Key			0x01
#define Up_Key				0x02
#define Down_Key			0x03
#define Back_Key			0x04

typedef enum
{
	SelectMenu,
	HandleGui
}MenuState;


void MenuKeyTask(void);
void MenuSelectDisplay(void);
void HeadDisplay(char *p);
void MenuTask(void);

void MenuBuf_malloc(void);
void MenuBuf_free(void);

void Menu_init(void);
extern void (*MenuDuty)(void);
extern unsigned char menu_key;
extern Picture MenuPic;
extern RECT Rect[DIS_MAX_LINE];

extern struct MenuTyp *current_menu;
extern MenuState menu_state;
extern u16 MENU_BUF[MENU_W*MENU_H];
//extern u16 *MENU_BUF;

#endif


