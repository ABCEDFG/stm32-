#include <beep.h>


void Beep_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//开启 GPIOB 时钟
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;		//IO口 8
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//传输速率50MHz
	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	//GPIOB 初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_8);	//初始化GPIOB 8
}
