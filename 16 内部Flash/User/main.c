#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "stm32_flash.h"


#define STM32_FLASH_SAVE_ADDR  0X08070000  // 设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

const u8 text_buf[]="Hello World!";

int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 read_buf[30];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Key_Init();
	
	while(1)
	{
		key = Key_Scan(0);
		
		if(key == 1)
		{
			STM32_FLASH_Write(STM32_FLASH_SAVE_ADDR,(u16*)text_buf,sizeof(text_buf));
			printf("写入数据为：%s\r\n",text_buf);
		}
		else if(key == 2)
		{
			STM32_FLASH_Read(STM32_FLASH_SAVE_ADDR,(u16 *)read_buf,sizeof(text_buf));
			printf("读取数据为：%s\r\n",read_buf);
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
