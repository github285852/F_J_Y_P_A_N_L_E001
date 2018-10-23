#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "malloc.h"
#include "st7735s.h"
#include "stdlib.h"

u8 DMAING=0;
void SPI3_init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE );//SPI2时钟使能 	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //设置SPI单向或者双向的数据模式:SPI设置为单向
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设
	
//	SPI3_ReadWriteByte(0xAA);//启动传输		
	
	//DMA2 设置 SPI3TX 通道二
	                   //存储器地址自增   存储器读
	DMA2_Channel2->CCR =  DMA_CCR1_DIR | DMA_CCR1_MSIZE_0 | DMA_CCR2_TCIE;//开中断
	DMA2_Channel2->CPAR = SPI3_BASE + 0x0C; //外设地址
	
	SPI3->CR2 |= SPI_CR2_TXDMAEN;// 启动DMA缓存
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel2_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
	
	
	//DMA2_Channel2->CMAR = 
	//
	//DMA2_Channel2->CNDTR = 数目
  //DMA2_Channel2->CCR |= DMA_CCR1_EN;//使能通道
}

void SPI_SET_8(void)
{
	SPI3->CR1 &= ~SPI_CR1_SPE;//关闭SPI
	SPI3->CR1 &= ~SPI_CR1_DFF;
	SPI3->CR1 |= SPI_CR1_SPE;//打开SPI
}

void SPI_SET_16(void)
{
	SPI3->CR1 &= ~SPI_CR1_SPE;//关闭SPI
	SPI3->CR1 |= SPI_CR1_DFF;
	SPI3->CR1 |= SPI_CR1_SPE;//打开SPI 
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;		
	while(DMAING);
	while(DMA2_Channel2->CNDTR)
	{
	//	retry++;
	//	if(retry>200)return 0;
	}
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}		
	while((SPI3->SR&SPI_I2S_FLAG_BSY)|| ((SPI3->SR&SPI_I2S_FLAG_TXE)==0)); //检查指定的SPI标志位设置与否:发送缓存空标志位	
	SPI_SET_8();
	SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
	retry=0;
/*
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据
*/		
}

u8 SPI3_write_bytes(uint32_t *addr,u16 num)
{
	u8 retry=0;
	while(DMAING);
	while(DMA2_Channel2->CNDTR)
	{
		//retry++;
	//	if(retry>200)return 0;
	}
	while((SPI3->SR&SPI_I2S_FLAG_BSY)|| ((SPI3->SR&SPI_I2S_FLAG_TXE)==0)) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
	//	retry++;
	//	if(retry>200)return 0;
	}	
	while(DMA2_Channel2->CNDTR)
	{
	//	retry++;
	//	if(retry>200)return 0;
	}
	SPI_SET_8();
	DMA2_Channel2->CCR &= ~DMA_CCR1_EN;//关闭通道
	DMA2_Channel2->CCR |=  DMA_CCR1_DIR | DMA_CCR1_MINC;//地址自增
	DMA2_Channel2->CMAR = (uint32_t) addr;
	DMA2_Channel2->CNDTR = num;
	DMA2_Channel2->CCR |= DMA_CCR1_EN;//使能通道
	DMAING = 1;
	return 1;
}

u8 SPI3_write_half_words(uint32_t *addr,u16 num)
{
	u8 retry=0;
	while(DMAING);
	while(DMA2_Channel2->CNDTR)
	{
	//	retry++;
	//	if(retry>200)return 0;
	}
	while((SPI3->SR&SPI_I2S_FLAG_BSY)|| ((SPI3->SR&SPI_I2S_FLAG_TXE)==0)) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		//retry++;
		//if(retry>200)return 0;
	}	
	while(DMA2_Channel2->CNDTR)
	{
	//	retry++;
	//	if(retry>200)return 0;
	}
	SPI_SET_16();
	DMA2_Channel2->CCR &= ~DMA_CCR1_EN;//关闭通道
	DMA2_Channel2->CCR |=  DMA_CCR1_DIR | DMA_CCR1_MINC | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0;//16位，地址自增
	DMA2_Channel2->CMAR = (uint32_t) addr;
	DMA2_Channel2->CNDTR = num;
	DMA2_Channel2->CCR |= DMA_CCR1_EN;//使能通道
	DMAING = 1;
	return 1;
}

//连发送同一个16位的数
u8 SPI3_send_same_half_word(uint32_t *addr,u16 num)
{
	u8 retry=0;
	while(DMAING);
	while(DMA2_Channel2->CNDTR)
	{
		//retry++;
	//	if(retry>200)return 0;
	}
	while((SPI3->SR&SPI_I2S_FLAG_BSY)|| ((SPI3->SR&SPI_I2S_FLAG_TXE)==0)) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		//retry++;
		//if(retry>200)return 0;
	}	
	while(DMA2_Channel2->CNDTR)
	{
		//retry++;
	//	if(retry>200)return 0;
	}
	SPI_SET_16();
	DMA2_Channel2->CCR &= ~DMA_CCR1_EN;//关闭通
	DMA2_Channel2->CCR |=  DMA_CCR1_DIR | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0; //存储16位，
	DMA2_Channel2->CMAR = (uint32_t) addr;
	DMA2_Channel2->CNDTR = num;
	DMA2_Channel2->CCR |= DMA_CCR1_EN;//使能通道
	DMAING = 1;
	return 1;
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI3,ENABLE); 
} 
extern u16 *LCD_BUF;
void MenuBuf_free(void);
void DMA2_Channel2_IRQHandler(void)
{
	if(DMA2->ISR&DMA_ISR_TCIF2)
	{

		DMAING = 0;
		//CS = 1;
		DMA2->IFCR |= DMA_IFCR_CTCIF2;//清楚传输完成标志
	}
}