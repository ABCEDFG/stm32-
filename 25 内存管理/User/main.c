#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "malloc.h"



int main()
{
	u8 i = 0; 
	u8 key = 0;
	u8 *p=0;
	u8 sramx = 0;  // 0: 内部SRAM
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Key_Init();
	my_mem_init(SRAMIN);		//初始化内部内存池
	
	
	while(1)
	{
		key = Key_Scan(0);
		if(key == 1)
		{
			p=mymalloc(sramx,2*1024);//申请2K字节
			if(p!=NULL)
				printf("2K内存申请成功！\r\n");
		}
		
		i++;
		if(i%50==0)
		{
			printf("SRAMIN:%d%%\r\n",my_mem_perused(SRAMIN));  // 查看内存使用百分比
			
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
