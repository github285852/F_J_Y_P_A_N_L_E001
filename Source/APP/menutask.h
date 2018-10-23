#ifndef __MENUTASK_H
#define __MENUTASK_H



void HSITask(void);
void HSIInitTask(void);

void CCTTask(void);
void CCTInitTask(void);

void RGBInitTask(void);
void RGBTask(void);


void Scene1TaskInit(void);
void Scene2TaskInit(void);
void Scene3TaskInit(void);
void ImportSceneTaskInit(void);
void Scene1Task(void);

void DMXINTaskInit(void);
void Art_NetINTask(void);
void AutoDetectINInit(void);
void Save_Interface(void); //按确认键执行

void DMX_8_BitsTaskInit(void);
void DMX_16_BitsTaskInit(void);
void DMX_AdressTaskInit(void);
void DMX_AdressTask(void);
void DMX_ModeTask(void);
void DMX_ModeTaskInit(void);

void FAN_AUTO_TaskInit(void);
void FAN_OFF_TaskInit(void);
void FAN_DMX_TaskInit(void);
void SaveFanConfig(void);

void LCD_DIM_TaskInit(void);
void LCD_DIM_Task(void);
void LCD_TIM_TaskInit(void);
void LCD_TIM_Task(void);
void LCD_RotateTaskInit(void);
void LCD_RotateTask(void);

void InfoTask(void);
void InfoTaskInit(void);

void EnglishTaskInit(void);
void ChineseTaskInit(void);
void ChineseTask(void);
	
void firmware_update_init(void);
void firmware_update(void);
void firmware_update_Enter(void);

#endif




