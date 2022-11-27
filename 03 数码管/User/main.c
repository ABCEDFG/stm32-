#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "smg.h"


u8 smg[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//         0    1    2    3	   4	5    6    7    8    9    a    b    c    d    e    f 


int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Smg_Init();
	Usart1_Init(115200);
	
	while(1)
	{
		for(i=0;i<16;i++)
		{
			GPIO_Write(GPIOC,smg[i]);
			GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);  //置1
		//	GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10); //置0
			delay_ms(1000);
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
