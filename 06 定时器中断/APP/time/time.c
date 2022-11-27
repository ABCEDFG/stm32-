#include "time.h"
#include <LED.h>


/*
*   ���� = (pre * psc)/72M
*   ��λ��S
*   psc ����� 1����ʼ���Ѽ�
*   pre �� psc ȡֵ��Χ��0 ~ 65535
*   ��ΪC������Ҳ�� time.h �ļ�����ͷ�ļ�ֻ��ʹ�á���
*/

void Tim4_Init(u16 pre,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	 // ʹ�ܶ�ʱ��4 ʱ��
	TIM_TimeBaseInitStruct.TIM_Period=pre;	     // ���ö�ʱ������
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;	 // ���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	    // 1��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	// �������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	// ����жϱ�־λ
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;	// EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	 // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	// �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	 // ����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 
	TIM_Cmd(TIM4,ENABLE);
} 


void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)	// �ж������жϷ���
	{
		LED1=~LED1;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	// ����жϱ�־λ
}
