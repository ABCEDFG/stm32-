#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"



int main()
{
	u8 i=0; 
	u16 val = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	ADCx_Init();
	
	while(1)
	{
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
			
			val = ADC_Data(ADC_Channel_1, 20);
			
			printf("检测AD值为：%d\r\n",val);
		}
		delay_ms(10);
	}
}
