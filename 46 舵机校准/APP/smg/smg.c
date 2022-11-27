#include <smg.h>

void Smg_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO初始化结构体
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	//开启 GPIOC 时钟
		GPIO_InitStructure.GPIO_Pin=(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);		//IO口 5
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//传输速率 50MHz
		
		GPIO_Init(GPIOC,&GPIO_InitStructure); 	//GPIOB 初始化
}	