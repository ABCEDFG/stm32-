#include <key.h>
#include <SysTick.h>

void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO初始化结构体	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	//开启 GPIOE GPIOA 时钟
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_13;		//IO口 5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉 输入
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	//GPIOE 初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//开启 GPIOA 时钟
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;	//IO口 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	//下拉 输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOA 初始化
}

// mode=0--单次扫描
// mode=1--连续扫描
u8 Key(u8 mode)
{
	static u8 kg;
	if(mode==1)
	{
		kg=1;
	}
	if(kg==1 && (k_up==1||k0==0||k1==0))
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
		if(k_up==0&&k0==1&&k1==1)
		{
			kg=1;
		}
		return 0;
	}
}

