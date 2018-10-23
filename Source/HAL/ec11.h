#ifndef   __EC11_H
#define		__EC11_H


#define ReadT1APin()		GPIOE->IDR&0x02
#define ReadT1BPin()		GPIOE->IDR&0x04

#define ReadT2APin()		GPIOE->IDR&0x10
#define ReadT2BPin()		GPIOE->IDR&0x20


#define ReadT3APin()		GPIOC->IDR&0x2000
#define ReadT3BPin()		GPIOC->IDR&0x4000





void ec11_init(void);
void ecll_speed_duty(void);

extern long ec11_pos[3];
extern unsigned char ec11_check;
extern int Ec11Speed[3];
extern float EC11_speed;









#endif





