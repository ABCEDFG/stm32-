#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"



// div ：PLLXTPRE分频
// pllm：PLLMUL倍频
void RCC_HSE_Config(u32 div,u32 pllm)	//自定义系统时间（修改时钟）
{
	RCC_DeInit();		//将外设RCC寄存器重设为缺省值（默认选项）
	RCC_HSEConfig(RCC_HSE_ON);	//设置外部高速晶振（HSE）
	if(RCC_WaitForHSEStartUp()==SUCCESS)	//等待HSE稳定且就绪
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	//设置AHB分频（HCLK）
		RCC_PCLK1Config(RCC_HCLK_Div2);		//设置低速ABH时钟分频（PCLK1）
		RCC_PCLK2Config(RCC_HCLK_Div1);		//设置高速ABH时钟分频（PCLK2）
		RCC_PLLConfig(div,pllm);	//设置PLL时钟及倍频系数
		RCC_PLLCmd(ENABLE);	//使能或者失能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);	//检查指定的RCC标志位设置与否，PLL就绪
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//PLL作系统时钟（SYSCLK）
		while(RCC_GetSYSCLKSource()!=0x08);	//返回用作系统时钟的时钟原，0x08:PLL作为系统时钟
	}
}


int main()
{
	u8 i=0; 
	
	RCC_HSE_Config(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);  // 72MHz
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
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
