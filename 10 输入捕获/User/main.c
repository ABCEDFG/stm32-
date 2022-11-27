#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "input.h"


/*
*	最后算出的时间 t 单位为 us
*/


int main()
{
	u8 i=0; 
	u32 t=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Input_Init(0xffff,72);  // 以 1MHz 频率计数  最大值 65535
	
	while(1)
	{
		t = Input_Scan();
		
		if(t > 0)
		{
			printf("输入捕获时间：%dus \r\n",t);
			printf("输入捕获时间：%dms \r\n",t/1000);
		}
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
