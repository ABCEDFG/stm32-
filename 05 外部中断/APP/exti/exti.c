#include "exti.h"
#include <LED.h>
#include <SysTick.h>

/*
*  �ж�ͨ����EXTI1, EXTI2, EXTI3, EXTI4, EXTI9_5, EXTI15_10
*
*  �ж��ߣ�EXTI_Line0 ~ EXTI_Line15
*/

void Exti_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  // GPIO
	NVIC_InitTypeDef NVIC_InitStructure;  // �ж����ȼ�
	EXTI_InitTypeDef EXTI_InitStructure;  // �ⲿ�ж�
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	        //���� AFIO ʱ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//ѡ��GPIO E4 �ܽ������ⲿ�ж���·
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;		//IO�� 4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//���� ����
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	    //GPIOE ��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;	// EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	    // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                 // ����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	// ѡ���ж���  4
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	    // �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	// �½��ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  // �ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);	 //  ����ָ��������ʼ��EXTI
}


void EXTI4_IRQHandler()	// �ⲿ�ж�4����
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)	// �ж��ж���4�����жϷ���
	{
		delay_ms(10);
		LED1=~LED1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);	// ����жϱ�־λ
}
