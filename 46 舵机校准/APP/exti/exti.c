#include <exti.h>
#include <SysTick.h>
#include <key.h>
#include <LED.h>

void Exti_Init()	//�������޿���ʱ�ӣ���ΪLED���ѿ���
{
	 NVIC_InitTypeDef NVIC_InitStructure;
	 EXTI_InitTypeDef EXTI_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//���� AFIO ʱ��SSS
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);	//ѡ��GPIO A0 �ܽ������ⲿ�ж���·
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);	//ѡ��GPIO E3 �ܽ������ⲿ�ж���·
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);	//ѡ��GPIO E4 �ܽ������ⲿ�ж���·
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;	//EXTI0�ж�ͨ��
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	//�����ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQͨ��ʹ��
	 NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;	//EXTI3�ж�ͨ��
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;	//�����ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQͨ��ʹ��
	 NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;	//EXTI3�ж�ͨ��
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//�����ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQͨ��ʹ��
	 NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 
	 EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//ѡ���ж��� 0	
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//�ж�ģʽ	
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;	//�����ش���		
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;	//�ж���ʹ��
	 EXTI_Init(&EXTI_InitStructure);	//����ָ��������ʼ��EXTI
	 
	 EXTI_InitStructure.EXTI_Line=EXTI_Line3|EXTI_Line4;	//ѡ���ж��� 3 | 4
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;	//�ж�ģʽ
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;	//�½��ش���
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;	//�ж�ʹ��
	 EXTI_Init(&EXTI_InitStructure);	//����ָ��������ʼ��EXTI
}

void EXTI0_IRQHandler()	//�ⲿ�ж�0����
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)	//�ж��ж���0�����жϷ���
	{
		delay_ms(10);
		if(k_up==1)
		{
			led0=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);	//����жϱ�־λ
}

void EXTI3_IRQHandler()	//�ⲿ�ж�3����
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)	//�ж��ж���3�����жϷ���
	{
		delay_ms(10);
		if(k1==0)
		{
			led0=1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);	//����жϱ�־λ
}

void EXTI4_IRQHandler()	//�ⲿ�ж�4����
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)	//�ж��ж���4�����жϷ���
	{
		delay_ms(10);
		if(k0==0)
		{
			led0=~led0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);	//����жϱ�־λ
}
