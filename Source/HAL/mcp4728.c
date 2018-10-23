#include "mcp4728.h"
#include "sys.h"
#include "myiic.h"
#include "st7735s.h"
#include "sys.h"

unsigned int mcp4728_data[DAC_CHALE]={1000,2000,3000,4000,1000,2000,3000,4000,1000,2000,3000,4000};
int mcp4728_general_call_software_update(void)
{
	int re;
	IIC_Start();
	IIC_Send_Byte(0x00);//broadcast
	if(IIC_Wait_Ack())
	{
		return 1;
	}
	IIC_Send_Byte(0x08); // eneral_call_software_update
	if(IIC_Wait_Ack())
	{
		return 1;
	}
	IIC_Stop();
	return 0;
}

int mcp4728_general_call_reset(void)
{
	int re;
	IIC_Start();
	IIC_Send_Byte(0x00);//broadcast
	if(IIC_Wait_Ack())
	{
		return 1;
	}
	IIC_Send_Byte(0x06); // eneral_call_software_update
	if(IIC_Wait_Ack())
	{
		return 1;
	}
	IIC_Stop();
	return 0;
}

unsigned char	mcp4728_general_call_address(unsigned char nack)
{
	unsigned char re;
	LDAC_H;
	IIC_Start();
	IIC_Send_Byte(0x00);//broadcast
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Send_Byte(0x0C); // eneral_call_software_update
	LDAC_L;
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Start();
	IIC_Send_Byte(0xC1);
	IIC_Wait_Ack();
	re = IIC_Read_Byte(nack);
	IIC_Stop();
	return re;
}

unsigned char mcp4728_set_Vrefx(unsigned char addr,unsigned char mode)
{
	u8 temp_byte = 0x80|(0x0F&mode);
	IIC_Start();
	IIC_Send_Byte(MCP4728_ADDR|(addr<<1));
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Send_Byte(temp_byte);
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Stop();
}

unsigned char mcp4728_set_Gx(unsigned char addr,unsigned char mode)
{
	u8 temp_byte = 0xC0|(0x0F&mode);
	IIC_Start();
	IIC_Send_Byte(MCP4728_ADDR|(addr<<1));
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Send_Byte(temp_byte);
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Stop();
}

void mcp4728_init(void)
{
	int i;
	for(i=0;i<MCP4728_NUMS;i++)
	{
		mcp4728_set_Vrefx(i,0x0f);
		mcp4728_set_Gx(i,0x0f);
	}
}
int mcp4728_fast_write_comand(unsigned char addr,unsigned char *cmd,unsigned int *data)
{
	unsigned char temp_byte;
	int i;
	IIC_Start();
	IIC_Send_Byte(MCP4728_ADDR|(addr<<1));
	if(IIC_Wait_Ack())
		return 0;
	for(i=0;i<4;i++)
	{
		temp_byte = ((cmd[i]&0x03)<<4)|((data[i]>>8)&0x0F);
		IIC_Send_Byte(temp_byte);
		if(IIC_Wait_Ack())
			return 0;
		IIC_Send_Byte((unsigned char)data[i]);
		if(IIC_Wait_Ack())
			return 0;
	}
	IIC_Stop();
}


int mcp4728_change_address(unsigned char addr,unsigned char new_addr)
{
	unsigned char temp_byte;
	LDAC_H;
	IIC_Start();
	IIC_Send_Byte(MCP4728_ADDR|(addr<<1));
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	temp_byte = ((addr<<2)|0x60)|0x01;
	IIC_Send_Byte(temp_byte);
	delay_us(50);
	LDAC_L;
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	temp_byte = ((new_addr<<2)|0x60)|0x02;
	IIC_Send_Byte(temp_byte);
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	temp_byte = ((new_addr<<2)|0x60)|0x03;
	IIC_Send_Byte(temp_byte);
	if(IIC_Wait_Ack())
	{
		return 0;
	}
	IIC_Stop();
	return 1;
}

void mcp4728_update_chanle(unsigned int *buf)
{
	int i,j;
	unsigned int ch_buf[DAC_CHALE];
	u8 cmd[4] = {0,0,0,0};///注意这个命令
	for(i=0;i<MCP4728_NUMS;i++)
	{
		for(j=0;j<4;j++)
		{
			ch_buf[i*4+j] = buf[i*4+3-j];
		}
	}
	for(i=0;i<MCP4728_NUMS;i++)
	{
		mcp4728_fast_write_comand(i,cmd,&ch_buf[i*4]);
	}
	mcp4728_general_call_software_update();
}

void mcp4728_read(unsigned char addr,unsigned char *buf,unsigned char len)
{
	int i;
	IIC_Start();
	IIC_Send_Byte(MCP4728_ADDR|(addr<<1)|0x01);//读
	if(IIC_Wait_Ack())
	{
		return ;
	}
	for(i=0;i<len-1;i++)
	{
		buf[i] = IIC_Read_Byte(1);//应达
	}
	buf[i] = IIC_Read_Byte(0);//不应
	IIC_Stop();
}

u8 addr;
unsigned char test_buf[4];
void mcp4728_test(void)
{
	//mcp4728_general_call_reset();
	//mcp4728_change_address(0,2);
	//delay_ms(100);
	///mcp4728_read(2,test_buf,3);
	//LCD_ShowNum(50,50,addr,4,16);
//	mcp4728_set_addr(0);
	addr = mcp4728_general_call_address(0);
	mcp4728_update_chanle(mcp4728_data);
	LCD_ShowNum(50,50,addr,4,16);
}

//需要在板子上用跳线帽练接相应的引脚

int mcp4728_set_addr(unsigned char new_addr)
{
	int i;
	for(i=0;i<8;i++)
	{
		if(mcp4728_change_address(i,new_addr))
		{
			return 0;
		}
		else
		{
			delay_ms(100);
		}
	}
	return 1;
}

//int mcp4728_set_addr(unsigned char new_addr)
//{
//	unsigned char ep_addr,reg_addr;
//	reg_addr = mcp4728_general_call_address(0);
//	if(reg_addr&0x10==0)
//	{
//		//读地址错误。
//		return 1;
//	}
//	ep_addr = reg_addr & 0xE0;
//	reg_addr <<= 4;
//	if(reg_addr!=ep_addr)
//	{
//		//读地址错误。
//		return 1;
//	}
//	reg_addr >>= 5;
//	//delay_ms(1000);

//	mcp4728_change_address(reg_addr,new_addr);	
//	reg_addr = mcp4728_general_call_address(0);

//	if(reg_addr&0x10==0)
//	{
//		//读地址错误。
//		return 1;
//	}
//	ep_addr = reg_addr & 0xE0;
//	reg_addr <<= 4;
//	if(reg_addr!=ep_addr)
//	{
//		//读地址错误。
//		return 1;
//	}
//	reg_addr >>= 5;
//	
//	if(reg_addr==new_addr)
//	{
//		return 0;
//	}
//	else
//	{
//			return 1;
//	}
//	
//}
