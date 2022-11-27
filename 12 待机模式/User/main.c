#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "wkup.h"


int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	
	while(1)
	{
		printf("����ģʽ��\r\n");
		
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
			Wkup_Init();
		}
		delay_ms(10);
	}
}
