#ifndef _board_H
#define _board_H

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"


void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
