#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


int a1, a2;

void Task1(void *param)
{
	while (1)
	{
		a1 = 0;
		a2 = 1;
	}
}

void Task2(void *param)
{
	while (1)
	{
		a1 = 1;
		a2 = 0;
	}
}


int main()
{
	u8 i=0; 
	TaskHandle_t task1_Handle;
	TaskHandle_t task2_Handle;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	
	xTaskCreate(Task1, "Task1", 10, NULL, 1, &task1_Handle);
	xTaskCreate(Task2, "Task2", 10, NULL, 1, &task2_Handle);
	
	vTaskStartScheduler();
	
	while(1)
	{ 
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
