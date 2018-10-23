#ifndef __UCS8903_H
#define __UCS8903_H

#define DIN_H   	PBout(9) = 1   //GPIOB->BSRR |= 0x200
#define DIN_L			PBout(9) = 0   //GPIOB->BRR  |= 0x200

#define UCS8903_NUMS			8

void ucs8903_init(void);
void ucs8903_update_chanle(unsigned int *buf);



extern unsigned int ucs8903_data[24];



#endif

