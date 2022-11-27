#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "flash.h" 
#include "key.h"


int main()
{
	u8 i = 0; 
	u8 key = 0;
	const u8 text_buf[]="Hello World!";
	u8 text[30];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	EN25QXX_Init();
	Key_Init();
	
	while(1)
	{
		key = Key_Scan(0);
		
		if (key == 1)
		{
			EN25QXX_Write((u8 *)text_buf,0,sizeof(text_buf));
			printf("保存的数据：%s\r\n",text_buf);
		}
		else if (key == 2)
		{
			EN25QXX_Read((u8 *)text, 0, sizeof(text_buf));
			printf("读取的数据：%s\r\n",text);
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
