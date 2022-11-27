#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc_temp.h"


int main()
{
	u8 i=0; 
	s32 temp;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	ADC_Temp_Init();
	
	while(1)
	{
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
			
			temp = ADC_Temp();
			if(temp < 0)
	 		{
				temp = -temp;
				printf("�ڲ��¶ȼ��ֵΪ��-%.2f��C\r\n",(float)temp/100.0);
			}
			else
			{
				printf("�ڲ��¶ȼ��ֵΪ��%.2f��C\r\n",(float)temp/100.0);
			}
		}
		delay_ms(10);
	}
}
