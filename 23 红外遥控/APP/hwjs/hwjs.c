#include <hwjs.h>
#include <sysTick.h>

u32 hw_rec;  //������
u8  hw_rec_flag; //���ܱ�־

void Hwjs_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //����GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //�����ܽŸ���ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);  //ѡ��GPIO�ܽ���Ϊ�ⲿ�ж���·
	EXTI_ClearITPendingBit(EXTI_Line9);  //����жϱ�־λ
	
	//�����ⲿ�ж�ģʽ
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

//���ظߵ�ƽ����ʱ��
u8 Hw_Jssj()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)  //�ߵ�ƽ
	{
		t++;
		delay_us(20);
		if(t>=250)
		{
			return t;
		}
	}
	return t;
}

void EXTI9_5_IRQHandler(void)
{
	u8 tim=0;
	u8 ok=0;
	u8 data,num=0;
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)
		{
			tim = Hw_Jssj();  //��ȡ�˴θߵ�ƽʱ��
			if(tim>=250)  //�� �����ź�
			{
				break;
			}
		
			if(tim>=200 && tim<250)
			{
				ok=1;  //������ʼ�ź�
			}
			else if(tim>=60 && tim<90)
			{
				data=1;  //�յ����� 1
			}
			else if(tim>=10 && tim<50)
			{
				data=0;  //�յ����� 0
			}
		
			if(ok==1)
			{
				hw_rec <<= 1;
				hw_rec += data;
			
				if(num>=32)
				{
					hw_rec_flag=1;  break;
				}
			}
			num++;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line9);  //����жϱ�־λ
}
