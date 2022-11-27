#include <time.h>
#include <LED.h>

void Tim4_Init(u16 pre,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//使能定时器4 时钟
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//设置定时器周期
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;	//设置定时器预分频器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	// 1分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	//设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//清除中断标志位
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;	//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	 
	TIM_Cmd(TIM4,ENABLE);
} 

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)	//判断有无中断发生
	{
		led0=~led0;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//清除中断标志位
}
