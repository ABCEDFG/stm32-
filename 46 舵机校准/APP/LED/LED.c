#include "led.h"

void LED_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO初始化结构体
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//开启 GPIOB 时钟
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;		//IO口 5
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//传输速率 50MHz
		GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB 初始化
		
		//GPIO_SetBits(GPIOB,GPIO_Pin_5);	//初始化GPIOB 5
}
