#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"



// div ��PLLXTPRE��Ƶ
// pllm��PLLMUL��Ƶ
void RCC_HSE_Config(u32 div,u32 pllm)	//�Զ���ϵͳʱ�䣨�޸�ʱ�ӣ�
{
	RCC_DeInit();		//������RCC�Ĵ�������Ϊȱʡֵ��Ĭ��ѡ�
	RCC_HSEConfig(RCC_HSE_ON);	//�����ⲿ���پ���HSE��
	if(RCC_WaitForHSEStartUp()==SUCCESS)	//�ȴ�HSE�ȶ��Ҿ���
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//����AHB��Ƶ��HCLK��
		RCC_PCLK1Config(RCC_HCLK_Div2);		//���õ���ABHʱ�ӷ�Ƶ��PCLK1��
		RCC_PCLK2Config(RCC_HCLK_Div1);		//���ø���ABHʱ�ӷ�Ƶ��PCLK2��
		RCC_PLLConfig(div,pllm);	//����PLLʱ�Ӽ���Ƶϵ��
		RCC_PLLCmd(ENABLE);	//ʹ�ܻ���ʧ��PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	//���ָ����RCC��־λ�������PLL����
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//PLL��ϵͳʱ�ӣ�SYSCLK��
		while(RCC_GetSYSCLKSource()!=0x08);	//��������ϵͳʱ�ӵ�ʱ��ԭ��0x08:PLL��Ϊϵͳʱ��
	}
}


int main()
{
	u8 i=0; 
	
	RCC_HSE_Config(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);  // 72MHz
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	LED_Init();
	Usart1_Init(115200);
	
	while(1)
	{
		i++;
		if(i%50==0)
		{
			LED2=!LED2;
			i=0;
		}
		delay_ms(10);
	}
}
