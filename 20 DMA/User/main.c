#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "dma.h"
#include "key.h"


#define SEND_LEN 50000
u8 send_buf[SEND_LEN];


//初始化数组
void Send_Data(u8 *p)
{
	u16 i;
	for(i=0;i<SEND_LEN;i++)
	{
		*p='5';
		p++;
	}
}


int main()
{
	u8 i = 0; 
	u8 key = 0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	LED_Init();
	Usart1_Init(115200);
	Key_Init();
	DMAx_Init(DMA1_Channel4,(u32)&USART1->DR,(u32)send_buf,SEND_LEN);  
	Send_Data(send_buf);
	
	while(1)
	{
		key = Key_Scan(0);
		if(key == 1)
		{
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1DMA发送
			DMAx_Enable(DMA1_Channel4,SEND_LEN);  //开启一次DMA传输
			
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_GL4)!=0)
				{
					DMA_ClearFlag(DMA1_FLAG_GL4);   break;
				}
				LED2=!LED2;
				delay_ms(300);
			}
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
