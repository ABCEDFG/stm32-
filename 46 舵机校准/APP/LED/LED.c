#include "led.h"

void LED_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ���ṹ��
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//���� GPIOB ʱ��
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;		//IO�� 5
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//�������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//�������� 50MHz
		GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB ��ʼ��
		
		//GPIO_SetBits(GPIOB,GPIO_Pin_5);	//��ʼ��GPIOB 5
}
