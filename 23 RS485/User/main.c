#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "rs485.h"


/* δ���� */
/* ��ʵ����USART��Ч��Ϊ���ؽ��յ������� */

int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	RS485_Init(115200);
	
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
