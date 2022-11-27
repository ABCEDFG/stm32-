#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "input.h"


/*
*	��������ʱ�� t ��λΪ us
*/


int main()
{
	u8 i=0; 
	u32 t=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	Input_Init(0xffff,72);  // �� 1MHz Ƶ�ʼ���  ���ֵ 65535
	
	while(1)
	{
		t = Input_Scan();
		
		if(t > 0)
		{
			printf("���벶��ʱ�䣺%dus \r\n",t);
			printf("���벶��ʱ�䣺%dms \r\n",t/1000);
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
