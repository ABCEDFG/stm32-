#include <touch_key.h>
#include <SysTick.h>
#include <usart.h>


u16 touch_val=0;  //δ����ʱ�ĳ��ʱ��
#define Touch_ARR_MAX_VAL 0xffff  //���ֵ
#define TOUCH_GATE_VAL 100  //���ڴ�ֵ��Ϊ����Ч����


//���ݴ�����ʼ��
u8 Touch_Init(u8 psc)
{
	u8 i,j;
	u16 A[10],x;
	Touch_Key_Init(Touch_ARR_MAX_VAL,psc);
	
	for(i=0;i<10;i++)	//��ȡ10�ΰ��µĴ���ֵ
	{
		A[i]=Touch_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<10;i++)
	{
		for(j=i+1;j<10;j++)	//��С��������
		{
			if(A[i]>A[j])
			{
				x=A[i];
				A[i]=A[j];
				A[j]=x;
			}
		}
	}
	
	x=0;
	for(i=2;i<8;i++)	//ȡ�м�6����ֵ��� ȡƽ��ֵ
	{
		x+=A[i];
	}
	touch_val=x/6;
	
	printf("δ�����ĳ��ʱ��==%d \t\n",touch_val);
	if(touch_val>Touch_ARR_MAX_VAL/2)  //�жϲ���ʱ���Ƿ�����
	{
		return 1;	//������
	}
	return 0;
}


void Touch_Key_Init(u16 pre,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //���� GPIO ��ʼ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	//��ʱ����ʼ���ṹ��
	TIM_ICInitTypeDef TIM_ICIinitStructure;	//���벶���ʼ���ṹ��
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	 //ʹ�ܶ�ʱ��5 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;		//IO�� 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//����ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB ��ʼ��
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//���ö�ʱ������
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;	//���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	// 1��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	//�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);	//��ʼ����ʱ��5
	
	TIM_ICIinitStructure.TIM_Channel=TIM_Channel_2;	//ͨ��1
	TIM_ICIinitStructure.TIM_ICFilter=0x00;	//���˲�
	TIM_ICIinitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICIinitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	//һ��Ƶ
	TIM_ICIinitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;	//ֱ��ӳ��
	TIM_ICInit(TIM5,&TIM_ICIinitStructure);	//���벶���ʼ��
	
	TIM_Cmd(TIM5,ENABLE);	//������ʱ��
}


//�ŵ�
void Touch_Reset()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //���� GPIO ��ʼ���ṹ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;		//IO�� 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//��©���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//�������ģʽΪ50M
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB ��ʼ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//����͵�ƽ �ŵ�
	delay_ms(5);  //�ȴ��ͷ����
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update|TIM_FLAG_CC2);  //�������Ͳ����־
	TIM_SetCounter(TIM5,0);	//������ֵ ��0
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB ��ʼ��
}


//���ص��ݳ��ʱ��
u16 Touch_Get_Val()
{
	Touch_Reset();
	
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_CC2)==0)	//�ȴ��ߵ�ƽ�����־
	{
		if(TIM_GetCounter(TIM5)>Touch_ARR_MAX_VAL-500)	//��ʱ��ֱ�ӷ���CNTֵ
		{
			return TIM_GetCounter(TIM5);
		}
	}
	
	return TIM_GetCapture2(TIM5);	//���ز���ߵ�ƽֵ
}





//���س�����ʱ��
//n ����
u16 Touch_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	
	while(n--)
	{
		temp=Touch_Get_Val();	//�õ�һ��ֵ
		if(temp>res)
		{
			res=temp;
		}
	}
	
	return res;
}



//0 ����ɨ��  1 ����ɨ�� 
//���� 1�д���  0�޴���
u8 Touch_Key_Scan(u8 mode)	
{
	u8 res=0; //״̬ 1�д��� 0�޴���
	u8 n=3;  //��������
	u16 x=0;  //���ʱ��
	static u8 keyen=0;
	
	if(mode)
	{
		n=6;
		keyen=0;
	}
	
	x=Touch_Get_MaxVal(n);  //���ʱ�����ֵ
	
	if(x>(TOUCH_GATE_VAL+touch_val) && x<(10*touch_val))  // ���ʱ����� ��Чֵ+��ֵ  �� С�� 10 ����ֵ
	{
		if(keyen==0)
		{
			res=1;
		}
		printf("�����󲶻�ߵ�ƽֵΪ��%d\r\n",x);
		keyen=3;
	}
	
	if(keyen)
	{
		keyen--;
	}
	
	return res;
}
