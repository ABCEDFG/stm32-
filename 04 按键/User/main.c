#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"


int main()
{
	u8 i=0; 
	u8 key=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Key_Init();
	Usart1_Init(115200);
	
	while(1)
	{
		key=Key_Scan(0);
		switch(key)
		{
			case(0):  break;
			case(1):  LED1=~LED1;  break;
			case(2):  LED1=~LED1;  break;
			case(3):  LED1=~LED1;  break;
			default: break;
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
