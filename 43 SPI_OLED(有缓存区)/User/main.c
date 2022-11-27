#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "oled.h"



int main()
{
	u8 i = 0; 
	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	OLED_Init();
	Usart1_Init(115200);
	
	
	OLED_Rectangle(10, 10, 20, 30, 0);
	OLED_Fill(); 
	
	while(1)
	{
		
		
		
		i++;
		if(i > 50)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
