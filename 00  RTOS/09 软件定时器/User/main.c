#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);

/* 软件定时器控制块 */
static rt_timer_t swtmr1 = RT_NULL;
static rt_timer_t swtmr2 = RT_NULL;

/* 定时器回调函数 */
static void swtmr1_callback(void *parameter);
static void swtmr2_callback(void *parameter);

/* 定时器回调次数 */
static uint32_t timcb_coun1 = 0;
static uint32_t timcb_coun2 = 0;



int main()
{
	
	
	/* 创建 LED1 线程 */
	led1_thread = rt_thread_create(
					 "led1",                // 线程名
					 led1_thread_entry,     // 线程入口函数
					 RT_NULL,               // 线程入口函数参数
					 512,                   // 线程栈大小
					 3,                     // 线程优先级
					 20);                   // 线程时间片
	 
	/* 启动线程，开启调度 */
	if(led1_thread!=RT_NULL)
	{
		rt_thread_startup(led1_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建一个软件定时器 */
	swtmr1 = rt_timer_create(
						"swtmr1",  // 名称
						swtmr1_callback,  // 回调函数
						0,             // 超时函数的入口参数
						1000,          // 超时时间
						RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);  //定时器模式，一次性
	
	/* 启动定时器 */
	if(swtmr1 != RT_NULL)
	{
		rt_timer_start(swtmr1);                                                                                                                     
	}
	else
	{
		return -1;
	}
	
	/* 创建一个软件定时器 */
	swtmr2 = rt_timer_create(
						"swtmr2",  // 名称
						swtmr2_callback,  // 回调函数
						0,             // 超时函数的入口参数
						5000,          // 超时时间
						RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);  //定时器模式，一次性
	
	/* 启动定时器 */
	if(swtmr2 != RT_NULL)
	{
		rt_timer_start(swtmr2);
	}
	else
	{
		return -1;
	}
}


// LED1 线程
static void led1_thread_entry(void *parameter)
{
	while(1)
	{
		LED1=!LED1;
		rt_thread_delay(500);
		rt_kprintf("led1\n");
	}
}


/* 软件定时器1的回调函数 */
static void swtmr1_callback(void *parameter)
{
	uint32_t tick_num1;
	
	timcb_coun1++;
	
	tick_num1 = (uint32_t)rt_tick_get();  // 获取滴答定时器值
	
	rt_kprintf("time1 次数：%d\n", timcb_coun1);
	rt_kprintf("滴答定时器值：%d", tick_num1);
}


/* 软件定时器 2 的回调函数 */
static void swtmr2_callback(void *parameter)
{
	uint32_t tick_num2;
	
	timcb_coun2++;
	
	tick_num2 = (uint32_t)rt_tick_get();  // 获取滴答定时器值
	
	rt_kprintf("time2 次数：%d\n", timcb_coun2);
	rt_kprintf("滴答定时器值：%d", tick_num2);
}

