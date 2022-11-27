#include <hwjs.h>
#include <sysTick.h>

u32 hw_rec;  //接收码
u8  hw_rec_flag; //接受标志

void Hwjs_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开启管脚复用时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //上拉输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);  //选择GPIO管脚做为外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line9);  //清除中断标志位
	
	//设置外部中断模式
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;   //打开全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);
}

//返回高电平持续时间
u8 Hw_Jssj()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1)  //高电平
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
			tim = Hw_Jssj();  //获取此次高电平时间
			if(tim>=250)  //是 无用信号
			{
				break;
			}
		
			if(tim>=200 && tim<250)
			{
				ok=1;  //收起起始信号
			}
			else if(tim>=60 && tim<90)
			{
				data=1;  //收到数据 1
			}
			else if(tim>=10 && tim<50)
			{
				data=0;  //收到数据 0
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
	EXTI_ClearITPendingBit(EXTI_Line9);  //清除中断标志位
}
