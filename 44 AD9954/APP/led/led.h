#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED1_PORT 			GPIOA   
#define LED1_PIN 			GPIO_Pin_8
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOA

#define LED2_PORT 			GPIOD   
#define LED2_PIN 			GPIO_Pin_2
#define LED2_PORT_RCC		RCC_APB2Periph_GPIOD


#define LED1 PAout(8)  	
#define LED2 PDout(2)  	


void LED_Init(void);


#endif
