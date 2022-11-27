#include <input.h>


u8 sta = 0;   // sta & 0x3f  为定时器溢出的次数
u16 val = 0;  // 定时器计数值


/* 
*	计数频率计算公式 72MHz/psc
*   pre: 为定时器溢出的值
*   输入捕获溢出的时间计算公式:  (((sta & 0x3f) * pre + val) * psc) / 72MHz
*   其实就是总计数次数除以定时器计数频率
*   当 pre 取 0xffff， psc 取 72 时， 计数频率为 1us， Input_Scan()的返回值单位为 1us
*   捕获高电平时间
*
*   若要同时使用多路捕获，直接增加通道即可
*/
void Input_Init(u16 pre,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义 GPIO 初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	//定时器初始化结构体
	TIM_ICInitTypeDef TIM_ICIinitStructure;	//输入捕获初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;	//定时器中断初始化结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	//使能定时器5 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;		//IO口 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	//下拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIO 初始化
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//设置定时器周期
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;	//设置定时器预分频器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	// 1分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	//设置向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);	//初始化定时器5
	
	TIM_ICIinitStructure.TIM_Channel=TIM_Channel_1;	//通道1
	TIM_ICIinitStructure.TIM_ICFilter=0x00;	//无滤波
	TIM_ICIinitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICIinitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	//一分频
	TIM_ICIinitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;	//直接映射
	TIM_ICInit(TIM5,&TIM_ICIinitStructure);	//输入捕获初始化
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);	//开启捕获和定时器溢出中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;	//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM5,ENABLE);	//开启定时器
}


/* 获取输入捕获时间 */
u32 Input_Scan()
{
	u32 t = 0;
	
	if(sta&0x80)
	{
		t=sta&0x3f;
		t*=0xffff;
		t+=val;
		sta=0;
		val=0;
	}
	
	return t;
}


void TIM5_IRQHandler(void)
{
	if((sta & 0x80)==0)
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update))  //更新中断
		{
			if(sta & 0x40)  //是否捕获低电平
			{
				if((sta & 0x3f)==0x3f)  //是否达到最大值
				{
					sta|=0x80;
					val=0xffff;
				}
				else
				{
					sta++;
				}
			}
		}
		if(TIM_GetITStatus(TIM5,TIM_IT_CC1))  //发生捕获
		{
			if(sta & 0x40)  //是否捕获低电平
			{
				sta|=0x80;
				val=TIM_GetCapture1(TIM5);	//或的输入捕获1的值
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);	//上升沿捕获
			}
			else
			{
				sta=0;
				val=0;
				sta|=0x40;
				TIM_Cmd(TIM5,DISABLE);	//关闭定时器
				TIM_SetCounter(TIM5,0);	//定时器初值为0
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);  //下降沿捕获
				TIM_Cmd(TIM5,ENABLE);	//开启定时器
			}
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);  //清除状态标志
}	
