#include "board.h"
#include "rtthread.h"


/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t send_thread  = RT_NULL;
static rt_thread_t rec_thread = RT_NULL;

/* 线程主体函数 */
static void led1_thread_entry(void *parameter);
static void send_thread_entry(void *parameter);
static void rec_thread_entry(void *parameter);


/* 互斥量控制块 */
static rt_mutex_t text_mutex = RT_NULL;


/* 全局变量 */
u8 gvalue[2] = {0x00, 0x00};



int main()
{
	/* 创建信息队列 */
	text_mutex = rt_mutex_create("text_mutex", RT_IPC_FLAG_FIFO);   // 队列模式   RT_IPC_FLAG_PRIO  优先级模式

	if(text_mutex != RT_NULL)
		rt_kprintf("互斥量创建成功！\n");
	
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
	
	/* 创建发送信息线程 */
	send_thread = rt_thread_create(
						"send",             // 线程名字
						send_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片
	
	/* 启动发送信息线程 */
	if(send_thread != RT_NULL)
	{
		rt_thread_startup(send_thread);
	}
	else
	{
		return -1;
	}
	
	/* 创建信息接受线程 */
	rec_thread = rt_thread_create(
						"rec",             // 线程名字
						rec_thread_entry,  // 线程入口函数
						RT_NULL,                // 线程参数
						512,                    // 线程大小
						2,                      // 线程优先级
						20);                    // 线程时间片 
	
	/* 启动接收信息线程 */
	if(rec_thread != RT_NULL)
	{
		rt_thread_startup(rec_thread);
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


/* 发送线程 */
static void send_thread_entry(void *parameter)
{
	while(1)
	{
		rt_mutex_take(text_mutex, RT_WAITING_FOREVER);  // 获取互斥量，一直等待
		gvalue[0]++;
		rt_thread_delay(100);  // 延时 100ms
		gvalue[1]++;
		rt_mutex_release(text_mutex);  // 释放二值信号
		rt_thread_delay(100);  // 延时 100ms	
		rt_thread_yield();  // 放弃剩余时间片，进行一次线程切换
	}
}


/* 接受线程 */
static void rec_thread_entry(void *parameter)
{
	while(1)
	{
		rt_mutex_take(text_mutex, RT_WAITING_FOREVER);  // 获取互斥量，一直等待
		if(gvalue[0] == gvalue[1])
			rt_kprintf("成功！\n");
		else
			rt_kprintf("失败！\n");
		
		rt_mutex_release(text_mutex);  // 释放二值信号量
		rt_thread_delay(100);  // 延时 100ms
	}
}
