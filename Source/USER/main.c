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
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB
	
	//按键和编码器
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = 0xFFFF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//
	
	GPIO_InitStructure.GPIO_Pin = 0xEF00;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	
//LCD	 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JATG使能SWD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB
	//CS = 0;
	LCD_BACK =0;
//tm1289
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);
	PBout(9) = 1;
//IIC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
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
	SCB->VTOR = FLASH_BASE | STM32_FLASH_APP_BASE; /* Vector Table Relocation in Internal FLASH. SystemInit() 时会将中断入口重新设置 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Configuration();
	GPIO_init();
	LoadConfig();
	Debug_init();
	my_mem_init(SRAMIN);//初始化内部内存
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
	LCD_BACK = 1;//打开背光
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
	//显示
	MenuBuf_malloc();
	MenuPic.data = MENU_BUF;
	Set_display_option = 3; //不显示选项
	GUI_DisplayInit();
	while(DMAING);
	MenuBuf_free();
	timer4_init(100,71);//LCD背光PWM
	ec11_init();//放在后面，不然会死机
	
	AlgorithmTest();
	
	//IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s	
  while(1)
  {		
		Duty_Loop();
  }
  return 0;
}


