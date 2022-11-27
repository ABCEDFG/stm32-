#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "hwjs.h"


int main()
{
	u8 i=0; 
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	Hwjs_Init();
	
	while(1)
	{
		if(hw_rec_flag==1)
		{
			hw_rec_flag=0;
			printf("��������� %0.8X \r\n",hw_rec);  //��ӡ
			hw_rec=0;
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
