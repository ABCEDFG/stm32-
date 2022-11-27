#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "ad7606.h"


int main()
{
	u8 i=0; 
	u8 j = 0;
	s16 DB_data[8] = {0};
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	AD7606_Init();
	
	
	while(1)
	{

		
		i++;
		if(i >= 50)
		{
			AD7606_Read(DB_data);  
			
			for(j = 6; j < 7; j++)
			{
				printf("CH%1d:%8.1f mv  0x%04x %6d\r\n", j+1, (float)(DB_data[j]*10000.0/32768), 
											(u16)(DB_data[j]^0x8000), (u16)(DB_data[j]^0x8000));
				delay_ms(10);
			}
			
			LED2 = !LED2;
			i = 0;
		}
		delay_ms(10);
	}
}
