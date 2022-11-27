#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "dac.h"


int main()
{
	u8 i=0; 
	u16 dac_value = 100;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	DAC1_Init();
	
	while(1)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R,dac_value);
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
