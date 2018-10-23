#ifndef __LEDPOWER_H
#define __LEDPOWER_H

#include "color_light.h"

void LedPowerOut(RGB  *ledk,float dim,unsigned char pixel);
void AllLedPowerOut(RGB  *ledk,float dim);
void LedPowerOff(unsigned char pixel);
void AllLedPowerOff(void);


#endif


