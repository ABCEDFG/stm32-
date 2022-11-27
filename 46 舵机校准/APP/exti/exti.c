#include <exti.h>
#include <SysTick.h>
#include <key.h>
#include <LED.h>

void Exti_Init()	//本程序无开启时钟，因为LED中已开启
{
	 NVIC_InitTypeDef NVIC_InitStructure;
	 EXTI_InitTypeDef EXTI_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//开启 AFIO 时钟SSS
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);	//选择GPIO A0 管脚用作外部中断线路
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);	//选择GPIO E3 管脚用作外部中断线路
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//选择GPIO E4 管脚用作外部中断线路
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;	//EXTI0中断通道
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	//子优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;	//EXTI3中断通道
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	//子优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;	//EXTI3中断通道
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//子优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 
	 EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//选择中断线 0	
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//中断模式	
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;	//上升沿触发		
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;	//中断线使能
	 EXTI_Init(&EXTI_InitStructure);	//根据指定参数初始化EXTI
	 
	 EXTI_InitStructure.EXTI_Line=EXTI_Line3|EXTI_Line4;	//选择中断线 3 | 4
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//中断模式
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	//下降沿触发
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;	//中断使能
	 EXTI_Init(&EXTI_InitStructure);	//根据指定参数初始化EXTI
}

void EXTI0_IRQHandler()	//外部中断0函数
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)	//判断中断线0有无中断发生
	{
		delay_ms(10);
		if(k_up==1)
		{
			led0=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);	//清除中断标志位
}

void EXTI3_IRQHandler()	//外部中断3函数
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)	//判断中断线3有无中断发生
	{
		delay_ms(10);
		if(k1==0)
		{
			led0=1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);	//清除中断标志位
}

void EXTI4_IRQHandler()	//外部中断4函数
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)	//判断中断线4有无中断发生
	{
		delay_ms(10);
		if(k0==0)
		{
			led0=~led0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);	//清除中断标志位
}
