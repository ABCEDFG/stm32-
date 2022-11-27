#include "SysTick.h"
#include "rtthread.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数


extern volatile rt_uint8_t rt_interrupt_nest;

// 在 board.c 文件的 rt_hw_board_init() 里面将其设置为1
uint8_t OSRunning=0;  

#ifdef RT_THREAD_PRIORITY_MAX  // RT_THREAD_PRIORITY 定义了，说明要支持 RT_Thread
#define delay_osrunning OSRunning  // OS 是否运行标记 0：不运行  1：运行
#define delay_ostickspersec RT_TICK_PER_SECOND // OS 时钟节拍，即每秒调度次数
#define delay_osintnesting rt_interrupt_nest  // 中断嵌套级别，即中断嵌套次数
#endif


// us级延时，关闭任务调度(防止打断us级延时)
void delay_osschedlock()
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_enter_critical();
#endif
}

// us级延时，恢复任务调度
void delay_osschedunlock()
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_exit_critical();
#endif
}

// 调用 OS 自带的延时函数延时
// ticks: 延时的节拍数
void delay_ostimedly(u32 ticks)
{
#ifdef RT_THREAD_PRIORITY_MAX
	rt_thread_delay(ticks);
#endif
}


//初始化延迟函数
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void SysTick_Init(u8 SYSCLK)
{
	u32 reload;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;
	reload=SYSCLK/8;  // 每秒钟的次数
	reload*=1000000/delay_ostickspersec;  // 更据delay_ostickspersec 设定溢出时间
	
	fac_ms = 1000/delay_ostickspersec;  // 代表可以 OS 延时的最少单位
		
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;  // 开启 SYSTICK 中断
	SysTick->LOAD=reload;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;  // 开启 SYSTICK
}								    


//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{			
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;  // LOAD 的值
	
	ticks=nus*fac_us;  // 需要的节拍数
	delay_osschedlock();  // 阻止 OS 调度，防止打断 us 延时
	told=SysTick->VAL;  // 刚进入是的计数器值
	
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)
			{
				tcnt+=told-tnow;  // 注意 SYSTICK 是一个递减的计数器
			}
			else
			{
				tcnt+=reload-tnow+told;
			}
			told=tnow;
			if(tcnt>=ticks)  // 时间超过
				break;
		}
	}
	delay_osschedunlock();  // 恢复 OS 调度
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;							//清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
//	SysTick->VAL =0X00;       					//清空计数器	  	 

	/* OS 已经在运行，且不是在中断里（中断里不能任务调度） */
	if(delay_osrunning&&delay_osintnesting==0)
	{
		if(nms>=fac_ms)
		{
			delay_ostimedly(nms/fac_ms);  // OS 延时
		}
		nms%=fac_ms;  // OS 无法提供这么小的延时，使用普通方法进行
	}
	delay_us((u32)(nms*1000));  // 普通方式延时
} 


