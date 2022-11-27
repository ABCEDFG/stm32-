#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "ad9954.h"


int main()
{
	u8 i=0; 
	u16 x = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	AD9954_Init();
	
	AD9954_Write_ASF(0x3fff);
	AD9954_Set_Fre(1000);
	//AD9954_Linear_Sweep(1,10,1,1,1,1,0);
	
	while(1)
	{
		
		//AD9954_SETFRE(1000);
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
