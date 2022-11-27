#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"

void SysTick_Init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);


#endif
