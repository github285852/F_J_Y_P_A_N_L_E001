#ifndef __UART3_H
#define __UART3_H


void uart3_init(u32 baudrate);
void DMX1_Send(u8 *buf,u16 len);


extern unsigned char DMX1_TX_BUF[512];


#endif


