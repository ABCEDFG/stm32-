#ifndef _WS2812B_H
#define _WS2812B_H


#include "system.h"


#define  WS2812B_PORT 			GPIOB   
#define  WS2812B_PIN 			GPIO_Pin_9
#define  WS2812B_PORT_RCC		RCC_APB2Periph_GPIOB
#define  WS2812B                PBout(9)  


void Ws2812b_Init(void);
void Ws2812b_WriteByte(u32 data);
void Ws2812b_Write(u32 *data, u8 len);

#endif

