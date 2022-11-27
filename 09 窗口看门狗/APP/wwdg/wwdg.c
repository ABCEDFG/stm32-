#include <wwdg.h>
#include <led.h>
#include <systick.h>


/*
*	����ֵ�����ֵΪ 0x7f, ��СֵΪ 0x40
*   WWDG_Prescaler_1��2��4��WWDG_Prescaler_8
*
*/


void Wwdg_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);	//�򿪿��Ź�ʱ��	
	WWDG_SetWindowValue(0x5f);	//���ô�������
	WWDG_SetPrescaler(WWDG_Prescaler_8);	//���÷�Ƶϵ��  
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;	//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	WWDG_Enable(0x7f);	//���ü�������ֵ ��ʹ�ܼ�����
	WWDG_ClearFlag();	//���״̬λ
	WWDG_EnableIT();	//���� WWDG �ж�
}


void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(0x7f);	//ι��  ���ü�������ֵ
	WWDG_ClearFlag();	//���״̬λ
	LED1=!LED1;
}
