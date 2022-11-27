#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "iwdg.h"


int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Iwdg_Init(6,1000);  // 6.5536 S 复位
	
	while(1)
	{
		i++;
		//if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		Iwdg_FeedDog();
		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
	}
}
