#include "debug.h"
#include "stm32f10x.h"
#include "string.h"
#include "includes.h"


void Debug_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DEBUG_USART,ENABLE);//ʹ��DMX_USARTʱ��
//	#if GPIO_REMAP
//	GPIO_PinRemapConfig(GPIO_Remap_DMX_USART,ENABLE);
//	#endif

	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin =	DEBUG_TX_PIN; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(DEBUG_TX_GPIO, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = DEBUG_RX_PIN;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(DEBUG_RX_GPIO, &GPIO_InitStructure);//��ʼ��GPIOA.10  
		
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DEBUG_USART,ENABLE);//��λ����
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_DEBUG_USART,DISABLE);//ֹͣ��λ
 
	USART_InitStructure.USART_BaudRate = 115200;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_2;//����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(DEBUG_USART, &USART_InitStructure);  //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);//���ж�
	USART_Cmd(DEBUG_USART, ENABLE);                    //ʹ�ܴ��� 
	
}


#define USART_REC_LEN  			200  
u8  USART_RX_BUF[USART_REC_LEN]; 
u16 USART_RX_STA;  

void uart_resiver(u8 Res)
{
	if((USART_RX_STA&0x8000)==0)
	{
		if(USART_RX_STA&0x4000)
		{
			if(Res!=0x0a)
			{
				USART_RX_STA=0;
			}
			else 
			{
				USART_RX_STA|=0x8000;
			}
		}
		else 
		{	
			if(Res==0x0d)
				USART_RX_STA|=0x4000;
			else
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))
					USART_RX_STA=0;  
			}		 
		}
	} 
}

void debug_send_str(unsigned char *str)
{
	while(*str != 0)
	{
		while((DEBUG_USART->SR&0X40)==0)
		{}
    DEBUG_USART->DR = *str;     
		str++;
	}		
}

/*
FUNCTION:�ж����ַ���������
INPUT:buf,Ҫ�б���ַ���cmd �ж������params,��������
OUTPUT:param,ȡ�õĲ��� ָ��
RETURN��1,�Ǹ�����
�������ֱ�Ӹ�����������֮����','����
*/
int IsCmd(unsigned char *buf,char *cmd,u8 params,float *param)
{
	unsigned short len;
	char *p;
	int i = 0;
	p = strstr((const char*)buf,(const char*)cmd);
	if(p != NULL)
	{	
		p += strlen(cmd);
		while(i<params)
		{
			param[i] = atof((const char*)p);
			p = strstr((const char*)p,(const char*)",");
			if( p!= NULL)
			{
				i++;
				p++;
			}
			else
			{
				break;
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void uart_duty(void)
{
	float param_f[LED_CHS];
	unsigned short param_u16[LED_CHS];
	int i;
	if(USART_RX_STA&0x8000)
	{
		USART_RX_BUF[USART_RX_STA&0X3FFF]=0; //�ַ���������־
		if(IsCmd(USART_RX_BUF,"Seach device",0,NULL))
		{
			DeviceACK("%s,ID=%s\r\n",DEVICE_NAME,DEVICE_VER);
		}
		if(IsCmd(USART_RX_BUF,"IOUT:",LED_CHS,param_f))
		{
			for(i=0;i<LED_CHS;i++)
				param_u16[i] = (unsigned short)param_f[i];
			DMXChanleDataRefresh(0,param_u16);
			DeviceACK("IOUT OK\r\n");
		}
		if(IsCmd(USART_RX_BUF,"Start check",0,NULL))
		{
			Sys.check = 1;
			DeviceACK("Start check\r\n");
		}
		if(IsCmd(USART_RX_BUF,"End check",0,NULL))
		{
			Sys.check = 0;
			DeviceACK("End check\r\n");
		}
		USART_RX_STA=0;  
	}
}


void DEBUG_USART_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_NE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_NE); 
		USART_ReceiveData(DEBUG_USART);    
	}
	else if(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_FE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_FE); 
		USART_ReceiveData(DEBUG_USART);    
	}
	else if(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_ORE)==SET)
	{
		//USART_ClearFlag(DMX_USART,USART_FLAG_ORE); 
		USART_ReceiveData(DEBUG_USART);    
	}	
	else if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		res = USART_ReceiveData(DEBUG_USART); 	//��ȡ���յ�������
		uart_resiver(res);
	}
} 



void Debug_printf(char* fmt,...)  
{
//	#if DEBUG
	u8 *pbuf,*p;
	va_list ap;
	pbuf = malloc(50);
	if(!pbuf)									
	{
		return ;
	}
	va_start(ap,fmt);
	vsprintf((char*)pbuf,fmt,ap);
	va_end(ap);			
  p = pbuf;
	debug_send_str(p);
	free(pbuf);
//	#endif
}



