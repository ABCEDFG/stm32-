#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "i2c.h"
#include "at24cxx.h"
#include "key.h"



int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 x = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	AT24CXX_Init();
	Key_Init();
	
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			x++;
			if(x > 255)
			{
				x = 255;
			}
			AT24CXX_WriteOneByte(0, x);
			printf("写入的数据是：%d \r\n", x);
		}
		
		if(key == 2)
		{
			x = AT24CXX_ReadOneByte(0);
			printf("读取的数据是：%d \r\n",x);
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
