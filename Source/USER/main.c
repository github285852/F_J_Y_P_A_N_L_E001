#include "includes.h"

void timer4_init(u16 period,u16 Prescaler);
void AlgorithmTest(void);

CONFIG default_data = 
{
	.lcd.rotate = 1,
	.lcd.dim = 80,
	.lcd.tim = 20,
	.dmx.addr = 1,
	.dmx._16bits = 0,
	.dmx.mode = 1,
	.english = 1,
};

void param_init(void)
{
	Sys.save_cnt = 20*Sys.Config.lcd.tim;
	Sys.lcd_back_on = 1;
	Sys.menu_mask = 0;
	if(Sys.Config.lightmode>3)
	{
		Sys.Config.lightmode = 0;
	}
}

void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB
	
	//�����ͱ�����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = 0xFFFF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���óɸ�������
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin = 0xEF00;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	
//LCD	 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�JATGʹ��SWD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOB
	//CS = 0;
	LCD_BACK =0;
//tm1289
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);
	PBout(9) = 1;
//IIC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //��©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
	PBout(6) = 1;
	PBout(7) = 1;
	delay_ms(100);
}

SYS Sys;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;

int main(void)
{
	int i;
	Picture screen;
	SCB->VTOR = FLASH_BASE | STM32_FLASH_APP_BASE; /* Vector Table Relocation in Internal FLASH. SystemInit() ʱ�Ὣ�ж������������ */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Configuration();
	GPIO_init();
	LoadConfig();
	Debug_init();
	my_mem_init(SRAMIN);//��ʼ���ڲ��ڴ�
	ADC1Init();
	dmx512_init();
	uart3_init(250000);
	Lcd_Init();
  color_light_init();
	LCD_Clear(BLACK);
	SceneTimerInit();
	screen.data = (u16*)(&gImage_filmgear_data[0]);
	screen.h = 128;
	screen.w = 160;
	LCD_Fill_Picture(0,0,screen);
	delay_ms(20);
	LCD_BACK = 1;//�򿪱���
	Menu_init();
	param_init();
	AllLedPowerOff();
	FAN_OUT(800);
	delay_ms(1000);
  /* Init Host Library */
  USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS  
            USB_OTG_FS_CORE_ID,
#else 
            USB_OTG_HS_CORE_ID,
#endif 
            &USB_Host,
            &USBH_MSC_cb, 
            &USR_cb);
	//��ʾ
	MenuBuf_malloc();
	MenuPic.data = MENU_BUF;
	GUI_DisplayInit();
	while(DMAING);
	MenuBuf_free();
	timer4_init(100,71);//LCD����PWM
	ec11_init();//���ں��棬��Ȼ������
	
	AlgorithmTest();
	
	//IWDG_Init(4,625);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	
  while(1)
  {		
		Duty_Loop();
  }
  return 0;
}


