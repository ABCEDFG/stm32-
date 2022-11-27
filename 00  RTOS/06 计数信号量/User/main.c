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


/* 信息量控制块 */
static rt_sem_t text_sem = RT_NULL;




int main()
{
	/* 创建信息队列 */
	text_sem = rt_sem_create(
				"text_sem",  // 信息队列名字
				10,  		 // 信号量初值，默认有一个信号量
				RT_IPC_FLAG_FIFO);  // 队列模式   RT_IPC_FLAG_PRIO  优先级模式

	if(text_sem != RT_NULL)
		rt_kprintf("信息队列创建成功！\n");
	
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
	rt_err_t erflag = 0;
	
	while(1)
	{
		if(Key_Scan(0) == 1)
		{
			erflag = rt_sem_release(text_sem);  // 释放一个计数信号量，最大值为 65535
			
			if(erflag == RT_EOK)
			{
				rt_kprintf("释放一个信号量\n");
			}
			else
			{
				rt_kprintf("无信号量可释放\n");
			}
			
			rt_thread_delay(20);  // 延时 20ms
		}
	}
}


/* 接受线程 */
static void rec_thread_entry(void *parameter)
{
	rt_err_t erflag =0;
	
	while(1)
	{
		if(Key_Scan(0) == 2)
		{
			erflag = rt_sem_take(text_sem, 0);  // 获取信号量，不等待
			
			if(erflag == RT_EOK)
			{
				rt_kprintf("申请信号量成功\n");
			}
			else
			{
				rt_kprintf("信号量已满\n");
			}
			
			rt_thread_delay(20);  // 延时 20ms
		}
	}
}
