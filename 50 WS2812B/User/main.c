#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "ws2812b.h"



int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	Ws2812b_Init();
	
	while(1)
	{
		
		i++;
		if(i%100==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
