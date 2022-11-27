#include <wwdg.h>
#include <led.h>
#include <systick.h>


/*
*	窗口值的最大值为 0x7f, 最小值为 0x40
*   WWDG_Prescaler_1，2，4，WWDG_Prescaler_8
*
*/


void Wwdg_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);	//打开看门狗时钟	
	WWDG_SetWindowValue(0x5f);	//设置窗口上限
	WWDG_SetPrescaler(WWDG_Prescaler_8);	//设置分频系数  
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;	//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	WWDG_Enable(0x7f);	//设置计数器初值 并使能计数器
	WWDG_ClearFlag();	//清除状态位
	WWDG_EnableIT();	//开启 WWDG 中断
}


void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(0x7f);	//喂狗  设置计数器的值
	WWDG_ClearFlag();	//清除状态位
	LED1=!LED1;
}
