#include <touch_key.h>
#include <SysTick.h>
#include <usart.h>


u16 touch_val=0;  //未按下时的充电时间
#define Touch_ARR_MAX_VAL 0xffff  //最大值
#define TOUCH_GATE_VAL 100  //大于此值认为是有效触摸


//电容触摸初始化
u8 Touch_Init(u8 psc)
{
	u8 i,j;
	u16 A[10],x;
	Touch_Key_Init(Touch_ARR_MAX_VAL,psc);
	
	for(i=0;i<10;i++)	//读取10次按下的触摸值
	{
		A[i]=Touch_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<10;i++)
	{
		for(j=i+1;j<10;j++)	//从小到大排序
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
	for(i=2;i<8;i++)	//取中间6个数值求和 取平均值
	{
		x+=A[i];
	}
	touch_val=x/6;
	
	printf("未触摸的充电时间==%d \t\n",touch_val);
	if(touch_val>Touch_ARR_MAX_VAL/2)  //判断测量时间是否正常
	{
		return 1;	//不正常
	}
	return 0;
}


void Touch_Key_Init(u16 pre,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义 GPIO 初始化结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;	//定时器初始化结构体
	TIM_ICInitTypeDef TIM_ICIinitStructure;	//输入捕获初始化结构体
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	 //使能定时器5 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA时钟	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;		//IO口 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//浮空模式
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB 初始化
	
	TIM_TimeBaseInitStruct.TIM_Period=pre;	//设置定时器周期
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;	//设置定时器预分频器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;	// 1分频
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; 	//设置向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);	//初始化定时器5
	
	TIM_ICIinitStructure.TIM_Channel=TIM_Channel_2;	//通道1
	TIM_ICIinitStructure.TIM_ICFilter=0x00;	//无滤波
	TIM_ICIinitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICIinitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;	//一分频
	TIM_ICIinitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;	//直接映射
	TIM_ICInit(TIM5,&TIM_ICIinitStructure);	//输入捕获初始化
	
	TIM_Cmd(TIM5,ENABLE);	//开启定时器
}


//放电
void Touch_Reset()
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义 GPIO 初始化结构体
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;		//IO口 0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//开漏输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	//设置输出模式为50M
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB 初始化
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//输出低电平 放电
	delay_ms(5);  //等待释放完成
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update|TIM_FLAG_CC2);  //清除溢出和捕获标志
	TIM_SetCounter(TIM5,0);	//计数器值 清0
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//浮空输入模式
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	//GPIOB 初始化
}


//返回电容充电时间
u16 Touch_Get_Val()
{
	Touch_Reset();
	
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_CC2)==0)	//等待高电平捕获标志
	{
		if(TIM_GetCounter(TIM5)>Touch_ARR_MAX_VAL-500)	//超时了直接返回CNT值
		{
			return TIM_GetCounter(TIM5);
		}
	}
	
	return TIM_GetCapture2(TIM5);	//返回捕获高电平值
}





//返回充电最大时间
//n 次数
u16 Touch_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	
	while(n--)
	{
		temp=Touch_Get_Val();	//得到一次值
		if(temp>res)
		{
			res=temp;
		}
	}
	
	return res;
}



//0 单次扫描  1 连续扫描 
//返回 1有触摸  0无触摸
u8 Touch_Key_Scan(u8 mode)	
{
	u8 res=0; //状态 1有触摸 0无触摸
	u8 n=3;  //测量次数
	u16 x=0;  //充电时间
	static u8 keyen=0;
	
	if(mode)
	{
		n=6;
		keyen=0;
	}
	
	x=Touch_Get_MaxVal(n);  //充电时间最大值
	
	if(x>(TOUCH_GATE_VAL+touch_val) && x<(10*touch_val))  // 充电时间大于 有效值+初值  且 小于 10 倍初值
	{
		if(keyen==0)
		{
			res=1;
		}
		printf("触摸后捕获高电平值为：%d\r\n",x);
		keyen=3;
	}
	
	if(keyen)
	{
		keyen--;
	}
	
	return res;
}
