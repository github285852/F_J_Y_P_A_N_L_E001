#ifndef __MCP4728_H
#define __MCP4728_H


#define LDAC_H		PBout(9) = 1
#define LDAC_L		PBout(9) = 0

#define MCP4728_ADDR 0xC0
#define MCP4728_NUMS	6

#define DAC_CHALE			24 //6*4


int mcp4728_general_call_software_update(void);
unsigned char	mcp4728_general_call_address(unsigned char nack);
int mcp4728_fast_write_comand(unsigned char addr,unsigned char *cmd,unsigned int *data);
void mcp4728_test(void);
int mcp4728_set_addr(unsigned char new_addr);
void mcp4728_init(void);
void mcp4728_update_chanle(unsigned int *buf);

extern unsigned int mcp4728_data[DAC_CHALE];

#endif






