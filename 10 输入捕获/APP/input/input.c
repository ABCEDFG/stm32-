#include <input.h>


u8 sta = 0;   // sta & 0x3f  Ϊ��ʱ������Ĵ���
u16 val = 0;  // ��ʱ������ֵ


/* 
*	����Ƶ�ʼ��㹫ʽ 72MHz/psc
*   pre: Ϊ��ʱ�������ֵ
*   ���벶�������ʱ����㹫ʽ:  (((sta & 0x3f) * pre + val) * psc) / 72MHz
*   ��ʵ�����ܼ����������Զ�ʱ������Ƶ��
*   �� pre ȡ 0xffff�� psc ȡ 72 ʱ�� ����Ƶ��Ϊ 1us�� Input_Scan()�ķ���ֵ��λΪ 1us
*   ����ߵ�ƽʱ��
*
*   ��Ҫͬʱʹ�ö�·����ֱ������ͨ������
*/
void Input_Init(u16 pre,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //���� GPIO ��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	//��ʱ����ʼ���ṹ��
	TIM_ICInitTypeDef TIM_ICIinitStructure;	//���벶���ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;	//��ʱ���жϳ�ʼ���ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	//ʹ�ܶ�ʱ��5 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;		//IO�� 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIO ��ʼ��
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//���ö�ʱ������
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;	//���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	// 1��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	//�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);	//��ʼ����ʱ��5
	
	TIM_ICIinitStructure.TIM_Channel=TIM_Channel_1;	//ͨ��1
	TIM_ICIinitStructure.TIM_ICFilter=0x00;	//���˲�
	TIM_ICIinitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICIinitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	//һ��Ƶ
	TIM_ICIinitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;	//ֱ��ӳ��
	TIM_ICInit(TIM5,&TIM_ICIinitStructure);	//���벶���ʼ��
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);	//��������Ͷ�ʱ������ж�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;	//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	TIM_Cmd(TIM5,ENABLE);	//������ʱ��
}


/* ��ȡ���벶��ʱ�� */
u32 Input_Scan()
{
	u32 t = 0;
	
	if(sta&0x80)
	{
		t=sta&0x3f;
		t*=0xffff;
		t+=val;
		sta=0;
		val=0;
	}
	
	return t;
}


void TIM5_IRQHandler(void)
{
	if((sta & 0x80)==0)
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update))  //�����ж�
		{
			if(sta & 0x40)  //�Ƿ񲶻�͵�ƽ
			{
				if((sta & 0x3f)==0x3f)  //�Ƿ�ﵽ���ֵ
				{
					sta|=0x80;
					val=0xffff;
				}
				else
				{
					sta++;
				}
			}
		}
		if(TIM_GetITStatus(TIM5,TIM_IT_CC1))  //��������
		{
			if(sta & 0x40)  //�Ƿ񲶻�͵�ƽ
			{
				sta|=0x80;
				val=TIM_GetCapture1(TIM5);	//������벶��1��ֵ
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);	//�����ز���
			}
			else
			{
				sta=0;
				val=0;
				sta|=0x40;
				TIM_Cmd(TIM5,DISABLE);	//�رն�ʱ��
				TIM_SetCounter(TIM5,0);	//��ʱ����ֵΪ0
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);  //�½��ز���
				TIM_Cmd(TIM5,ENABLE);	//������ʱ��
			}
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);  //���״̬��־
}	
