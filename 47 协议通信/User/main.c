#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "pact.h"



int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	while(1)
	{
		i++;
		if(i%50==0)
		{
			printf("%d %d %d\r\n",pact_a,pact_b,pact_c);
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
