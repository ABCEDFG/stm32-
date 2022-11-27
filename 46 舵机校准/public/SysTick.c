#include <SysTick.h>

static u8  fac_us=0;
static u16 fac_ms=0;

void SysTick_Init(u8 SYSCLK)
{
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//以AHB时钟除以8作为时钟源
		fac_us=SYSCLK/8;	
	  fac_ms=(u16)fac_us * 1000;
}

void delay_us(u32 nus)	//最多1.864s
{
		u32 temp;
		SysTick->LOAD=nus*fac_us;	//时间加载
		SysTick->VAL=0x00;	//清空定时器器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//打开定时器，开始倒数
		do
		{
				temp=SysTick->CTRL;	//读取定时器数值
		}while((temp&0x01)&&!(temp&(1<<16)));	//已打开定时器且定时结束
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭定时器
		SysTick->VAL=0x00;	//清空定时器
}

void delay_ms(u16 nms) 	//最多1.864s
{
		u32 temp;
		SysTick->LOAD=nms*fac_ms;	//时间加载
		SysTick->VAL=0x00;	//清空定时器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//打开定时器，开始倒数
		do
		{
				temp=SysTick->CTRL;	//读取定时器数值
		}while((temp&0x01)&&!(temp&(1<<16)));	//已打开定时器且定时结束
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭定时器
		SysTick->VAL=0x00;	//清空定时器
}
