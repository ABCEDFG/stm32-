#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"


int main()
{
	u8 i=0; 
	u16 x=500;
	u8 fx=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Tim3_Pwm_Init(20000,72);
	
	while(1)
	{
		if(fx==0)
		{
			x++;
		}
		else
		{
			x--;
		}
		if(x==10000)
		{
			fx=0;
		}
		if(x==500)
		{
			fx=1;
		}
		TIM_SetCompare2(TIM3,x);	//i最大499 因为ARR最大499
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
