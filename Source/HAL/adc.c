#include "stm32f10x_adc.h"
#include "stm32f10x.h"
#include "includes.h"
#include "adc.h"
#include "math.h"

u16 ADC1_BUFFER[MEAN_NUMS]={0,0,0};
u32 adc_data;
void ADC1Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//DMA_InitTypeDef DMA_InitStructure;
  //DMA_Channel_TypeDef DMA_Channel1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  //?? DMA?? 
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	ADC_DeInit(ADC1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);//PA.4Init  //
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);//PA.3Init  //	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//	??????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//???????????? 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC ????? 
	ADC_InitStructure.ADC_NbrOfChannel = 1;  //通道数目 
	ADC_Init(ADC1, &ADC_InitStructure);          //???????????? ADCx   
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_239Cycles5); 
 
	ADC_DMACmd(ADC1,ENABLE);//??ADC1?DMA
	ADC_Cmd(ADC1, ENABLE);                          //????? ADC1 
	ADC_ResetCalibration(ADC1);                          //??????    
	while(ADC_GetResetCalibrationStatus(ADC1));  //???????? 
	ADC_StartCalibration(ADC1);                          //?? AD?? 
	while(ADC_GetCalibrationStatus(ADC1));          //?????? 

	//DMA1_Channel1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
	
	DMA1_Channel1->CPAR = (u32)&ADC1->DR;
	DMA1_Channel1->CMAR = (u32)ADC1_BUFFER;
	
	DMA1_Channel1->CCR = 0x15A0;    //16位，外设地址不变，存储递增，必须先关闭
	DMA1_Channel1->CNDTR = MEAN_NUMS;   //传输100次
	DMA1_Channel1->CCR |= 0x01;     //开启传输
	DMA1_Channel1->CCR |= 0x02; //开启DMA传输中断
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //使能软件转换功能   
}


const float Rp=10000.0; //10K
const float T2 = (273.15+25.0);//T2
const float Bx = 3435.0;//B
const float Ka = 273.15;
float Get_Temp(void)
{
	float Rt;
	float temp;
	//Rt = Get_TempResistor();
	Rt = 10*adc_data*1000/(4096-adc_data);
	//like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),  
	temp = Rt/Rp;
	temp = log(temp);//ln(Rt/Rp)
	temp/=Bx;//ln(Rt/Rp)/B
	temp+=(1/T2);
	temp = 1/(temp);
	temp-=Ka;
	return temp;
} 


#define A  0.01
void DMA1_Channel1_IRQHandler(void)
{
	int i;
	u8 temp;
  static u32 last_data = 0;
	u32 temp_data=0;
	if(DMA1->ISR&0x02)//DMA传输完成
	{
		for(i=0;i<100;i++)
		{
			temp_data += ADC1_BUFFER[i];
		}
		adc_data = temp_data/MEAN_NUMS; 
		adc_data = A*last_data + adc_data*(1-A);//一阶低通滤波
		last_data = adc_data;
		Sys.panle_t = Get_Temp();
		DMA1->IFCR|= 0x02;//清楚标志
	}
}

