#include <pwm.h>

void Tim3_Pwm_Init(u16 pre,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	//��ʱ����ʼ���ṹ��
	TIM_OCInitTypeDef TIM_OCInitStructure;	//PWM��ʼ���ṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	    //ʹ�ܶ�ʱ��3 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//ʹ�� GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//ʹ�� ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;		//IO�� 5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	//�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//�������� 50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //�ı�ָ���ܽŵ�ӳ��	
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//���ö�ʱ������
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;	//���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	// PWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;	//������� �͵�ƽ ��Ч
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	//���ʹ��
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR2 �ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3,ENABLE);	//ʹ�ܶ�ʱ��
	
	
}
