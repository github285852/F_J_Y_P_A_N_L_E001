#ifndef __TM1829_H
#define __TM1829_H


#define DIN_H   	PBout(9) = 1   //GPIOB->BSRR |= 0x200
#define DIN_L			PBout(9) = 0   //GPIOB->BRR  |= 0x200

#define TM1289_NUMS			4

void tm1289_send_24bits(unsigned long data);
void tm1289_update_chanle(u8 *buf);


extern u8 tm1829_data[12];
#endif







