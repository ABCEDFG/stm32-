#include <key.h>
#include <SysTick.h>

void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ���ṹ��	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA,ENABLE);	//���� GPIOE GPIOA ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;		//IO�� 5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//���� ����
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	//GPIOE ��ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//���� GPIOA ʱ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;	//IO�� 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	//���� ����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOA ��ʼ��
}


// mode=0--����ɨ��
// mode=1--����ɨ��
u8 Key_Scan(u8 mode)
{
	static u8 kg;
	if(mode==1)
	{
		delay_ms(50);
		kg=1;
	}
	if(kg==1 && (k_up==1 || k0==0 || k1==0))
	{
		delay_ms(10);
		kg=0;
		if(k_up==1)
		{
			return 1;
		}
		if(k0==0)
		{
			return 2;
		}
		if(k1==0)
		{
			return 3;
		}
	}
	else
	{
		if(k_up==0 && k0==1 && k1==1)
		{
			kg=1;
		}
		return 0;
	}
	return 0;
}

