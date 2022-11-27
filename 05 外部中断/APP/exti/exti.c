#include "exti.h"
#include <LED.h>
#include <SysTick.h>

/*
*  中断通道：EXTI1, EXTI2, EXTI3, EXTI4, EXTI9_5, EXTI15_10
*
*  中断线：EXTI_Line0 ~ EXTI_Line15
*/

void Exti_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  // GPIO
	NVIC_InitTypeDef NVIC_InitStructure;  // 中断优先级
	EXTI_InitTypeDef EXTI_InitStructure;  // 外部中断
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	        //开启 AFIO 时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//选择GPIO E4 管脚用作外部中断线路
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;		//IO口 4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉 输入
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	    //GPIOE 初始化
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;	// EXTI0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	    // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);                 // 根据指定的参数初始化VIC寄存器
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	// 选择中断线  4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	    // 中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	// 下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  // 中断使能
	EXTI_Init(&EXTI_InitStructure);	 //  根据指定参数初始化EXTI
}


void EXTI4_IRQHandler()	// 外部中断4函数
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)	// 判断中断线4有无中断发生
	{
		delay_ms(10);
		LED1=~LED1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);	// 清除中断标志位
}
